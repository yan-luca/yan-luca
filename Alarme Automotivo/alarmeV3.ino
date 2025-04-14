TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;
//_______________________________BLUETOOTH ______________________________________//
#include "BluetoothSerial.h"
String device_name = "yan";
BluetoothSerial SerialBT;
//______________________________Protótipos_______________________________________//

void descerVidros();
void fazerZuada();
void trancar();
void destrancar();
void piscarSetas();
void travasAbrir();
void travasFechar();
void subirVidros();

//_______________________________PINOS__________________________________________//
//PLACA RELE 3V
#define pulsoVidrosSubir 22    // Canal 1  | Sinal subir vidros
#define pulsoVidrosDescer 21   // Canal 2  |
#define buzina 19              // Canal 3  |
#define farol 18               // Canal 4  |
#define setaEsquerda 5         // Canal 5  |
#define setaDireita 17         // Canal 6  |
#define motorTrava1 16         // Canal 7  | Motores Travas Elétricas
#define motorTrava2 4          // Canal 8  |
#define rfid 15
//Placa 4 Reles 3v
 #define inputPartida 0

//PLACA RELE 12V
#define sensorPorta 34         // Canal 1
#define posChave 35            // Canal 2
#define fechadura 32           // Canal 3
#define botaoTrancar 33
#define biometria_in 23         // Sinal de entrada do sensor de biometria



//VARIÁVEIS DE TEMPO
#define tempoTravas 200   
#define tempoVidros 800

#define tempoBuzina 500
#define tempoBuzinaEncontrar 500
#define tempoBuzinaTrancar 100
#define tempoSetas 500

bool zuada = false;
unsigned long tempoInicio = 0;
unsigned long tempoBotao = 0;
bool trancado;
char valorRecebido;
bool estadoTravas = false;
bool carroLigado;
unsigned long x = 0;
unsigned long y = 0;
uint32_t updateCounter = 0;
bool travadoPeloBotao = 0;

void fazerZuada(){
  zuada = !zuada;
}
void trancar(){
  if(!trancado && digitalRead(posChave)==0 && digitalRead(sensorPorta) == 0 ){
    digitalWrite(setaDireita, HIGH);
    digitalWrite(setaEsquerda, HIGH);
    travasFechar();
    subirVidros();

    digitalWrite(farol, HIGH);
    digitalWrite(setaDireita, LOW);
    digitalWrite(setaEsquerda, LOW);
    delay(tempoSetas);
    trancado=1;
    digitalWrite(setaDireita, HIGH);
    digitalWrite(setaEsquerda, HIGH);
  }
}
void trancarComBuzina(){
  digitalWrite(setaDireita, HIGH);
  digitalWrite(setaEsquerda, HIGH);
  travasFechar();
  subirVidros();
  digitalWrite(farol, HIGH);
  digitalWrite(buzina, LOW);
  digitalWrite(setaDireita, LOW);
  digitalWrite(setaEsquerda, LOW);
  delay(tempoBuzinaTrancar);
  digitalWrite(buzina,HIGH);
  delay(tempoSetas);
  digitalWrite(setaDireita, HIGH);
  digitalWrite(setaEsquerda, HIGH);
  delay(tempoSetas);
  trancado=1;
}
void destrancar(){
  trancado=0;
  travasAbrir();
  // digitalWrite(farol, LOW);
  // digitalWrite(setaEsquerda, LOW);
  // digitalWrite(setaDireita, LOW);
  //x = millis();
}
void tempoAberto(){
  //Serial.print("Tempo aberto: ");
  //Serial.println(y);
  if(!carroLigado && y >= 60000) {
    digitalWrite(farol, HIGH);
    digitalWrite(setaEsquerda, HIGH);
    digitalWrite(setaDireita, HIGH);
  }

}
void descerVidros(){
  digitalWrite(pulsoVidrosDescer, LOW);
  if(trancado) destrancar();
  delay(tempoVidros);
  digitalWrite(pulsoVidrosDescer, HIGH);
}
void subirVidros(){
  digitalWrite(pulsoVidrosSubir, LOW);
  delay(tempoVidros);
  digitalWrite(pulsoVidrosSubir, HIGH);
}
void funcao(){
  if(tempoBotao > 0 && tempoBotao<1200 && !trancado){
    Serial.println("Comando Travar");
    trancar();
  }
  else if(tempoBotao>0 && trancado && tempoBotao<1200){
    Serial.println("Comando Destrancar");
    destrancar();
    //descerVidros();
  }
  if(tempoBotao > 1200){
    Serial.println("Comando Descer Vidros");
    descerVidros();
  }
  tempoBotao=0;
  tempoInicio = 0;
}
void leitura_botao(){
  tempoInicio=millis();
  while(digitalRead(biometria_in) == 1){
    Serial.print("Botao Pressionado: " );
    tempoBotao = millis() - tempoInicio;
    Serial.println(tempoBotao);
  }
}
// void leitura_rfid(){
//   if(digitalRead(rfid)==1 && trancado){

//     destrancar();
//     delay(2000);
//   }
//   else if(digitalRead(rfid)==1 && !trancado){
//     trancar();
//     delay(2000); 
//   } 

//}
void disparar(){
  if(trancado &&  (digitalRead(sensorPorta)==1 || digitalRead(posChave)==1 || zuada)){
    
    while(trancado){
      SerialBT.println("ALARME DISPARADO");
      
      digitalWrite(buzina, LOW);
      digitalWrite(setaDireita, LOW);
      digitalWrite(setaEsquerda, HIGH);
      delay(tempoBuzina);
      
      digitalWrite(buzina, HIGH);
      digitalWrite(setaDireita, HIGH);
      digitalWrite(setaEsquerda, LOW);
      delay(tempoBuzina);
      digitalWrite(setaEsquerda, HIGH);
      delay(tempoSetas);
    }
    digitalWrite(setaDireita, HIGH);
    digitalWrite(setaEsquerda, HIGH);
    trancado = 0;
  }
}
void travasAbrir(){
  digitalWrite(motorTrava1, LOW);
  delay(tempoTravas);
  digitalWrite(motorTrava1, HIGH);
  estadoTravas = false;
  travadoPeloBotao = false;
}
void travasFechar(){
  digitalWrite(motorTrava2, LOW);
  delay(tempoTravas);
  digitalWrite(motorTrava2, HIGH);
  estadoTravas = true;
}
void buzinaEncontrar(){
  if(trancado){
    for(int i = 0; i<=10; i++){
      
      if(!trancado){
        digitalWrite(buzina, HIGH);
        digitalWrite(setaDireita, HIGH);
        digitalWrite(setaEsquerda, HIGH);
        break;
      } 

      digitalWrite(buzina, LOW);
      digitalWrite(setaDireita, LOW);
      digitalWrite(setaEsquerda, HIGH);
      delay(tempoBuzinaEncontrar);

      if(!trancado){
        
        digitalWrite(buzina, HIGH);
        digitalWrite(setaDireita, HIGH);
        digitalWrite(setaEsquerda, HIGH);
        break;
      } 

      digitalWrite(buzina, HIGH);
      digitalWrite(setaDireita, HIGH);
      digitalWrite(setaEsquerda, LOW);
      delay(tempoBuzinaEncontrar);
      digitalWrite(setaEsquerda, HIGH);
      
      if(!trancado){
        digitalWrite(buzina, HIGH);
        digitalWrite(setaDireita, HIGH);
        digitalWrite(setaEsquerda, HIGH);
        break;
      } 
      
    }
    digitalWrite(setaEsquerda, HIGH);
    digitalWrite(setaDireita, HIGH);
  }
}
void ligado(){

  if(!trancado && digitalRead(posChave)==1){
    int k = 0;
    
      if(digitalRead(posChave)==1){
        digitalWrite(setaEsquerda, HIGH);
        digitalWrite(setaDireita, HIGH);
        digitalWrite(farol, LOW);
        delay(2000);
        travasFechar();
      }
    while(digitalRead(posChave)==1){
      //tempoAtual = millis();
      //if( (tempoAtual-tempoAnterior > 5000) && k==0){
      // k=1;
      //}
      carroLigado = 1;
      botaoTrava();
      //tempoAnterior = tempoAtual;
    }
    carroLigado = 0;
    travasAbrir();
    digitalWrite(farol, HIGH);
  }
}
void piscarSetas(){
  bool estadoTrancado = trancado;
  for(int i = 0; i<=3; i++){
    
    if(estadoTrancado!=trancado){
      digitalWrite(setaDireita, HIGH);
      digitalWrite(setaEsquerda, HIGH);
      break;
    } 
    digitalWrite(setaDireita, LOW);
    digitalWrite(setaEsquerda, HIGH);
    delay(tempoSetas);
  
    
    if(estadoTrancado!=trancado){
      digitalWrite(setaDireita, HIGH);
      digitalWrite(setaEsquerda, LOW);
      delay(tempoSetas);
      digitalWrite(setaEsquerda, HIGH);
      break;
    } 
    digitalWrite(setaDireita, HIGH);
    digitalWrite(setaEsquerda, LOW);
    delay(tempoSetas);
    
    if(estadoTrancado!=trancado){
      digitalWrite(setaDireita, HIGH);
      digitalWrite(setaEsquerda, LOW);
      delay(tempoSetas);
      digitalWrite(setaEsquerda, HIGH);
      break;
    } 
    digitalWrite(setaEsquerda, HIGH);
    SerialBT.print("Piscada: ");
    SerialBT.println(i);
  }

}
void farolEstado(){
  digitalWrite(farol,!digitalRead(farol));
}
void luzes(){
  digitalWrite(farol,!digitalRead(farol));
  digitalWrite(setaEsquerda,!digitalRead(setaEsquerda));
  digitalWrite(setaDireita,!digitalRead(setaDireita));
}
void partida(){
  SerialBT.println("Partida ligada");
  while(digitalRead(inputPartida)==LOW){
    digitalWrite(2,LOW);
  }
  SerialBT.println("Partida desligada");
  digitalWrite(2,HIGH);
}

void bluetooth(){
  valorRecebido = (char)SerialBT.read();
  
  if(valorRecebido == 'y'){
    SerialBT.println("Função encontrar ativada");
    buzinaEncontrar();
  }
  else if(valorRecebido == 'f') {
    farolEstado();
    SerialBT.println("Farol");
  }
  else if(valorRecebido == 't'){
    trancar();
    SerialBT.println("Trancado");
  }
  else if(valorRecebido == 'v') {
    descerVidros();
    SerialBT.println("Descer vidros");
  }
  else if(valorRecebido == 'a') {
    destrancar();
    SerialBT.println("Destrancado");
  }
  else if(valorRecebido == 's'){
  SerialBT.println("PISCA PISCA");
  piscarSetas();
  }
  else if( valorRecebido == 'l') {
    SerialBT.println("LUZES");
    luzes();
  }
  else if( valorRecebido == 'x'){
    SerialBT.println("Trancar com buzina");
    trancarComBuzina();
  }
  else if( valorRecebido == 'h'){
    SerialBT.println("SubirVidros");
    subirVidros();
  }
  
  delay(20);
}

void botaoTrava(){
  if(!trancado)
    if(digitalRead(botaoTrancar) && estadoTravas) {
          travasAbrir();
          delay(500);
        }
        else if(digitalRead(botaoTrancar) && !estadoTravas) {
          travasFechar(); 
          travadoPeloBotao = true;
          delay(500);
        }
    
}

void bluetoothCode(void *pvParameters){
  Serial.print("Bluetooth running on core ");
  Serial.println(xPortGetCoreID());

  SerialBT.begin(device_name);
  for(;;){
    bluetooth();
  }
}
// void IRAM_ATTR tratarInterrupcao() {
//   if (digitalRead(fechadura) == HIGH) {
//     acionado = true; // Detectou borda de subida
//   } else {
//     desacionado = true; // Detectou borda de descida
//   }
// }
void destrancarComChave(){
  if(digitalRead(fechadura) == 1 && !trancado && !travadoPeloBotao){
    trancar();
     Serial.println("Trancar");
     delay(500);
  }
  if(digitalRead(fechadura) == 0 && trancado){
    destrancar();
    Serial.println("Destrancar");
    delay(500);
  }
}

void outrosCode(void *pvParameters){
  Serial.print("Outros running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
    disparar();
    //controle();
    // if(!carroLigado && y<65000){
    //  tempoAberto();
    //  y= millis() - x;
    // }
    ligado();
   


  }
}
void botaoCode(void *pvParameters){
  for(;;){
    leitura_botao();
    //leitura_rfid();
    funcao();
    destrancarComChave();
    botaoTrava();
  }
}
 
//--------------------------------------------------------------------------------------------//



void setup() {
  //----------------------------------PINOS----------------------------------//
  //Placa reles 3v
  pinMode(pulsoVidrosSubir, OUTPUT);  //Canal 1
  digitalWrite(pulsoVidrosSubir, HIGH);
  
  pinMode(pulsoVidrosDescer, OUTPUT);  //Canal 2
  digitalWrite(pulsoVidrosDescer, HIGH);
  
  pinMode(buzina, OUTPUT);       //Canal 3
  digitalWrite(buzina, HIGH);
  
  pinMode(farol, OUTPUT);        //Canal 4
  digitalWrite(farol, HIGH);
  
  pinMode(setaEsquerda, OUTPUT); //Canal 5
  digitalWrite(setaEsquerda, HIGH);
  
  pinMode(motorTrava1, OUTPUT);  //Canal 6
  digitalWrite(motorTrava1, HIGH);
  
  pinMode(setaDireita, OUTPUT);  //Canal 7
  digitalWrite(setaDireita, HIGH);
  
  pinMode(motorTrava2, OUTPUT);  //Canal 8
  digitalWrite(motorTrava2, HIGH);

  pinMode(inputPartida, INPUT);
  attachInterrupt(inputPartida, partida, FALLING);

  pinMode(rfid,INPUT_PULLDOWN);
  
  pinMode(fechadura, INPUT_PULLDOWN);
  //attachInterrupt(digitalPinToInterrupt(fechadura), tratarInterrupcao, CHANGE);


  //Placa reles 12v
  pinMode(sensorPorta, INPUT_PULLDOWN);  
  pinMode(posChave, INPUT_PULLDOWN);      
  // pinMode(receptor, INPUT_PULLDOWN);
  // pinMode(saidaReles12v, OUTPUT);
  // digitalWrite(saidaReles12v, HIGH);
  
  pinMode(biometria_in, INPUT_PULLDOWN);
  pinMode(botaoTrancar, INPUT_PULLDOWN);
  //-------------------------------------------------------------------------//
  
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    bluetoothCode,    // Task function
    "Task2",      // Name of task
    2000,        // Stack size of task
    NULL,         // parameter of the task
    4,             // priority of the task
    NULL,         // Task handle to keep track of created task
    0             // pin task to core 0
  );
  delay(500);

  xTaskCreatePinnedToCore(
    outrosCode,    // Task function
    "Task3",      // Name of task
    2000,        // Stack size of task
    NULL,         // parameter of the task
    1,             // priority of the task
    NULL,         // Task handle to keep track of created task
    1             // pin task to core 0
  );
  delay(500);

   xTaskCreatePinnedToCore(
    botaoCode,    // Task function
    "Task4",      // Name of task
    2000,        // Stack size of task
    NULL,         // parameter of the task
    1,             // priority of the task
    NULL,         // Task handle to keep track of created task
    1             // pin task to core 0
  );
  delay(500);
}
void loop() {
  //server.handleClient();
}
