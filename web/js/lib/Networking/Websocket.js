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
        let socket = this;
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
            socket._handleMessage(message.data);
        };
    }

    /**
     * @param {*} data
     * @private
     */
    _handleMessage(data)
    {
        data = JSON.parse(data);

        switch (data.type) {
            case "processingStatus":
                return this._handleSystemStatus(data);
            default:
                console.log("Received unknown message type " + type);
        }
    }

    /**
     * @param {*} message
     * @private
     */
    _handleSystemStatus(message)
    {
        let status = new SystemStatus(
            message.data.engineStatus !== undefined ? new EngineStatus(message.data.engineStatus) : undefined,
            new BrakeInput(message.data.innerBrake),
            new BrakeInput(message.data.outerBrake),
            message.data['rotationDirection'],
            message.data['rotationSpeed'],
            message.data.maxSpeed,
            message.data.targetSpeed,
        );

        app.currentPage.onSystemStatus(status);
    }
}