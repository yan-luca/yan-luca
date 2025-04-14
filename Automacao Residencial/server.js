const express = require('express');
const mqtt = require('mqtt');
const path = require('path');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
const port = 4000;

// Configuração do MQTT
const mqttBroker = 'mqtt://192.168.1.100:1883';
const mqttClient = mqtt.connect(mqttBroker, {
  clean: true,
  connectTimeout: 4000,
  username: '',
  password: '',
  reconnectPeriod: 1000,
});

// Quando conectar ao broker MQTT
mqttClient.on('connect', () => {
  console.log('Conectado ao MQTT Broker');
  mqttClient.subscribe('home/sala');
  mqttClient.subscribe('home/cozinha');
  mqttClient.subscribe('home/varanda');
  mqttClient.subscribe('home/quarto');
});

// Quando receber uma mensagem do MQTT
mqttClient.on('message', (topic, message) => {
  const status = message.toString();
  console.log(`Mensagem recebida (${topic}): ${status}`);
  
  // Emitir evento para todos os clientes conectados
  io.emit('mqtt-message', { topic, status });
});

// Configurar Socket.IO
io.on('connection', (socket) => {
  console.log('Novo cliente conectado');
  
  // Quando um cliente se conecta, enviar status atual
  // (opcional, depende se você armazena o estado)
});

app.get('/toggle-light', (req, res) => {
  const room = req.query.room;
  const status = req.query.status === 'true' ? '1' : '0';
  
  mqttClient.publish(room, status, (err) => {
    if (err) {
      console.error(`Erro ao publicar no MQTT: ${err}`);
      return res.status(500).send(`Erro ao controlar dispositivo em ${room}`);
    }
    console.log(`Enviado para MQTT: ${room} - ${status}`);
    res.send(`Lâmpada do ${room} agora está ${status}`);
  });
});

app.use(express.static(path.join(__dirname, 'public')));

// Iniciar o servidor web (note que usamos 'server' em vez de 'app')
server.listen(port, () => {
  console.log(`🚀 Servidor rodando em http://localhost:${port}`);
});