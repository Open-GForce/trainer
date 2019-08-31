class App
{
    constructor()
    {
        /**
         * @type {Websocket}
         */
        this.socket = undefined;
    }

    start()
    {
        this.socket = new Websocket();
        this.socket.start();
    }
}

