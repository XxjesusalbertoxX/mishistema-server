interface deviceRelation {
    id: string;
    name: string;
    description: string;
    code: string;
}

export class DeviceRelation {
    public readonly id: string;
    public name: string;
    public description: string;
    public code: string;

    constructor(device: deviceRelation) {
        this.id = device.id;
        this.name = device.name;
        this.description = device.description;
        this.code = device.code;
    }
}