import {db} from '../../../database/index';
import {users} from "../../../database/schemas";
import {eq} from "../../../database/node_modules/drizzle-orm";
import {UserRequest} from "../models/User/user.request.model";
import User from "../models/User/user.model";


const createUser = async (data: UserRequest) => {
    const user = await db.insert(users).values(data).returning()

    return new User(user[0]);
};

const getUserByEmail = async (email: string) => {
    const user = await db.query.users.findFirst({
        where: eq(users.email, email),
    })

    if (!user) {
        throw new Error("User not found");
    }

    return new User(user);
};

const getUserById = async (id: string) => {
    const user = await db.query.users.findFirst({
        where: eq(users.id, id),
    })

    if (!user) {
        throw new Error("User not found");
    }

    return new User(user);
}

interface UpdateUser {
    name?: string,
    phone?: string,
    email?: string,
    password?: string,
}

const updateUser = async (id: string, data: UpdateUser) => {
    const user = await db.update(users).set(data).where(eq(users.id, id)).returning();

    if (!user) {
        throw new Error("User not found");
    }


    return new User(user[0]);
}


export const userServices = {
    createUser,
    getUserByEmail,
    updateUser
}

