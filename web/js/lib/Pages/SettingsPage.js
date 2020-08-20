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
         * @type {jQuery}
         */
        this.forceCalculationSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.softStartSegment = undefined;

        /**
         * @type {jQuery}
         */
        this.accelerationStagesSegment = undefined;

        /**
         * @type {UserSettings}
         */
        this.settings = undefined;

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
        this.forceCalculationSegment = $('#forceCalculationSegment');
        this.softStartSegment = $('#softStartSegment');
        this.accelerationStagesSegment = $('#accelerationStagesSegment');

        this._configureBrakeRangeSegment(this.innerBrakeSegment, Message.REQUEST_TYPE_CONF_INNER_BRAKE, () => {
            return this.lastSystemStatus.innerBrake
        });

        this._configureBrakeRangeSegment(this.outerBrakeSegment, Message.REQUEST_TYPE_CONF_OUTER_BRAKE, () => {
            return this.lastSystemStatus.outerBrake
        });

        this._configureForceCalculationSegment();
        this._configureSoftStartSegment();
        this._configureAccelerationStagesSegment();

        let configRequest = new Message(Message.REQUEST_GET_USER_SETTINGS, {});
        app.socket.send(configRequest);

        $('body').css('overflow-y', 'scroll');
    }

    shutdown()
    {
        $('body').css('overflow-y', '');
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
     * @private
     */
    _configureForceCalculationSegment()
    {
        let radiusInput = this.forceCalculationSegment.find('.radius.input input');
        let saveButton = this.forceCalculationSegment.find('.save.button');

        saveButton.click(() => {
            let message = new Message(Message.REQUEST_TYPE_CONF_ROT_RADIUS, {
                rotationRadius: parseFloat(radiusInput.val().replace(',', '.')),
            });
            app.socket.send(message);
            this._saveAnimation(saveButton);
        });
    }

    /**
     * @private
     */
    _configureSoftStartSegment()
    {
        let speedInput = this.softStartSegment.find('.speed input');
        let accelerationInput = this.softStartSegment.find('.acceleration input');
        let saveButton = this.softStartSegment.find('.save.button');

        saveButton.click(() => {
            let message = new Message(Message.REQUEST_TYPE_CONF_SOFT_START, {
                speed: RotationMath.forceToSpeed(parseFloat(speedInput.val().replace(',', '.'))),
                acceleration: parseInt(accelerationInput.val())
            });
            app.socket.send(message);
            this._saveAnimation(saveButton);
        });
    }

    /**
     * @private
     */
    _configureAccelerationStagesSegment()
    {
        let saveButton = this.accelerationStagesSegment.find('.save.button');
        saveButton.click(() => {
            let message = new Message(Message.REQUEST_TYPE_CONF_ACC_STAGES, {
                stages: this.settings.accelerationStages
            });
            app.socket.send(message);
            this._saveAnimation(saveButton);
        });
    }

    /**
     * @private
     */
    _renderAccelerationStages()
    {
        let page = this;
        let segment = this.accelerationStagesSegment;
        let data = {
            stages: []
        };

        for (const stage of this.settings.accelerationStages) {
            data.stages.push({
                force: RotationMath.speedToForce(stage.speed).toFixed(2) + ' G',
                acceleration: stage.acceleration
            })
        }

        app.templates.load('accelerationStagesTable', function (template) {
            let rendered = Mustache.render(template, data);
            segment.find('table').replaceWith(rendered);

            segment.find('.delete.button').each(function (index) {
                $(this).click(function () {
                    page.settings.accelerationStages.splice(index, 1);
                    page._renderAccelerationStages();
                });
            });

            segment.find('.add.button').click(function () {
                let speedInput = segment.find('.speed input');
                let accelerationInput = segment.find('.acceleration input');

                page.settings.accelerationStages.push({
                    speed: RotationMath.forceToSpeed(parseFloat(speedInput.val().replace(',', '.'))),
                    acceleration: parseInt(accelerationInput.val())
                });

                page._renderAccelerationStages();
            });
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
        this.settings = settings;

        this.innerBrakeSegment.find('.minimum.input input').val(settings.innerBrakeRange.min);
        this.innerBrakeSegment.find('.maximum.input input').val(settings.innerBrakeRange.max);

        this.outerBrakeSegment.find('.minimum.input input').val(settings.outerBrakeRange.min);
        this.outerBrakeSegment.find('.maximum.input input').val(settings.outerBrakeRange.max);

        this.forceCalculationSegment.find('.radius.input input').val(settings.rotationRadius);

        this.softStartSegment.find('.speed input').val(RotationMath.speedToForce(settings.softStart.speed).toFixed(2));
        this.softStartSegment.find('.acceleration input').val(settings.softStart.acceleration);

        this._renderAccelerationStages();
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