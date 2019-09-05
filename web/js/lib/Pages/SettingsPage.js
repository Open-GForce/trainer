class SettingsPage extends AbstractPage
{
    constructor()
    {
        super("settingsPage");

        /**
         * @type {jQuery}
         */
        this.brakeStatsSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.innerBrakeSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.outerBrakeSegment = undefined;

        /**
         * @type {SystemStatus}
         */
        this.lastSystemStatus = undefined;
    }

    _initialize()
    {
        this.brakeStatsSegment = $('#brakeStatsSegment');
        this._renderBrakeStats(undefined);

        this.innerBrakeSegment = $('#innerBrakeSegment');
        this.outerBrakeSegment = $('#outerBrakeSegment');

        this._configureBrakeRangeSegment(this.innerBrakeSegment, () => {
            return this.lastSystemStatus.innerBrake
        });

        this._configureBrakeRangeSegment(this.outerBrakeSegment, () => {
            return this.lastSystemStatus.outerBrake
        });
    }

    /**
     * @param {jQuery} segment
     * @param {function} brakeStatus
     * @private
     */
    _configureBrakeRangeSegment(segment, brakeStatus)
    {
        let minInput = segment.find('.minimum.input input');
        segment.find('.minimum.input .button').click(() => {
            minInput.val(brakeStatus().raw);
        });

        let maxInput = segment.find('.maximum.input input');
        segment.find('.maximum.input .button').click(() => {
            maxInput.val(brakeStatus().raw);
        });
    }

    /**
     * @inheritDoc
     */
    onSystemStatus(status)
    {
        this.lastSystemStatus = status;
        this._renderBrakeStats(status);
    }

    /**
     * @param {SystemStatus} status
     * @private
     */
    _renderBrakeStats(status)
    {
        let innerBrake = 0;
        let outerBrake = 0;

        if (status !== undefined) {
            innerBrake = status.innerBrake.raw;
            outerBrake = status.outerBrake.raw;
        }

        app.templates.load('rawBrakeStatistics', (template) => {
            let rendered = Mustache.render(template, {
                firstBrake: innerBrake,
                secondBrake: outerBrake
            });

            this.brakeStatsSegment.html(rendered);
        })
    }
}