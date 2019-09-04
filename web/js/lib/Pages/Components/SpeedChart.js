class SpeedChart extends GaugeChart
{
    constructor() {
        super();

        this.colors = ['#1976d2', '#bdbdbd','#004ba0'];
        this.valueColor = '#fff';
    }

    /**
     * @param {number} speed
     */
    setTarget(speed) {
        if (this.chart.data.datasets[0].gaugeLimits[1] > 0) {
            this.chart.data.datasets[0].gaugeLimits[1]++;
            return;
        }

        this.chart.data.datasets[0].gaugeLimits[1] = Math.round(speed);
    }

    /**
     * @param {number} speed
     */
    setCurrent(speed) {
        this.chart.data.datasets[0].gaugeData.value = 200;
    }

    /**
     * @param {number} speed
     */
    setMax(speed) {
        this.chart.data.datasets[0].gaugeLimits[2] = Math.round(speed);
    }

    /**
     * @inheritDoc
     */
    _getCanvas() {
        return $('#speedChart');
    }
}