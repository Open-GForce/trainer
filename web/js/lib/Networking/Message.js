class Message
{
    /**
     * @param {string} type
     * @param {object} data
     */
    constructor(type, data)
    {
        this.type = type;
        this.data = data;
    }
}

Message.REQUEST_TYPE_MAX_SPEED = 'setMaxSpeed';
Message.REQUEST_TYPE_DIRECTION = 'setRotationDirection';