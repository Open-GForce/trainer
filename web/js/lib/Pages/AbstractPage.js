class AbstractPage
{
    /**
     * @param {string} mainTemplateName
     */
    constructor(mainTemplateName)
    {
        this.mainTemplateName = mainTemplateName;
    }

    start()
    {
        app.templates.load(this.mainTemplateName, (mainTemplate) => {
            app.templates.load('connectionDimmer', (dimmerTemplate) => {
                let template = mainTemplate + dimmerTemplate;
                $('#content').html(template);
                app.socket.onPageChange();
                this._initialize();
            });
        })
    }

    shutdown()
    {
    }

    /**
     * Gets called as soon socket connection has been established
     */
    onSocketConnected()
    {
    }

    /**
     * @param {SystemStatus} status
     */
    onSystemStatus(status)
    {
    }

    /**
     * @param {UserSettings} settings
     */
    onUserSettings(settings)
    {
    }

    /**
     * @param {SystemSettings} settings
     */
    onSystemSettings(settings)
    {
    }

    /**
     * @param {string} name
     */
    onActiveConfiguration(name)
    {
    }

    /**
     * @param {string[]} names
     */
    onAvailableUserSettings(names)
    {
    }

    /**
     * @protected
     */
    _initialize()
    {
    }

    /**
     * Helper function for converting available user settings names to dropdown values
     *
     * @param names
     * @param active
     * @returns {*[]}
     */
    settingsNamesToDropdownValues(names, active)
    {
        let values = [];

        for (name of names) {
            let item = name === "default"
                ? {name: "Standard", value: "default"}
                : {name: name, value: name};

            if (name === active) {
                item.selected = true;
            }

            values.push(item);
        }

        return values;
    }

    /**
     * @param {jQuery} active
     * @param {jQuery} passive
     * @param {jQuery|undefined} passive2
     * @protected
     */
    toggleButtons(active, passive, passive2 = undefined)
    {
        active.addClass('active').addClass('white');
        passive.removeClass('active').removeClass('white');

        if (passive2 !== undefined) {
            passive2.removeClass('active').removeClass('white');
        }
    };
}