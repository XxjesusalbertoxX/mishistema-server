import Device from "../Device/device.model";

export interface UserDeviceRequest {
    name: string;
    user_id: string;
    device_id: string;
}

export interface userdeviceUpdate {
    is_on?: boolean;
}
