class RotationMath
{
    /**
     * @param {SystemSettings} settings
     */
    static setSystemSettings = function (settings)
    {
        RotationMath.forceTable = new Map();
        RotationMath.speedTable = new Map();

        let sortedByForce = settings.forceTable.sort(function (a, b) {
            return b[0] - a[0];
        });

        for (let item of sortedByForce) {
            RotationMath.forceTable.set(item[0], item[1]);
            RotationMath.speedTable.set(item[1], item[0]);
        }

        console.log("Using the following force/speed lookup table:");
        console.log(RotationMath.forceTable);
    };

    /**
     * @param {number} speed rotation speed in 1/min
     * @return {number} Force in G
     */
    static speedToForce(speed)
    {
        if (this.speedTable === undefined) {
            return 0;
        }

        return RotationMath._lookup(speed, RotationMath.speedTable);
    }

    /**
     * @param {number} force Force in G
     * @return {number} rotation speed in 1/min
     */
    static forceToSpeed(force)
    {
        if (this.forceTable === undefined) {
            return 0;
        }

        return RotationMath._lookup(force, RotationMath.forceTable);
    }

    /**
     * @param {number} needle
     * @param {Map}    table
     * @private
     */
    static _lookup(needle, table)
    {
        needle = Math.round(needle * 100);
        let previousElement = undefined;

        for (let [key, value] of table) {
            if (key === needle) {
                return value / 100;
            }

            if (needle > key && previousElement === undefined) {
                return value / 100;
            }

            if (needle > key) {
                let deltaToKey = needle - key;
                let keyDeltaToPrevious = previousElement[0] - key;
                let valueDeltaToPrevious = previousElement[1] - value;
                let factor = deltaToKey / keyDeltaToPrevious;

                return ((factor * valueDeltaToPrevious) + value) / 100;
            }

            previousElement = [key, value];
        }

        return 0;
    }
}

/**
 * Force/Speed lookup table of SystemSettings sorted by force descending
 * Key   = Force (scaled by factor 100)
 * Value = Speed (scaled by factor 100)
 *
 * @type {Map|undefined}
 */
RotationMath.forceTable = undefined;

/**
 * Speed/Force lookup table of SystemSettings sorted by force descending
 * Key   = Force (scaled by factor 100)
 * Value = Speed (scaled by factor 100)
 *
 * @type {Map|undefined}
 */
RotationMath.speedTable = undefined;