class BarChart
{
    constructor(name)
    {
        /**
         * @type {{legend: {display: boolean}, responsive: boolean, scales: {yAxes: {ticks: {max: number, beginAtZero: boolean}}[]}}}
         */
        this.baseOptions = {
            responsive: false,
            legend: {
                display: false
            },
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: true,
                        max: 100,
                    }
                }]
            }
        };

        /**
         * @type {string}
         */
        this.name = name;

        /**
         * @type {string[]}
         */
        this.labels = [];

        /**
         * @type {string[]}
         */
        this.colors = [];

        /**
         * @type {number[]}
         */
        this.data = [];

        /**
         * @type {Chart}
         */
        this.chart = undefined;
    }

    show()
    {
        this.chart = new Chart(this._getCanvas(), {
            type: 'bar',
            data: {
                labels: this.labels,
                datasets: [{
                    label: this.name,
                    data: this.data,
                    backgroundColor: this.colors,
                    borderWidth: 1
                }]
            },
            options: this._getConfig()
        });
    }

    update()
    {
        this.chart.update(0);
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