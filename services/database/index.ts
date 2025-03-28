import { drizzle } from 'drizzle-orm/node-postgres';
import { Pool } from 'pg';
import * as schema from './schemas';
import dotenv from 'dotenv';
dotenv.config();

const connectionString = process.env.DATABASE_URL as string;



const pool = new Pool({ connectionString });

export const db = drizzle(pool, { schema });