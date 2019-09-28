class Websocket
{
    constructor()
    {
        /**
         * @type {WebSocket}
         */
        this.connection = undefined;

        /**
         * @type {number}
         */
        this.iteration = 0;
    }


    start() {
        let handler = this;
        this.connection = new WebSocket('ws://192.168.2.201:8763');

        this.connection.onopen = function ()
        {
        };

        this.connection.onerror = function (error)
        {
            console.log('socket Error: ' + error);
        };

        this.connection.onmessage = function (message)
        {
            handler._handleMessage(message.data);
        };
    }

    /**
     * @param {Message} message
     */
    send(message)
    {
        this.connection.send(JSON.stringify(message));
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
            case "userSettings":
                return this._handleUserSettings(data);
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

        this.iteration++;
        app.currentPage.onSystemStatus(status);
    }

    /**
     * @param {*} message
     * @private
     */
    _handleUserSettings(message)
    {
        let settings = new UserSettings(message.data);
        app.currentPage.onUserSettings(settings);
    }
}