import express from "express";
import bodyParser from "body-parser";
import admin from "firebase-admin";
import loginRoute from "./routes/login.js";
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';
import { readFileSync } from 'fs';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const serviceAccountPath = join(__dirname, '../examen-6d420-firebase-adminsdk-fbsvc-13aa8488ef.json');
const serviceAccount = JSON.parse(readFileSync(serviceAccountPath, 'utf8'));

admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
});

const app = express();
const port = 3000;

app.use(bodyParser.json());

app.use("/api/auth", loginRoute);

app.listen(port, () => {
  console.log(`Servidor escuchando en el puerto ${port}`);
});