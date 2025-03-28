import {integer, pgTable, varchar, uuid, timestamp, boolean} from "drizzle-orm/pg-core"
import {relations} from "drizzle-orm";

export const users = pgTable('users', {
    id: uuid().primaryKey().defaultRandom(),
    name: varchar({length: 50}).notNull(),
    phone: varchar({length: 20}).notNull(),
    email: varchar({length: 100}).notNull(),
    password: varchar().notNull(),
    created_at: timestamp().defaultNow(),
    is_on: boolean().default(true).notNull(),
});


export const devices = pgTable('devices', {
    id: uuid().primaryKey().defaultRandom(),
    name: varchar({length: 50}).notNull(),
    description: varchar({length: 100}).notNull(),
    code: varchar({length: 20}).notNull(),
    is_on: boolean().notNull().default(true),
    created_at: timestamp('created_at', { withTimezone: true }).notNull().defaultNow(),
    updated_at: timestamp('updated_at', { withTimezone: true }).notNull().defaultNow(),
    deleted_at: timestamp('deleted_at', { withTimezone: true }),
});

export const userDevices = pgTable('user_devices', {
    id: uuid().primaryKey().defaultRandom(),
    name: varchar({length: 50}).notNull(),
    user_id: uuid().notNull().references(() => users.id),
    device_id: uuid().notNull().references(() => devices.id),
    is_on: boolean().notNull().default(true),
    created_at: timestamp('created_at', { withTimezone: true }).notNull().defaultNow(),
    updated_at: timestamp('updated_at', { withTimezone: true }).notNull().defaultNow(),
    deleted_at: timestamp('deleted_at', { withTimezone: true }),
});

export const places = pgTable('places', {
    id: uuid().primaryKey().defaultRandom(),
    name: varchar({length: 50}).notNull(),
    description: varchar({length: 100}).notNull(),
    is_on: boolean().notNull().default(true),
    user_id: uuid().notNull().references(() => users.id),
    created_at: timestamp('created_at', { withTimezone: true }).notNull().defaultNow(),
    updated_at: timestamp('updated_at', { withTimezone: true }).notNull().defaultNow(),
    deleted_at: timestamp('deleted_at', { withTimezone: true }),
});

export const placeDevices = pgTable('place_devices', {
    id: uuid().primaryKey().defaultRandom(),
    place_id: uuid().notNull().references(() => places.id),
    user_device_id: uuid().notNull().references(() => userDevices.id),
    is_on: boolean().notNull().default(true),
    created_at: timestamp('created_at', { withTimezone: true }).notNull().defaultNow(),
    updated_at: timestamp('updated_at', { withTimezone: true }).notNull().defaultNow(),
    deleted_at: timestamp('deleted_at', { withTimezone: true }),
});

export const userRelations = relations(users, ({ one, many }) => ({
    userDevices: many(userDevices),
    places: many(places),
}));

export const deviceRelations = relations(devices, ({ one, many }) => ({
    userDevices: many(userDevices),
}));

export const placeRelations = relations(places, ({ one, many }) => ({
    placeDevices: many(placeDevices),
}));

export const placeDeviceRelations = relations(placeDevices, ({ one }) => ({
    place: one(places, {
        fields: [placeDevices.place_id],
        references: [places.id],
    }),
    device: one(devices, {
        fields: [placeDevices.user_device_id],
        references: [devices.id],
    }),
}));

export const userDeviceRelations = relations(userDevices, ({ one }) => ({
    user: one(users, {
        fields: [userDevices.user_id],
        references: [users.id],
    }),
    device: one(devices, {
        fields: [userDevices.device_id],
        references: [devices.id],
    }),
}));