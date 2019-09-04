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
         * @type {number}
         */
        this.iteration = 0;
    }

    start()
    {
        this.speedSlider = $('.control.segment .slider').slider({
                min: 0,
                max: 7,
                start: 2,
                step: 0.5
            });

        this.brakeChart = new BrakeInputChart();
        this.speedChart = new SpeedChart();
        this.brakeChart.show();
        this.speedChart.show();
    }

    /**
     * @param {SystemStatus} status
     */
    onSystemStatus(status)
    {
        this.iteration++;

        this.brakeChart.setInnerBrake(status.innerBrake.scaled * 100);
        this.brakeChart.setOuterBrake(status.outerBrake.scaled * 100);
        this.brakeChart.setTotalValue((status.innerBrake.scaled - status.outerBrake.scaled) * 100);
        this.brakeChart.update();

        this.speedChart.setCurrentSpeed(status.currentSpeed + (this.iteration * 0.2));
        this.speedChart.setMaxSpeed(1000);
        this.speedChart.setTargetSpeed(600);
    }
}