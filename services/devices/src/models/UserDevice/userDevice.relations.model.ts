import {DeviceRelation} from "../Device/deviceRelations";
import {UserRelation} from "../User/userRelations.model";

export interface userDeviceWithRelations {
    id: string;
    name: string;
    user_id: string;
    user: UserRelation;
    device_id: string;
    device: DeviceRelation;
    is_on: boolean;
}

class UserDeviceWithRelations {
    public readonly id: string;
    public name: string;
    public user_id: string;
    public user: UserRelation;
    public device_id: string;
    public device: DeviceRelation;
    public is_on: boolean;

    constructor(userDevice: userDeviceWithRelations) {
        this.id = userDevice.id;
        this.name = userDevice.name
        this.user_id = userDevice.user_id;
        this.user = userDevice.user;
        this.device_id = userDevice.device_id;
        this.device = userDevice.device;
        this.is_on = userDevice.is_on;
    }
}

export default UserDeviceWithRelations;