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
     * @protected
     */
    _initialize()
    {
    }

    /**
     * @param {jQuery} active
     * @param {jQuery} passive
     * @protected
     */
    toggleButtons(active, passive)
    {
        active.addClass('active').addClass('white');
        passive.removeClass('active').removeClass('white');
    };
}