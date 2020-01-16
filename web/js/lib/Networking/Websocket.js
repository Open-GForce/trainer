class Websocket
{
    constructor()
    {
        /**
         * @type {WebSocket}
         */
        this.connection = undefined;

        /**
         * @type {boolean}
         */
        this.connected = false;

        /**
         * @type {number}
         */
        this.iteration = 0;

        /**
         * @type {jquery}
         */
        this.dimmer = undefined;
    }

    start() {
        let handler = this;
        this._connect();

        // Interval for catching pending closing
        setInterval(function () {
            handler._updateConnectionStatus();
        }, 1000);

        setInterval(function () {
            if (!handler.connected) {
                handler._connect();
            }
        }, 3000);
    }

    /**
     * @param {Message} message
     */
    send(message)
    {
        this._updateConnectionStatus();
        this.connection.send(JSON.stringify(message));
    }

    /**
     * Gets called by AbstractPage if page context changes, so that dimmer status may be updated
     */
    onPageChange()
    {
        this.dimmer = $('#pendingConnectionDimmer');
        this._updateConnectionStatus();
        this._renderDimmer();
    }

    _connect()
    {
        let handler = this;
        if (this.connection !== undefined) {
            this.connection.onopen = function () {};
            this.connection.onerror = function () {};
            this.connection.onmessage = function () {};
            this.connection.onclose = function () {};
            this.connection.close();
        }

        this.connection = new WebSocket('ws://192.168.2.201:8763');

        this.connection.onopen = function ()
        {
            handler.connected = true;
            handler._renderDimmer();
        };

        this.connection.onerror = function (error)
        {
            handler.connected = false;
            handler._renderDimmer();
        };

        this.connection.onclose = function (error)
        {
            handler.connected = false;
            handler._renderDimmer();
        };

        this.connection.onmessage = function (message)
        {
            handler._handleMessage(message.data);
        };
    }

    _renderDimmer()
    {
        if (this.connected) {
            this.dimmer.removeClass('active');
        } else {
            this.dimmer.addClass('active');
        }
    }

    _updateConnectionStatus()
    {
        let newStatus = this.connection.readyState === this.connection.OPEN;
        if (newStatus !== this.connected) {
            this.connected = newStatus;
            this._renderDimmer();
        }
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
            message.data.operationMode
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