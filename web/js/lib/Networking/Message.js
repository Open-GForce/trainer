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
Message.REQUEST_TYPE_OPERATION_MODE = 'setOperationMode';
Message.REQUEST_TYPE_RELEASE_STATUS = 'setReleaseStatus';

Message.REQUEST_GET_USER_SETTINGS     = 'getUserSettings';
Message.REQUEST_TYPE_CONF_INNER_BRAKE = 'setInnerBrakeRange';
Message.REQUEST_TYPE_CONF_OUTER_BRAKE = 'setOuterBrakeRange';
Message.REQUEST_TYPE_CONF_ROT_RADIUS   = 'setRotationRadius';
Message.REQUEST_TYPE_CONF_SOFT_START   = 'setSoftStart';
Message.REQUEST_TYPE_CONF_ACC_STAGES   = 'setAccelerationStages';