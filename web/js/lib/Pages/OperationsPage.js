class OperationsPage
{
    constructor()
    {
        /**
         * @type {*}
         */
        this.speedSlider = undefined;

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
         * @type {jQuery}
         */
        this.statusSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.forceSegment = undefined;
    }

    start()
    {

        this.statusSegment = $('.status.segment');
        this.forceSegment = $('.force.segment');
        this.controlSegment = $('.control.segment');

        this.directionButtons.right = this.controlSegment.find('.direction.buttons .right');
        this.directionButtons.left = this.controlSegment.find('.direction.buttons .left');

        this.brakeChart = new BrakeInputChart();
        this.speedChart = new SpeedChart();
        this.brakeChart.show();
        this.speedChart.show();

        this._renderStatus(undefined);
        this._renderForce(undefined);
        this._bindControls();
    }

    /**
     * @param {SystemStatus} status
     */
    onSystemStatus(status)
    {
        this.brakeChart.setInnerBrake(status.innerBrake.scaled * 100);
        this.brakeChart.setOuterBrake(status.outerBrake.scaled * 100);
        this.brakeChart.setTotalValue((status.innerBrake.scaled - status.outerBrake.scaled) * 100);
        this.brakeChart.update();

        this.speedChart.setCurrentSpeed(status.currentSpeed);
        this.speedChart.setMaxSpeed(status.maxSpeed);
        this.speedChart.setTargetSpeed(status.targetSpeed);

        this._renderStatus(status);
        this._renderForce(status);
    }

    _bindControls()
    {
        this.speedSlider = $('.control.segment .slider').slider({
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
        })
    }

    /**
     * @param {string} direction
     * @private
     */
    _sendDirectionMessage(direction)
    {
        let message = new Message(Message.REQUEST_TYPE_DIRECTION, {
            direction: direction
        });
        app.socket.send(message);

        let toggleButtons = (active, passive) => {
           active.addClass('active').addClass('white');
           passive.removeClass('active').removeClass('white');
        };

        if (direction === 'right') {
            toggleButtons(this.directionButtons.right, this.directionButtons.left);
        } else {
            toggleButtons(this.directionButtons.left, this.directionButtons.right);
        }
    }

    /**
     * @param {SystemStatus|undefined} status
     */
    _renderStatus(status)
    {
        app.templates.load('statusLabels', function (template) {
            let rendered = '';

            if (status === undefined) {
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
                    inverterText: status.engineStatus.powerAmplifierReleased ? 'Endstufe bereit' : 'Endstufe gesperrt',
                    inverterColor: status.engineStatus.inverterReady ? 'green' : 'red',
                    fieldBusText: status.engineStatus.powerAmplifierReleased ? 'Feldbus bereit' : 'Feldbus deaktiviert',
                    fieldBusColor: status.engineStatus.inputDataReady ? 'green' : 'red',
                    malfunctionText: status.engineStatus.malfunction ? 'Störung' : 'Keine Störung',
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
}