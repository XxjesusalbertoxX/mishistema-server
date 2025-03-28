import bcrypt from "bcrypt";

interface user {
    id: string;
    name: string;
    phone: string;
    email: string;
    password: string;
}


class User {
    public readonly id: string;
    public name: string;
    public phone: string;
    public email: string;
    private readonly password: string;


    constructor(user: user) {
        this.id = user.id;
        this.name = user.name;
        this.phone = user.phone;
        this.email = user.email;
        this.password = user.password;
    }

    public async comparePassword (password: string): Promise<boolean> {
        return await bcrypt.compare(password, this.password);
    }
}

export default User;