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
        app.templates.load(this.mainTemplateName, (template) => {
            $('#content').html(template);
            this._initialize();
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
     * @protected
     */
    _initialize()
    {
    }
}