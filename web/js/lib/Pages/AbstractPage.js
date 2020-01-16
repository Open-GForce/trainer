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
     * @protected
     */
    _initialize()
    {
    }
}