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
    }

    /**
     * @protected
     */
    _initialize()
    {
        this.firstStatusHandled = false;

        this.statusSegment = $('.status.segment');
        this.forceSegment = $('.force.segment');
        this.controlSegment = $('.control.segment');

        this.directionButtons.right = this.controlSegment.find('.direction.buttons .right');
        this.directionButtons.left = this.controlSegment.find('.direction.buttons .left');

        this.modeButtons.simulation = this.controlSegment.find('.operation-mode.buttons .simulation');
        this.modeButtons.staticSpeed = this.controlSegment.find('.operation-mode.buttons .static-speed');

        this.releaseButton = this.controlSegment.find('.release.button');

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
    }

    /**
     * @inheritDoc
     */
    onUserSettings(settings)
    {
        RotationMath.trainerRadius = settings.rotationRadius;
        console.log(RotationMath.trainerRadius);
    }

    _bindControls()
    {
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

                let message = new Message(Message.REQUEST_TYPE_MAX_SPEED, {
                    speed: RotationMath.forceToSpeed(value)
                });
                app.socket.send(message);
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

        this.releaseButton.click(() => {
            this.releaseButton.addClass('disabled').addClass('loading');

            let message = new Message(Message.REQUEST_TYPE_RELEASE_STATUS, {
                released: !this.released
            });
            app.socket.send(message);
        });
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
}