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

        this._configureBrakeRangeSegment(this.innerBrakeSegment, Message.REQUEST_TYPE_CONF_INNER_BRAKE, () => {
            return this.lastSystemStatus.innerBrake
        });

        this._configureBrakeRangeSegment(this.outerBrakeSegment, Message.REQUEST_TYPE_CONF_OUTER_BRAKE, () => {
            return this.lastSystemStatus.outerBrake
        });

        let configRequest = new Message(Message.REQUEST_GET_USER_SETTINGS, {});
        app.socket.send(configRequest);
    }

    /**
     * @param {jQuery} segment
     * @param {string} messageType
     * @param {function} brakeStatus
     * @private
     */
    _configureBrakeRangeSegment(segment, messageType, brakeStatus)
    {
        let minInput = segment.find('.minimum.input input');
        segment.find('.minimum.input .button').click(() => {
            minInput.val(brakeStatus().raw);
        });

        let maxInput = segment.find('.maximum.input input');
        segment.find('.maximum.input .button').click(() => {
            maxInput.val(brakeStatus().raw);
        });

        let saveButton = segment.find('.save.button');
        saveButton.click(() => {
            let message = new Message(messageType, {
                min: parseInt(minInput.val()),
                max: parseInt(maxInput.val())
            });
            app.socket.send(message);
            this._saveAnimation(saveButton);
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
     * @inheritDoc
     */
    onUserSettings(settings)
    {
        this.innerBrakeSegment.find('.minimum.input input').val(settings.innerBrakeRange.min);
        this.innerBrakeSegment.find('.maximum.input input').val(settings.innerBrakeRange.max);

        this.outerBrakeSegment.find('.minimum.input input').val(settings.outerBrakeRange.min);
        this.outerBrakeSegment.find('.maximum.input input').val(settings.outerBrakeRange.max);
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

    /**
     * @param {jQuery} button
     * @private
     */
    _saveAnimation(button)
    {
        button.html('<i class="ui checkmark icon"></i>Gespeichert!')
            .removeClass('blue')
            .addClass('green')
            .addClass('disabled');

        setTimeout(function () {
            button.html('Speichern')
                .removeClass('green')
                .addClass('blue')
                .removeClass('disabled');
        }, 3000);
    }
}