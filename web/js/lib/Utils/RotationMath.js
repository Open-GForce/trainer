class RotationMath
{
    /**
     * @param speed rotation speed in 1/min
     * @return {number} Force in G
     */
    static speedToForce(speed)
    {
        let velocity = 4 * Math.pow(Math.PI, 2) * RotationMath.trainerRadius * Math.pow((speed/60), 2);
        return velocity / 9.81;
    }
}

/**
 * Radius of the G-Force trainer in meters
 *
 * @type {number}
 */
RotationMath.trainerRadius = 4;