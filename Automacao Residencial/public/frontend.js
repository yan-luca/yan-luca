// Conectar ao Socket.IO quando a página carregar
document.addEventListener('DOMContentLoaded', function() {
    // Conectar ao Socket.IO
    const socket = io();
  
    // Escutar eventos de mensagem MQTT
    socket.on('mqtt-message', function(data) {
      const { topic, status } = data;
      console.log(`Recebido via WebSocket: ${topic} - ${status}`);
      
      // Extrair o nome do cômodo do tópico (ex: 'home/sala' -> 'sala')
      const room = topic.split('/')[1];
      
      // Atualizar a interface com o novo status
      updateRoomStatus(room, status);
    });
  });
  
  // Função para atualizar o status visual na interface
  function updateRoomStatus(room, status) {
    const statusElement = document.getElementById(`${room}Status`);
    if (statusElement) {
      // Atualize o texto baseado no status
      if (status === '1') {
        statusElement.textContent = 'Ligada';
        statusElement.className = 'font-medium text-green-500';
      } else {
        statusElement.textContent = 'Desligada';
        statusElement.className = 'font-medium text-red-500';
      }
    } else {
      console.warn(`Elemento de status para ${room} não encontrado`);
    }
  }
  
  // Funções existentes
  function toggleLight(room, status) {
    fetch(`/toggle-light?room=${room}&status=${status}`)
      .then(response => response.text())
      .then(data => {
        console.log(data);
        // Não precisamos atualizar a UI aqui, pois o evento MQTT fará isso
      })
      .catch(error => console.error('Erro:', error));
  }
  
  function toggleAllLights(status) {
    toggleLight('home/sala', status);
    toggleLight('home/cozinha', status);
    toggleLight('home/varanda', status);
    toggleLight('home/quarto', status);
  }