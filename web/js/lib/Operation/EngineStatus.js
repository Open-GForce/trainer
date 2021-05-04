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

    /**
     * @return {boolean}
     */
    isFullyReleased()
    {
        return this.powerAmplifierReleased && this.inverterReady && this.inputDataReady;
    }

    /**
     * @param {EngineStatus} status
     * @returns {boolean}
     */
    isEqual(status)
    {
        return this.powerAmplifierReleased === status.powerAmplifierReleased
            && this.inverterReady === status.inverterReady
            && this.inputDataReady === status.inputDataReady
            && this.malfunction === status.malfunction
            && this.switchLeftActive === status.switchLeftActive
            && this.switchRightActive === status.switchRightActive
            && this.currentIntegratorSet === status.currentIntegratorSet
            && this.currentParameterSet === status.currentParameterSet;
    }
}