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
                max: 3400,
                value: 0,
                showValue: false,
            },
        );

        this.currentChart = Gauge(
            this.container[0], {
                max: 3400,
                value: 0,
                showValue: true,
                label: function(value) {
                    return Math.round(value) + ' 1/min'
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
        this.currentChart.setMaxValue(value);
        this.targetChart.setMaxValue(value);
    }

    /**
     * @param {number} value
     */
    setCurrentSpeed(value)
    {
        this.currentChart.setValue(value);
    }

    /**
     * @param {number} value
     */
    setTargetSpeed(value)
    {
        this.targetChart.setValue(value);
    }

}