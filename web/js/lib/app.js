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
         * @type {NavigationController}
         */
        this.navigation = undefined;

        /**
         * @type {AbstractPage}
         */
        this.currentPage = undefined;
    }

    start()
    {
        this.socket = new Websocket();
        this.socket.start();

        this.templates = new TemplateRepository();

        this.navigation = new NavigationController();
        this.navigation.start();
    }
}

