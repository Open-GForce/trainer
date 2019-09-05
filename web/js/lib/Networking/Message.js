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

Message.REQUEST_TYPE_MAX_SPEED      = 'setMaxSpeed';
Message.REQUEST_TYPE_DIRECTION      = 'setRotationDirection';
Message.REQUEST_TYPE_RELEASE_STATUS = 'setReleaseStatus';

Message.REQUEST_TYPE_CONF_INNER_BRAKE = 'setInnerBrakeRange';
Message.REQUEST_TYPE_CONF_OUTER_BRAKE = 'setInnerOuterRange';