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
        let connection = new WebSocket('ws://192.168.2.102:8763');

        connection.onopen = function ()
        {
        };

        connection.onerror = function (error)
        {
            console.log('socket Error: ' + error);
        };

        connection.onmessage = function (message)
        {
            console.log(message);
        };
    }
}