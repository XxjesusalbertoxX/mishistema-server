import express from 'express';
import passport from 'passport';
import { login, authenticate } from './services/authService.js';

const app = express();
app.use(express.json());
app.use(passport.initialize());

app.post('/login', login);

app.get('/protected', authenticate, (req, res) => {
  res.json({ message: 'This is a protected route' });
});

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
  console.log(`Auth service running on port ${PORT}`);
});