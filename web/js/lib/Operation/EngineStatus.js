class EngineStatus 
{
    /**
     * @param {*} props
     */
    constructor(props)
    {
        /**
         * @type {boolean}
         */
        this.powerAmplifierReleased = props.powerAmplifierReleased;

        /**
         * @type {boolean}
         */
        this.inverterReady = props.inverterReady;

        /**
         * @type {boolean}
         */
        this.inputDataReady = props.inputDataReady;

        /**
         * @type {boolean}
         */
        this.malfunction = props.malfunction;

        /**
         * @type {boolean}
         */
        this.switchLeftActive = props.switchLeftActive;

        /**
         * @type {boolean}
         */
        this.switchRightActive = props.switchRightActive;

        /**
         * @type {number}
         */
        this.currentIntegratorSet = props.currentIntegratorSet;

        /**
         * @type {number}
         */
        this.currentParameterSet = props.currentParameterSet;
    }
}