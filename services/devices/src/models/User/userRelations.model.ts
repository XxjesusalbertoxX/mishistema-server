interface userRelation {
    id: string;
    name: string;
    phone: string;
    email: string;
}
export class UserRelation {
    public readonly id: string;
    public name: string;
    public phone: string;
    public email: string;

    constructor(user: userRelation) {
        this.id = user.id;
        this.name = user.name;
        this.phone = user.phone;
        this.email = user.email;
    }
}