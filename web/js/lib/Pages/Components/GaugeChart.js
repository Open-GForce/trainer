class GaugeChart
{
    constructor()
    {
        /**
         * @type {{showMarkers: boolean, events: Array}}
         */
        this.baseOptions = {
            events: [],
            showMarkers: true,
            responsive: true,
            arrowColor: '#fff',
            defaultFontColor: '#fff',
            animation: {
                animateRotate: false,
                animateArrow: false
            }
        };

        /**
         * @type {string[]}
         */
        this.colors = [];

        /**
         * @type {string}
         */
        this.valueColor = '';

        /**
         * @type {number[]}
         */
        this.limits = [];

        /**
         * @type {Chart}
         */
        this.chart = undefined;
    }

    show()
    {
        this.chart = new Chart(this._getCanvas(), {
            type: "tsgauge",
            data: {
                datasets: [{
                    backgroundColor: this.colors,
                    borderWidth: 0,
                    gaugeData: {
                        value: 0,
                        valueColor: this.valueColor,
                    },
                    gaugeLimits: [0, 0, 0]
                }]
            },
            options: this._getConfig()
        });
    }

    update()
    {
        this.chart.update();
    }

    /**
     * @protected
     * @return {*|jQuery.fn.init|jQuery|HTMLElement}
     */
    _getCanvas() {}

    /**
     * @return {*}
     * @protected
     */
    _getConfig()
    {
        return this.baseOptions;
    }
}