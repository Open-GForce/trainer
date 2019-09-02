class App
{
    constructor()
    {
        /**
         * @type {Websocket}
         */
        this.socket = undefined;

        /**
         * @type {OperationsPage}
         */
        this.currentPage = undefined;
    }

    start()
    {
        this.socket = new Websocket();
        this.socket.start();

        this.currentPage = new OperationsPage();
        this.currentPage.start();
    }
}

