import {userDevices, users} from "../../../database/schemas";
import {db} from '../../../database/index';
import {and, eq} from "../../../database/node_modules/drizzle-orm";
import {UserDeviceRequest, userdeviceUpdate} from "../models/UserDevice/userDevice.request.model";
import UserDevice from "../models/UserDevice/userDevice.model";
import UserDeviceWithRelations from "../models/UserDevice/userDevice.relations.model";
import UserWithDevices from "../models/UserDevice/userWithDevices.model";
import {UserRelation} from "../models/User/userRelations.model";
import {DeviceRelation} from "../models/Device/deviceRelations";


const createUserDevice = async (data: UserDeviceRequest) => {
    const userDevice = await db.insert(userDevices).values(data).returning();

    return new UserDevice(userDevice[0]);
}
const getAllUserDevices = async () => {
    const allUserDevices = await db.query.userDevices.findMany({
        where: eq(userDevices.is_on, true),
        with: {
            user: true,
            device: true
        }
    });
    return allUserDevices.map(userDevice => {
        const userFormatted = {
            ...userDevice,
            user: new UserRelation(userDevice.user),
            device: new DeviceRelation(userDevice.device)
        }
        console.log(userFormatted);
        return new UserDeviceWithRelations(userFormatted);
    });
}
const getUserDeviceById = async (id: string) => {
    const userDevice = await db.query.userDevices.findFirst({
        where: and(
            eq(userDevices.id, id),
            eq(userDevices.is_on, true)
        ),
        with: {
            user: true,
            device: true
        }
    });
    if (!userDevice) {
        throw new Error("UserDevice not found");
    }
    const userDeviceFormatted = {
        ...userDevice,
        user: new UserRelation(userDevice.user),
        device: new DeviceRelation(userDevice.device)
    }
    return new UserDeviceWithRelations(userDeviceFormatted);
}
const getDevicesByUserId = async (userId: string) => {
    const devicesUser = await db.query.users.findFirst({
        where: and(
            eq(users.id, userId),
            eq(users.is_on, true)
        ),
        with: {
            userDevices: true
        }
    })
    if (!devicesUser) {
        throw new Error("User not found");
    }

    const devicesUserFormatted = {
        ...devicesUser,
        userDevices: devicesUser.userDevices.map(userDevice => new UserDevice(userDevice))
    }

    return new UserWithDevices(devicesUserFormatted);
}
const deleteUserDevice = async (id: string) => {
    const userDevice = await db.query.userDevices.findFirst({
        where: and(
            eq(userDevices.id, id),
            eq(userDevices.is_on, true)
        ),
        with: {
            user: true,
            device: true
        }
    });
    if (!userDevice) {
        throw new Error("UserDevice not found");
    }
    await db.update(userDevices).set({is_on: false}).where(eq(userDevices.id, id)).execute();
    return new UserDevice(userDevice);
}
const updateUserDevice = async (id: string, data: userdeviceUpdate) => {
    const userDevice = await db.query.userDevices.findFirst({
        where: and(
            eq(userDevices.id, id),
            eq(userDevices.is_on, true)
        )
    })
    if (!userDevice) {
        throw new Error("UserDevice not found");
    }
    const updatedUserDevice = await db.update(userDevices).set(data).where(eq(userDevices.id, id)).returning();
    const userDeviceWithRelations = await db.query.userDevices.findFirst({
        where: and(
            eq(userDevices.id, updatedUserDevice[0].id),
            eq(userDevices.is_on, true)
        ),
        with: {
            user: true,
            device: true
        }
    })
    if (!userDeviceWithRelations) {
        throw new Error("UserDevice not found");
    }
    return new UserDevice(userDeviceWithRelations);
}
export const userDeviceServices = {
    createUserDevice,
    getAllUserDevices,
    getUserDeviceById,
    getDevicesByUserId,
    deleteUserDevice,
    updateUserDevice
}