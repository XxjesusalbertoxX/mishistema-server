import UserDevice from "./userDevice.model";

export interface userWithDevices {
    id: string;
    name: string;
    phone: string;
    email: string;
    userDevices: UserDevice[];
}

class UserWithDevices {
    public readonly id: string;
    public name: string;
    public phone: string;
    public email: string;
    public userDevices: UserDevice[];

    constructor(userWithDevices: userWithDevices){
        this.id = userWithDevices.id;
        this.name = userWithDevices.name;
        this.phone = userWithDevices.phone;
        this.email = userWithDevices.email;
        this.userDevices = userWithDevices.userDevices;
    }
}

export default UserWithDevices