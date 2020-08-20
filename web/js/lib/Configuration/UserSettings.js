class UserSettings
{
    /**
     * @param {object} props
     */
    constructor(props)
    {
        /**
         * @type {{min: {number}, max: {number}}}
         */
        this.innerBrakeRange = {
            min: props.innerBrakeRange.min,
            max: props.innerBrakeRange.max,
        };

        /**
         * @type {{min: {number}, max: {number}}}
         */
        this.outerBrakeRange = {
            min: props.outerBrakeRange.min,
            max: props.outerBrakeRange.max,
        };

        /**
         * @type {number}
         */
        this.rotationRadius = props.rotationRadius;

        /**
         * @type {{acceleration: {number}, speed: {number}}}
         */
        this.softStart = {
            speed: props.softStartSpeed,
            acceleration: props.softStartAcceleration
        }
    }
}