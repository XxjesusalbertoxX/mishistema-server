import {Request, Response} from "express";
import {deviceServices} from "../services/device.services";
import {DeviceRequest} from "../models/Device/device.request.model";
import {DeviceUpdate} from "../models/Device/device.update";


const createDevice = async (req: Request, res: Response) => {
    try {
        const data:DeviceRequest = req.body;
        const device = await deviceServices.createDevice(data);
        res.json({msj: 'device created successfully', dta: {device}});
    } catch (err) {
        console.log(err);
        res.status(500).json({msj: 'ha ocurrido un error'});
    }
}

const getAllDevices = async (req: Request, res: Response) => {
    try {
        const devices = await deviceServices.getAllDevices();
        res.json({msj: 'devices found', dta: {devices}});
    } catch (err) {
        console.log(err);
        res.status(500).json({msj: 'ha ocurrido un error'});
    }
}

const getDeviceByCode = async (req: Request, res: Response) => {
    try {
        const {code} = req.params;
        const device = await deviceServices.getDeviceByCode(code);
        res.json({msj: 'device found', dta: {device}});
    } catch (err) {
        console.log(err);
        res.status(500).json({msj: 'ha ocurrido un error'});
    }
}

const getDeviceById = async (req: Request, res: Response) => {
    try {
        const {id} = req.params;
        const device = await deviceServices.getDeviceById(id);
        res.json({msj: 'device found', dta: {device}});
    } catch (err) {
        console.log(err);
        res.status(500).json({msj: 'ha ocurrido un error'});
    }
}


const updateDevice = async (req: Request, res: Response) => {
    try {
        const {id} = req.params;
        const data:DeviceUpdate = req.body;
        const device = await deviceServices.updateDevice(id, data);
        res.json({msj: 'device updated', dta: {device}});
    } catch (err) {
        console.log(err);
        res.status(500).json({msj: 'ha ocurrido un error'});
    }
}

const deleteDevice = async (req: Request, res: Response) => {
    try {
        const {id} = req.params;
        await deviceServices.deleteDevice(id);
        res.json({msj: 'device deleted'});
    } catch (err) {
        console.log(err);
        res.status(500).json({msj: 'ha ocurrido un error'});
    }
}

export const deviceController = {
    createDevice,
    getAllDevices,
    getDeviceByCode,
    getDeviceById,
    updateDevice,
    deleteDevice
}