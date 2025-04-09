import express from "express";
import axios from "axios";
import admin from "firebase-admin";

const router = express.Router();

const FIREBASE_API_KEY = process.env.FIREBASE_API_KEY || "AIzaSyDzMm4gw2sStAljnUQD8AaSOWpsTF8e_KI";

router.post("/login", async (req, res) => {
  const { email, password } = req.body;
  
  if (!email || !password) {
    return res.status(400).json({ message: "El email y la contraseña son obligatorios" });
  }
  
  const isValidEmail = (email) => {
    const emailPattern = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    return emailPattern.test(email);
  };
  
  if (!isValidEmail(email)) {
    return res.status(400).json({ message: "Formato de email inválido" });
  }

  try {
    const response = await axios.post(
      `https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=${FIREBASE_API_KEY}`,
      {
        email,
        password,
        returnSecureToken: true,
      },
      {
        headers: { "Content-Type": "application/json" },
      }
    );
    
    const { idToken, refreshToken, expiresIn } = response.data;

    return res.status(200).json({
      message: "Login exitoso",
      token: idToken,
      refreshToken,
      expiresIn,
    });
  } catch (error) {
    console.error("Error en autenticación:", error.response ? error.response.data : error.message);
    return res.status(401).json({
      message: "Error de autenticación, credenciales inválidas",
      error: error.response ? error.response.data : error.message,
    });
  }
});

router.post("/register", async (req, res) => {
  const { email, password } = req.body;
  
  if (!email || !password) {
    return res.status(400).json({ message: "El email y la contraseña son obligatorios" });
  }
  
  const isValidEmail = (email) => {
    const emailPattern = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    return emailPattern.test(email);
  };

  if (!isValidEmail(email)) {
    return res.status(400).json({ message: "Formato de email inválido" });
  }

  try {
    let userRecord;
    try {
      userRecord = await admin.auth().createUser({ email, password });
    } catch (error) {
      if (error.code === 'auth/email-already-exists') {
        return res.status(400).json({ message: 'Correo ya existente' });
      } else {
        throw error; 
      }
    }

    return res.status(201).json({
      message: "Usuario registrado exitosamente",
      user: userRecord,
    });
  } catch (error) {
    console.error("Error al registrar:", error);
    return res.status(500).json({
      message: "Error al registrar usuario",
      error: {
        code: error.code,
        message: error.message,
        full: error,
      },
    });
  }
});

export default router;
