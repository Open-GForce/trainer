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
        this.statusSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.forceSegment = undefined;
    }

    start()
    {
        this.speedSlider = $('.control.segment .slider').slider({
                min: 0,
                max: 7,
                start: 2,
                step: 0.5
            });

        this.statusSegment = $('.status.segment');
        this.forceSegment = $('.force.segment');

        this.brakeChart = new BrakeInputChart();
        this.speedChart = new SpeedChart();
        this.brakeChart.show();
        this.speedChart.show();

        this._renderStatus(undefined);
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