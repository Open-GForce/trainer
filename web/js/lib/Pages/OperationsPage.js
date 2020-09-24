class OperationsPage extends AbstractPage
{
    constructor()
    {
        super('operationsPage');

        /**
         * @type {jQuery}
         */
        this.speedSliderElement = undefined;

        /**
         * @type {BrakeInputChart}
         */
        this.brakeChart = undefined;

        /**
         * @type {SpeedChart}
         */
        this.speedChart = undefined;

        /**
         * @type {jQuery}
         */
        this.controlSegment = undefined;

        /**
         * @type {{left: {jQuery}, right: {jQuery}}}
         */
        this.directionButtons = {
            right: undefined,
            left: undefined
        };

        /**
         * @type {{simulation: {jQuery}, staticSpeed: {jQuery}}}
         */
        this.modeButtons = {
            simulation: undefined,
            staticSpeed: undefined
        };

        /**
         * @type {jQuery}
         */
        this.releaseButton = undefined;

        /**
         * @type {jQuery}
         */
        this.statusSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.forceSegment = undefined;

        /**
         * Is engine released (based on system status)?
         *
         * @type {boolean}
         */
        this.released = false;

        /**
         * If true min. one system status has been handled
         *
         * @type {boolean}
         */
        this.firstStatusHandled = false;

        /**
         * @type {jQuery}
         */
        this.regularControlsContainer = undefined;

        /**
         * @type {jQuery}
         */
        this.adaptiveAccelerationControlsContainer = undefined;

        /**
         * @type {jQuery}
         */
        this.accelerationButton = undefined;

        /**
         * Is acceleration button visible
         *
         * @type {boolean}
         */
        this.accelerationButtonVisiable = false;

        /**
         * Show adaptive acceleration button instead of regular controls when released
         *
         * @type {boolean}
         */
        this.useAdaptiveAccelerationButton = false;

        /**
         * Max. speed limit (updated by system status)
         *
         * @type {number}
         */
        this.maxSpeed = 0;
    }

    /**
     * @protected
     */
    _initialize()
    {
        this.firstStatusHandled = false;

        let configRequest = new Message(Message.REQUEST_GET_USER_SETTINGS, {});
        app.socket.send(configRequest);

        this.statusSegment = $('.status.segment');
        this.forceSegment = $('.force.segment');
        this.controlSegment = $('.control.segment');

        this.directionButtons.right = this.controlSegment.find('.direction.buttons .right');
        this.directionButtons.left = this.controlSegment.find('.direction.buttons .left');

        this.modeButtons.simulation = this.controlSegment.find('.operation-mode.buttons .simulation');
        this.modeButtons.staticSpeed = this.controlSegment.find('.operation-mode.buttons .static-speed');

        this.releaseButton = this.controlSegment.find('.release.button');

        this.regularControlsContainer = this.controlSegment.find('.regularControls');
        this.adaptiveAccelerationControlsContainer = this.controlSegment.find('.adaptiveAccelerationControls');
        this.accelerationButton = this.adaptiveAccelerationControlsContainer.find('.acceleration.button');

        this.brakeChart = new BrakeInputChart();
        this.speedChart = new SpeedChart();
        this.brakeChart.show();
        this.speedChart.show();

        this._renderStatus(undefined);
        this._renderForce(undefined);
        this._bindControls();
    }

    onSocketConnected()
    {
        let configRequest = new Message(Message.REQUEST_GET_USER_SETTINGS, {});
        app.socket.send(configRequest);
    }

    /**
     * @param {SystemStatus} status
     */
    onSystemStatus(status)
    {
        this.controlSegment.removeClass('loading');

        this.maxSpeed = status.maxSpeed;

        if (!this.firstStatusHandled) {
            this.speedSliderElement.slider("set value", RotationMath.speedToForce(status.maxSpeed));
            this.firstStatusHandled = true;
        }

        this.brakeChart.setInnerBrake(status.innerBrake.scaled * 100);
        this.brakeChart.setOuterBrake(status.outerBrake.scaled * 100);
        this.brakeChart.setTotalValue((status.innerBrake.scaled - status.outerBrake.scaled) * 100);
        this.brakeChart.update();

        if (status.maxSpeed > 0) {
            this.speedChart.setCurrentSpeed(status.currentSpeed);
            this.speedChart.setMaxSpeed(status.maxSpeed);
            this.speedChart.setTargetSpeed(status.targetSpeed);
        }

        this._renderStatus(status);
        this._renderForce(status);
        this._renderControlButtons(status);
        this._renderReleaseButton(status);
        this._renderAccelerationButton(status);
    }

    /**
     * @inheritDoc
     */
    onUserSettings(settings)
    {
        RotationMath.trainerRadius = settings.rotationRadius;
        this.useAdaptiveAccelerationButton = settings.useAdaptiveAccelerationUserInterface;
    }

    _bindControls()
    {
        let page = this;

        this.speedSliderElement = $('.control.segment .slider');
        this.speedSliderElement.slider({
            min: 0,
            max: 7,
            start: 0,
            step: 0.5,
            onChange: function (value) {
                if (value === 0) {
                    return;
                }

                page._setMaxSpeed(RotationMath.forceToSpeed(value));
            }
        });

        this.directionButtons.right.click(() => {
            this._sendDirectionMessage('right');
        });

        this.directionButtons.left.click(() => {
            this._sendDirectionMessage('left');
        });

        this.modeButtons.staticSpeed.click(() => {
            this._sendOperationModeMessage('staticMaxSpeed');
        });

        this.modeButtons.simulation.click(() => {
            this._sendOperationModeMessage('regularSpiral');
        });

        this.accelerationButton.click(() => {
            this.accelerationButton.addClass('loading');
            setTimeout(() => {
                this.accelerationButton.removeClass('loading');
            }, 500);

            let maxForce = RotationMath.speedToForce(this.maxSpeed) + 0.5;
            page._setMaxSpeed(RotationMath.forceToSpeed(maxForce));
        });

        this.releaseButton.click(() => {
            this.releaseButton.addClass('disabled').addClass('loading');

            let message = new Message(Message.REQUEST_TYPE_RELEASE_STATUS, {
                released: !this.released
            });
            app.socket.send(message);
        });
    }

    /**
     * Sets the max. speed by sending a message to the controller
     *
     * @param {number} maxSpeed Max. speed in 1/min
     * @private
     */
    _setMaxSpeed(maxSpeed)
    {
        if (maxSpeed >= RotationMath.forceToSpeed(7)) {
            maxSpeed = RotationMath.forceToSpeed(7);
        }

        this.maxSpeed = maxSpeed;
        let message = new Message(Message.REQUEST_TYPE_MAX_SPEED, {
            speed: this.maxSpeed
        });
        app.socket.send(message);
    }

    /**
     * @param {string} direction
     * @private
     */
    _sendDirectionMessage(direction)
    {
        this.controlSegment.addClass('loading');

        let message = new Message(Message.REQUEST_TYPE_DIRECTION, {
            direction: direction
        });
        app.socket.send(message);

        if (direction === 'right') {
            this._toggleButtons(this.directionButtons.right, this.directionButtons.left);
        } else {
            this._toggleButtons(this.directionButtons.left, this.directionButtons.right);
        }
    }


    /**
     * @param {string} mode
     * @private
     */
    _sendOperationModeMessage(mode)
    {
        this.controlSegment.addClass('loading');

        let message = new Message(Message.REQUEST_TYPE_OPERATION_MODE, {
            mode: mode
        });
        app.socket.send(message);

        if (mode === 'staticMaxSpeed') {
            this._toggleButtons(this.modeButtons.staticSpeed, this.modeButtons.simulation);
        } else {
            this._toggleButtons(this.modeButtons.simulation, this.modeButtons.staticSpeed);
        }
    }

    /**
     * @param {jQuery} active
     * @param {jQuery} passive
     * @private
     */
    _toggleButtons(active, passive)
    {
        active.addClass('active').addClass('white');
        passive.removeClass('active').removeClass('white');
    };

    /**
     * @param {SystemStatus|undefined} status
     */
    _renderStatus(status)
    {
        app.templates.load('statusLabels', function (template) {
            let rendered = '';

            if (status === undefined || status.engineStatus === undefined) {
                rendered = Mustache.render(template, {
                    amplifierText: 'unbekannt',
                    amplifierColor: 'grey',
                    inverterText: 'unbekannt',
                    inverterColor: 'grey',
                    fieldBusText: 'unbekannt',
                    fieldBusColor: 'grey',
                    malfunctionText: 'unbekannt',
                    malfunctionColor: 'grey',
                });
            } else {
                rendered = Mustache.render(template, {
                    amplifierText: status.engineStatus.powerAmplifierReleased ? 'Endstufe bereit' : 'Endstufe gesperrt',
                    amplifierColor: status.engineStatus.powerAmplifierReleased ? 'green' : 'red',
                    inverterText: status.engineStatus.inverterReady ? 'Umrichter bereit' : 'Umrichter gesperrt',
                    inverterColor: status.engineStatus.inverterReady ? 'green' : 'red',
                    fieldBusText: status.engineStatus.powerAmplifierReleased ? 'Feldbus bereit' : 'Feldbus deaktiviert',
                    fieldBusColor: status.engineStatus.inputDataReady ? 'green' : 'red',
                    malfunctionText: status.engineStatus.malfunction ? 'Stoerung' : 'Keine Stoerung',
                    malfunctionColor: status.engineStatus.malfunction ? 'red' : 'green',
                });
            }

            app.currentPage.statusSegment.html(rendered);
        });
    }

    /**
     * @param {SystemStatus|undefined} status
     */
    _renderForce(status)
    {
        app.templates.load('forceStatistics', function (template) {
            let rendered = '';

            if (status === undefined) {
                rendered = Mustache.render(template, {
                    currentForce: '0.0',
                    maxForce: '0.0'
                });
            } else {
                rendered = Mustache.render(template, {
                    currentForce: RotationMath.speedToForce(status.currentSpeed).toFixed(1),
                    maxForce: RotationMath.speedToForce(status.maxSpeed).toFixed(1)
                });
            }

            app.currentPage.forceSegment.html(rendered);
        });
    }

    /**
     * @param {SystemStatus|undefined} status
     */
    _renderControlButtons(status)
    {
        if (status.direction === "right") {
            this._toggleButtons(this.directionButtons.right, this.directionButtons.left);
        } else {
            this._toggleButtons(this.directionButtons.left, this.directionButtons.right);
        }

        if (status.operationMode === "staticMaxSpeed") {
            this._toggleButtons(this.modeButtons.staticSpeed, this.modeButtons.simulation);
        } else {
            this._toggleButtons(this.modeButtons.simulation, this.modeButtons.staticSpeed);
        }
    }

    /**
     * @param {SystemStatus|undefined} status
     */
    _renderReleaseButton(status)
    {
        let released = status.engineStatus === undefined ? false : status.engineStatus.isFullyReleased();
        let statusChanged = released !== this.released;

        if (statusChanged && this.releaseButton.hasClass('loading')) {
            this.releaseButton.removeClass('loading').removeClass('disabled');
        }

        this.released = released;

        if (this.released) {
            this.releaseButton
                .removeClass('blue').addClass('red')
                .html('Anhalten');
        } else {
            this.releaseButton
                .removeClass('red').addClass('blue')
                .html('Freigeben');
        }
    }

    /**
     * @param {SystemStatus|undefined} status
     */
    _renderAccelerationButton(status)
    {
        console.log(this.useAdaptiveAccelerationButton);
        if (!this.useAdaptiveAccelerationButton) {
            return;
        }

        if (!this.released && this.accelerationButtonVisiable) {
            this.adaptiveAccelerationControlsContainer.hide();
            this.regularControlsContainer.show();
            this.accelerationButtonVisiable = false;
            return;
        }

        if (this.released && !this.accelerationButtonVisiable) {
            let containerHeight = this.regularControlsContainer.height();
            this.accelerationButton.height(containerHeight);

            this.regularControlsContainer.hide();
            this.adaptiveAccelerationControlsContainer.show();
            this.accelerationButtonVisiable = true;
        }
    }
}