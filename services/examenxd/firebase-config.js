import { initializeApp } from "firebase/app";
import { getAuth, signInWithEmailAndPassword } from "firebase/auth";

const firebaseConfig = {
  apiKey: "AIzaSyDzMm4gw2sStAljnUQD8AaSOWpsTF8e_KI",
  authDomain: "examen-6d420.firebaseapp.com",
  projectId: "examen-6d420",
  storageBucket: "examen-6d420.firebasestorage.app",
  messagingSenderId: "496700110553",
  appId: "1:496700110553:web:d8b5da587459285d02b0c8"
};

const app = initializeApp(firebaseConfig);


const auth = getAuth(app);

const email = "user@example.com";
const password = "password";

signInWithEmailAndPassword(auth, email, password)
  .then((userCredential) => {
    const user = userCredential.user;
    console.log("Usuario autenticado:", user);
  })
  .catch((error) => {
    const errorCode = error.code;
    const errorMessage = error.message;
    console.error(errorCode, errorMessage);
  });
