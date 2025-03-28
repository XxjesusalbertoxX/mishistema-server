import {Request, Response} from "express";
import {userServices} from "../services/user.services";
import bcrypt from "bcrypt";
import {UserRequest} from "../models/User/user.request.model";
import UserModel from "../models/User/user.model";
import jwt from "jsonwebtoken";
import dotenv from "dotenv";

const register = async (req: Request, res: Response) => {

    try {
        const data:UserRequest = req.body;

        const hashedPassword = await bcrypt.hash(data.password, 10);

        const userToCreate:UserRequest = {
            ...data,
            password: hashedPassword
        }

        const user = await userServices.createUser(userToCreate);
        res.json({msg: "User created"});
    } catch (err) {
        console.log(err);
        res.status(500).json({err: 'ha ocurrido un error'});
    }


}

const login = async (req: Request, res: Response) => {
    try {
        const {email, password} = req.body;
        const user:UserModel = await userServices.getUserByEmail(email);
        if (!user) {
            res.status(404).json({err: "User not found"});
            return
        }

        const isValidPassword = await user.comparePassword(password);

        if (!isValidPassword) {
            res.status(401).json({err: "Invalid password"});
            return
        }

        const token = jwt.sign({id: user.id}, process.env.JWT_SECRET || 'secret', {
            expiresIn: '1h'
        }
        );

        console.log('Se logeo alguien')

        res.json({msg: "User logged", data: {token}});

    } catch (err) {
        console.log(err);
        res.status(500).json({err: 'ha ocurrido un error'});
    }
}

export const authController = {
    register,
    login
}