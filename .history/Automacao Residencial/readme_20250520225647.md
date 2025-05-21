Interruptor em tela LCD touch com ESP32
![Tela](fotoTela.jpeg)
[Vídeo](https://youtube.com/shorts/fCXj5r3NSYw)

Interface em JavaScript rodando como serviço em um Ubuntu Server a qual envia para um broker MQTT o estado dos botões e recebe via websocket qualquer mudança de estado para alterar o estado dos botões na interface

![Interface](interface.png)
![Serviço](servico.png)
![Broker](broker.png)

Também conectado ao broker, o Home Assistant envia e recebe informações por MQTT, como envio do estado de botões para controlar lâmpadas conectadas ao ESP32 e também controla lâmpadas inteligentes,
![HomeAssistant](homeassistant.png)

O hardware é formado por um microcontrolador ESP32 que recebe sinal de interruptores 12v que passam por optoacopladores. Seus pinos de saida controlam SSRs(Solid State Relays), onde são conectadas lâmpadas em corrente alternada.O ESP32 conecta-se na rede local através da interface Ethernet.
![Hardware](hardware.jpeg)