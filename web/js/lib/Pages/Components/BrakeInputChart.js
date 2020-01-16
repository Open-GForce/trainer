class BrakeInputChart extends BarChart
{
    constructor()
    {
        super("Steuerimpulse");

        this.labels = ['Aussen', 'Innen'];
        this.colors = ['#b71c1c', '#689f38', '#1976d2'];
        this.data = [0, 0];
    }

    /**
     * @param {number} value
     */
    setOuterBrake(value)
    {
        this.data[0] = Math.round(value);
    }

    /**
     * @param {number} value
     */
    setInnerBrake(value)
    {
        this.data[1] = Math.round(value);
    }

    /**
     * @param {number} value
     */
    setTotalValue(value)
    {
        this.data[2] = Math.round(value);
    }

    /**
     * @inheritDoc
     */
    _getCanvas() {
        return $('#brakeInputChart');
    }
}