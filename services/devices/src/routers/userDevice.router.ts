import express from "express";
import {userDeviceController} from "../controllers/userDevice.controller";

const router = express.Router();

router.post('/', userDeviceController.createUserDevice);
router.get('/', userDeviceController.getAllUserDevices);
router.get('/:userId', userDeviceController.getUserDevices);
router.get('/device/:id', userDeviceController.getUserDeviceById);
router.delete('/:id', userDeviceController.deleteUserDevice);
router.put('/:id', userDeviceController.updateUserDevice);

export default router;