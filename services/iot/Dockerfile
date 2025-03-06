FROM node:16-alpine

WORKDIR /app

# Usar usuario root para evitar problemas de permisos
USER root

# Copiar package.json y package-lock.json antes de instalar dependencias
COPY package*.json ./

# Instalar dependencias con permisos de root
RUN npm install


# Copiar el resto de los archivos
COPY . .

# Exponer el puerto 3002
EXPOSE 3002

# Comando para iniciar la aplicación
CMD ["npm", "run", "dev"]
