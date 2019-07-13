int Sensor = 0;
int sensorAnterior = 0; 

#define PinoSensor1 13
#define PinoSensor2 12
#define PinoSensor3 11
#define PinoSensor4 10
#define PinoSensor5  7

//Correcao necessário quando um motor gira mais rápido que outro, será somando a velocidade
#define correcaoMotorEsquerdo 0
#define correcaoMotorDireito  0
#define VEL_MAX 255
#define VEL_MED 150

#define Ena  5  //pino velocidade motor esquerdo
#define In1 A4  //rotaçao para tras motor esquerdo
#define In2 A3  //rotaçao para frente motor esquerdo
#define In3 A2  //rotaçao para frente motor direito
#define In4 A1  //rotaçao para tras motor direito
#define Enb  6  //pino velocidade motor direito

void setup() {
  //Serial.begin(9600);
  pinMode(PinoSensor1, INPUT);
  pinMode(PinoSensor2, INPUT);
  pinMode(PinoSensor3, INPUT);
  pinMode(PinoSensor4, INPUT);
  pinMode(PinoSensor5, INPUT);

  pinMode(Ena, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(Enb, OUTPUT);

  delay(3000); //tempo para o carro estabilizar na pista, se tiver rotina de calibração ela será aqui
  
  retoEmFrente(VEL_MED); //Inicia a corrida
  sensorAnterior = 0b00100; //indo para frente
}

void loop() {
  Sensor = 0;
  if (!digitalRead(PinoSensor5))
    Sensor += ( 1 << 0 );
  if (!digitalRead(PinoSensor4))
    Sensor += ( 1 << 1 );
  if (!digitalRead(PinoSensor3))
    Sensor += ( 1 << 2 );
  if (!digitalRead(PinoSensor2))
    Sensor += ( 1 << 3 );
  if (!digitalRead(PinoSensor1))
    Sensor += ( 1 << 4 );
  if (( Sensor == 0b00000 ) || ( Sensor == 0b11111 )) {
    Sensor = sensorAnterior;
  }

  /*
    Serial.println( "Sensor :" + String(Sensor,BIN) );
  */
  
  if ( Sensor != sensorAnterior ) { //Só faz alguma ação se a leitura atual for diferente da anterior
    sensorAnterior = Sensor;
    if (Sensor == 0b00100) {
      //Serial.println("RETA");
      retoEmFrente(VEL_MED);
    } else if (Sensor == 0b00010) {
      //Serial.println("curva suave para direita");
      curvaDireita( VEL_MED );
    } else if (Sensor == 0b01000) {
      //Serial.println("curva suave para esquerda");
      curvaEsquerda( VEL_MED );
    } else if (Sensor == 0b00001) {
      //Serial.println("curva fechada para direita");
      curvaDireita(VEL_MAX);
    } else if (Sensor == 0b10000) {
      //Serial.println("curva fechada para esquerda");
      curvaEsquerda( VEL_MAX );
    }
  }
}


void retoEmFrente( byte velocidade ) {
  analogWrite(Ena, velocidade + correcaoMotorEsquerdo );
  analogWrite(Enb, velocidade + correcaoMotorDireito );
  digitalWrite(In1, 0); //rotaçao para tras motor direito
  digitalWrite(In2, 1); //rotaçao para frente motor direito
  digitalWrite(In3, 1); //rotaçao para frente motor esquerdo
  digitalWrite(In4, 0); //rotaçao para tras motor esquerdo
}

void curvaDireita( byte velocidade ) {
  analogWrite(Ena, velocidade + correcaoMotorEsquerdo );
  analogWrite(Enb, velocidade + correcaoMotorDireito );
  digitalWrite(In1, 1); //rotaçao para tras motor direito
  digitalWrite(In2, 0); //rotaçao para frente motor direito
  digitalWrite(In3, 1); //rotaçao para frente motor esquerdo
  digitalWrite(In4, 0); //rotaçao para tras motor esquerdo
}

void curvaEsquerda( byte velocidade ) {
  analogWrite(Ena, velocidade + correcaoMotorEsquerdo );
  analogWrite(Enb, velocidade + correcaoMotorDireito );
  digitalWrite(In1, 0); //rotaçao para tras motor direito
  digitalWrite(In2, 1); //rotaçao para frente motor direito
  digitalWrite(In3, 0); //rotaçao para frente motor esquerdo
  digitalWrite(In4, 1); //rotaçao para tras motor esquerdo
}
