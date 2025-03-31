import express from 'express';
import dotenv from 'dotenv';
import deviceRouter from "./src/routers/device.router";
import userDeviceRouter from "./src/routers/userDevice.router";

dotenv.config();
const app = express();

app.use(express.json());

app.listen(process.env.API_PORT, () => {
    console.log(`Server is running on port ${process.env.API_PORT}`);
})

app.use("/devices", deviceRouter);
app.use("/user-devices", userDeviceRouter)

export default app;