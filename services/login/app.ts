import express from 'express';
import dotenv from 'dotenv';
import authRouter from "./src/routers/auth.router";

dotenv.config();
const app = express();
const port = process.env.API_PORT || 3000;


app.listen(port, () => {
    console.log(`Server is running on port ${port}`);
})

app.use(express.json());

app.use("/auth", authRouter);

