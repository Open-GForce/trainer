class SystemStatus
{
    /**
     * @param {EngineStatus} engineStatus
     * @param {BrakeInput} innerBrake
     * @param {BrakeInput} outerBrake
     * @param {string} direction
     * @param {number} currentSpeed
     * @param {number} maxSpeed
     * @param {number} targetSpeed
     * @param {string }operationMode
     */
    constructor(engineStatus, innerBrake, outerBrake, direction, currentSpeed, maxSpeed, targetSpeed, operationMode)
    {
        this.engineStatus = engineStatus;
        this.innerBrake = innerBrake;
        this.outerBrake = outerBrake;
        this.direction = direction;
        this.currentSpeed = currentSpeed;
        this.maxSpeed = maxSpeed;
        this.targetSpeed = targetSpeed;
        this.operationMode = operationMode;
    }
}