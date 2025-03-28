import {Request, Response} from "express";

import {userDeviceServices} from "../services/userDevice.services";
import {UserDeviceRequest, userdeviceUpdate} from "../models/UserDevice/userDevice.request.model";

const createUserDevice = async (req: Request, res: Response) => {
    try {
        const data:UserDeviceRequest = req.body;
        const userDevice = await userDeviceServices.createUserDevice(data);
        res.status(200).send({msj: 'UserDevice created', dta: {userDevice}});
    } catch (err) {
        if (err instanceof Error) {
            res.status(400).send({err: err.message});
        }
        res.status(400).send({err: 'Error desconocido'});
    }
}

const getAllUserDevices = async (req: Request, res: Response) => {
    try {
        const userDevices = await userDeviceServices.getAllUserDevices();
        res.status(200).send({msj: 'UserDevices found', dta: {userDevices}});
    } catch (err) {
        if (err instanceof Error) {
            res.status(400).send({err: err.message});
        }
        res.status(400).send({err: 'Error desconocido'});
    }
}

const getUserDevices = async (req: Request, res: Response) => {
    try {
        const userId = req.params.userId;
        const userDevices = await userDeviceServices.getDevicesByUserId(userId);
        res.status(200).send({msj: 'UserDevices found', dta: {userDevices}});
    } catch (err) {
        if (err instanceof Error) {
            res.status(400).send({err: err.message});
        }
        res.status(400).send({err: 'Error desconocido'});
    }
}

const getUserDeviceById = async (req: Request, res: Response) => {
    try {
        const id = req.params.id;
        const userDevice = await userDeviceServices.getUserDeviceById(id);
        res.status(200).send({msj: 'UserDevice found', dta: {userDevice}});
    } catch (err) {

        const error = err instanceof Error ? err.message : "Error Desconocido";

        res.status(400).json({err: error});
    }
}

const deleteUserDevice = async (req: Request, res: Response) => {
    try {
        const id = req.params.id;
        await userDeviceServices.deleteUserDevice(id);
        res.status(200).send({msj: 'UserDevice deleted'});
    } catch (err) {
        if (err instanceof Error) {
            res.status(400).send({err: err.message});
        }
        res.status(400).send({err: 'Error desconocido'});
    }
}

const updateUserDevice = async (req: Request, res: Response) => {
    try {
        const id = req.params.id;
        const data:userdeviceUpdate = req.body;
        const userDevice = await userDeviceServices.updateUserDevice(id, data);
        res.status(200).send({msj: 'UserDevice updated', dta: {userDevice}});
    } catch (err) {
        if (err instanceof Error) {
            res.status(400).send({err: err.message});
        }
        res.status(400).send({err: 'Error desconocido'});
    }
}

export const userDeviceController = {
    createUserDevice,
    getAllUserDevices,
    getUserDevices,
    getUserDeviceById,
    deleteUserDevice,
    updateUserDevice
}