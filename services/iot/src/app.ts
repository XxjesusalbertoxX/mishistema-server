import express, { Request, Response, NextFunction, RequestHandler } from 'express';
import amqp from 'amqplib';
import dotenv from 'dotenv';

dotenv.config();

const app = express();
app.use(express.json());

const RABBITMQ_URL = process.env.RABBITMQ_URL || 'amqp://rabbit_amqp';
const QUEUE_NAME = 'motor_configurations';

// Declaramos el manejador de la ruta con tipo RequestHandler y retorno Promise<void>
const configureMotorHandler: RequestHandler = async (
  req: Request,
  res: Response,
  next: NextFunction
): Promise<void> => {
  const { speed, steps } = req.body;

  if (typeof speed !== 'number' || typeof steps !== 'number') {
    res.status(400).json({ error: 'Invalid data format' });
    return;
  }

  try {
    // Conectarse a RabbitMQ
    const connection = await amqp.connect(RABBITMQ_URL);
    const channel = await connection.createChannel();

    // Asegurarse de que la cola exista
    await channel.assertQueue(QUEUE_NAME, { durable: true });

    // Enviar la configuración al Arduino a través de RabbitMQ
    const message = JSON.stringify({ speed, steps });
    channel.sendToQueue(QUEUE_NAME, Buffer.from(message), { persistent: true });

    console.log('Sent motor configuration to RabbitMQ:', message);

    // Ejemplo: responder inmediatamente (puedes implementar la lógica para esperar la respuesta)
    res.json({ message: 'Configuration sent' });
    
    // Cerrar conexión o canal si es necesario
    // await channel.close();
    // await connection.close();
  } catch (error) {
    console.error('Error:', error);
    next(error);
  }
};

app.post('/motor/configure', configureMotorHandler);

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`IoT Service running on port ${PORT}`);
});
