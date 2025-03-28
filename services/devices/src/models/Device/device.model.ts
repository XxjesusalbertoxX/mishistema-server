export interface device {
    id: string,
    name: string,
    description: string,
    code: string,
    is_on: boolean,
    created_at: Date,
    updated_at: Date,
    deleted_at: Date | null,
}

class Device {
    public readonly id: string;
    public name: string;
    public description: string;
    public code: string;
    public is_on: boolean;
    public created_at: Date;
    public updated_at: Date;
    public deleted_at: Date | null;

    constructor(device: device) {
        this.id = device.id;
        this.name = device.name;
        this.description = device.description
        this.code = device.code;
        this.is_on = device.is_on;
        this.created_at = device.created_at;
        this.updated_at = device.updated_at;
        this.deleted_at = device.deleted_at;
    }
}

export default Device;