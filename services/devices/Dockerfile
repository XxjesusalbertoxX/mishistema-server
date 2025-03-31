# Usa Node.js como base
FROM node:18

# Establece el directorio de trabajo dentro del contenedor
WORKDIR /app

# Copia el package.json y package-lock.json para instalar dependencias
COPY package*.json ./

# Instala dependencias del microservicio
RUN npm install

# Expone el puerto para el servicio
EXPOSE 3001

# Inicia el servicio
CMD ["npm", "run", "start"]
