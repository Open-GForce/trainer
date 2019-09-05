class RotationMath
{
    /**
     * @param {number} speed rotation speed in 1/min
     * @return {number} Force in G
     */
    static speedToForce(speed)
    {
        let velocity = 4 * Math.pow(Math.PI, 2) * RotationMath.trainerRadius * Math.pow((speed/60), 2);
        return velocity / 9.81;
    }

    /**
     * @param {number} force Force in G
     * @return {number} rotation speed in 1/min
     */
    static forceToSpeed(force)
    {
        let velocity = force * 9.81;
        let speed = Math.sqrt(velocity / (4 * Math.pow(Math.PI, 2) * RotationMath.trainerRadius));

        return speed * 60;
    }
}

/**
 * Radius of the G-Force trainer in meters
 *
 * @type {number}
 */
RotationMath.trainerRadius = 4;