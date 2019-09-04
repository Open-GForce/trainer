class App
{
    constructor()
    {
        /**
         * @type {Websocket}
         */
        this.socket = undefined;

        /**
         * @type {TemplateRepository}
         */
        this.templates = undefined;

        /**
         * @type {OperationsPage}
         */
        this.currentPage = undefined;
    }

    start()
    {
        this.socket = new Websocket();
        this.socket.start();

        this.templates = new TemplateRepository();

        this.currentPage = new OperationsPage();
        this.currentPage.start();
    }
}

