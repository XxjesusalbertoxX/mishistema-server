import passport from 'passport';
import { Strategy as JwtStrategy, ExtractJwt } from 'passport-jwt';
import jwt from 'jsonwebtoken';
import bcrypt from 'bcryptjs';
import { Request, Response, NextFunction } from 'express';

const users = [
  {
    id: '1',
    email: 'test@example.com',
    password: bcrypt.hashSync('password', 10), // Hashed password
    isAdmin: true,
  },
];


const jwtSecret = process.env.JWT_SECRET || 'your_jwt_secret';

passport.use(
  new JwtStrategy(
    {
      jwtFromRequest: ExtractJwt.fromAuthHeaderAsBearerToken(),
      secretOrKey: jwtSecret,
    },
    (jwtPayload, done) => {
      const user = users.find((user) => user.id === jwtPayload.id);
      if (user) {
        return done(null, user);
      } else {
        return done(null, false);
      }
    }
  )
);

export const generateToken = (user: any) => {
  return jwt.sign({ id: user.id, isAdmin: user.isAdmin }, jwtSecret, {
    expiresIn: '1h',
  });
};

export const authenticate = passport.authenticate('jwt', { session: false });

export const login = (req: Request, res: Response) => {
  const { email, password } = req.body;
  const user = users.find((user) => user.email === email);
  if (user && bcrypt.compareSync(password, user.password)) {
    const token = generateToken(user);
    res.json({ token });
  } else {
    res.status(401).json({ message: 'Invalid email or password' });
  }
};