class BrakeInput
{
    /**
     * @param {*} props
     */
    constructor(props)
    {
        /**
         * Raw 16 Bit integer value
         *
         * @type {number}
         */
        this.raw = props.raw;

        /**
         * Scaled percentage 0 (0%) - 1 (100%)
         *
         * @type {number}
         */
        this.scaled = props.scaled;
    }
}