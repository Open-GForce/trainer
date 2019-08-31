class Websocket
{
    constructor()
    {
        /**
         * @type {socket}
         */
        this.connection = undefined;
    }


    start() {
        let connection = new WebSocket('ws://localhost:8418');

        connection.onopen = function ()
        {
        };

        connection.onerror = function (error)
        {
            console.log('socket Error: ' + error);
        };

        connection.onmessage = function (e)
        {

        };
    }
}