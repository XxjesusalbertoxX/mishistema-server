interface userDevice {
    id: string;
    name: string
    user_id: string;
    device_id: string;
    is_on: boolean;
}

class UserDevice {
    public readonly id: string;
    public name: string;
    public user_id: string;
    public device_id: string;
    public is_on: boolean;

    constructor(userDevice: userDevice) {
        this.id = userDevice.id;
        this.name = userDevice.name;
        this.user_id = userDevice.user_id;
        this.device_id = userDevice.device_id;
        this.is_on = userDevice.is_on;
    }
}

export default UserDevice;