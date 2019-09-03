class SpeedChart extends BarChart
{
    constructor() {
        super("Geschwindigkeit");

        this.labels = ['Ziel', 'Aktuell', 'Max'];
        this.colors = ['#1976d2', '#63a4ff','#004ba0'];
        this.data = [0, 0, 0];
    }

    /**
     * @inheritDoc
     */
    _getCanvas() {
        return $('#speedChart');
    }

    /**
     * @inheritDoc
     */
    _getConfig() {
        let options = this.baseOptions;
        options.scales.yAxes[0].ticks.max = 3500;

        return options;
    }
}