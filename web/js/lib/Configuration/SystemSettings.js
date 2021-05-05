class SystemSettings
{
    /**
     * @param {object} props
     */
    constructor(props)
    {
        /**
         * Force/speed lookup table
         * Key = Force in G
         * Value = Speed in U/min
         *
         * Both values are scaled by factor 100
         *
         * @type {Array}
         */
        this.forceTable = props.forceTable;
    }
}