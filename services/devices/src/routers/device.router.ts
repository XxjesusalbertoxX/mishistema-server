import express from "express";
import {deviceController} from "../controllers/device.controller";

const router = express.Router();

router.get("/all", deviceController.getAllDevices);
router.post("/", deviceController.createDevice);
router.get("/code/:code", deviceController.getDeviceByCode);
router.get("/id/:id", deviceController.getDeviceById);
router.put("/:id", deviceController.updateDevice);
router.delete("/:id", deviceController.deleteDevice);

export default router;