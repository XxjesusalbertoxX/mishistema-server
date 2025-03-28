import {db} from '../../../database/index';
import {users, devices} from "../../../database/schemas";
import {eq, and} from "../../../database/node_modules/drizzle-orm";


import {DeviceRequest} from "../models/Device/device.request.model";
import Device from "../models/Device/device.model";
import {DeviceUpdate} from "../models/Device/device.update";

const createDevice = async (data: DeviceRequest) => {
    const device = await db.insert(devices).values(data).returning()

    return new Device(device[0]);
}

const getAllDevices = async () => {
    const allDevices = await db.query.devices.findMany({
        where: eq(devices.is_on, true)
    });
    return allDevices.map(device => new Device(device));
}

const getDeviceByCode = async (code: string) => {
    const device = await db.query.devices.findFirst({
        where: and(
            eq(devices.code, code),
            eq(devices.is_on, true)
        )
    })

    if (!device) {
        throw new Error("Device not found");
    }

    return new Device(device);

}

const getDeviceById = async (id: string) => {
    const device = await db.query.devices.findFirst({
        where: and(
            eq(devices.id, id),
            eq(devices.is_on, true)
        )
    })

    if (!device) {
        throw new Error("Device not found");
    }

    return new Device(device);
}


const updateDevice = async (id: string, data: DeviceUpdate) => {
    const deviceToUpdate = await db.query.devices.findFirst({
        where: and(
            eq(devices.id, id),
            eq(devices.is_on, true)
        )
    });

    if (!deviceToUpdate) {
        throw new Error("Device not found");
    }

    const deviceUpdated:Device = {
        ...deviceToUpdate,
        ...data
    }

    const device = await db.update(devices).set(deviceUpdated).where(eq(devices.id, id)).returning();

    return new Device(device[0]);
}

const deleteDevice = async (id: string) => {
    const deviceToDelete = await db.query.devices.findFirst({
        where: and(
            eq(devices.id, id),
            eq(devices.is_on, true)
        )
    });

    if (!deviceToDelete) {
        throw new Error("Device not found");
    }

    const device = await db.update(devices).set({is_on: false}).where(eq(devices.id, id)).returning();

    return new Device(device[0]);
}


export const deviceServices = {
    createDevice,
    getAllDevices,
    getDeviceByCode,
    getDeviceById,
    updateDevice,
    deleteDevice
}