class SpeedChart
{
    constructor()
    {
        /**
         * DOM containing chart
         */
        this.container = undefined;

        /**
         * @type {Gauge}
         */
        this.currentChart = undefined;

        /**
         * @type {Gauge}
         */
        this.targetChart = undefined;

        this.currentSpeed = 0;
        this.targetSpeed  = 0;
        this.maxSpeed     = 0;
    }

    show()
    {
        this.container = $('#speedChart');

        let outerWidth = $('.speed.segment').width();
        let containerWidth = this.container.height() * 1.08;
        let margin = (outerWidth - containerWidth) / 2;

        this.container.css('width',  containerWidth + 'px');
        this.container.css('margin-left', margin + 'px');

        this.targetChart = Gauge(
            this.container[0], {
                max: this.maxSpeed,
                value: this.targetSpeed,
                showValue: false,
            },
        );

        this.currentChart = Gauge(
            this.container[0], {
                max: this.maxSpeed,
                value: this.currentSpeed,
                showValue: true,
                label: function(value) {
                    return Math.round(value) + ' U/min'
                },
            },
        );

        let first = $(this.container.find('svg')[0]);
        let overlay = $(this.container.find('svg')[1]);

        overlay.css('margin-top', -first.height() + 'px');
    }

    /**
     * @param {number} value
     */
    setMaxSpeed(value)
    {
        if (value === this.maxSpeed) {
            return;
        }

        this.maxSpeed = value;
        this.currentChart.setMaxValue(value);
        this.targetChart.setMaxValue(value);
    }

    /**
     * @param {number} value
     */
    setCurrentSpeed(value)
    {
        if (value === this.currentSpeed) {
            return;
        }

        this.currentSpeed = value;
        this.currentChart.setValue(value);
    }

    /**
     * @param {number} value
     */
    setTargetSpeed(value)
    {
        if (value === this.targetSpeed) {
            return;
        }

        this.targetSpeed = value;
        this.targetChart.setValue(value);
    }

}