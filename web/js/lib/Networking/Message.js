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

Message.REQUEST_GET_USER_SETTINGS           = 'getUserSettings';
Message.REQUEST_CREATE_USER_SETTINGS        = 'createUserSettings';
Message.REQUEST_DELETE_USER_SETTINGS        = 'deleteUserSettings';
Message.REQUEST_SWITCH_USER_SETTINGS        = 'switchUserSettings';
Message.REQUEST_GET_ACTIVE_USER_SETTINGS    = 'getActiveUserSettingsName';
Message.REQUEST_GET_AVAILABLE_USER_SETTINGS = 'getAvailableUserSettingsNames';
Message.REQUEST_GET_SYSTEM_SETTINGS         = 'getSystemSettings';
Message.REQUEST_TYPE_CONF_INNER_BRAKE       = 'setInnerBrakeRange';
Message.REQUEST_TYPE_CONF_OUTER_BRAKE       = 'setOuterBrakeRange';
Message.REQUEST_TYPE_CONF_ROT_RADIUS        = 'setRotationRadius';
Message.REQUEST_TYPE_CONF_ROT_RADIUS        = 'setRotationRadius';
Message.REQUEST_TYPE_CONF_SOFT_START        = 'setSoftStart';
Message.REQUEST_TYPE_CONF_ACC_STAGES        = 'setAccelerationStages';
Message.REQUEST_TYPE_CONF_UI_SETTINGS       = 'setUserInterfaceSettings';