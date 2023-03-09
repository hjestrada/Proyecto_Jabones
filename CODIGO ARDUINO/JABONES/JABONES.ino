//------------Salidas--------------------
const int ElectValvula = 6;
const int MotoBomba = 5;
const int LedIndicador = 13;

//------------Sensores-------------------
const int sensorCaudalPin = 2;

//--------Botones de Acción--------------
const int botones[] = {10, 9, 8, 7, 11};
const int cantidad[] = {1000, 2000, 3000, 4000};


//------Variables Auxiliares--------------
volatile int total = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ElectValvula, OUTPUT);
  pinMode(MotoBomba, OUTPUT);
  pinMode(LedIndicador, OUTPUT);
  pinMode(sensorCaudalPin, INPUT);


  for (int i = 0; i < sizeof(botones) / sizeof(botones[0]); i++) {
    pinMode(botones[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(botones[i]), buttonPressed, CHANGE);
  }
}

void loop() {

  buttonPressed();

}




void buttonPressed() {
  total = 0;
  for (int i = 0; i < sizeof(botones) / sizeof(botones[0]); i++) {
    if (digitalRead(botones[i]) == LOW) {
      total += cantidad[i];
    }
  }
  if (digitalRead(botones[4]) == LOW) {
    Serial.print("Cantidad Total: ");
    Serial.println(total);
    IndicadorFuncion(total / 1000);

  }
}

void IndicadorFuncion(int numBlinks) {

  int blinkDelay = 150; // duración de cada parpadeo en milisegundos

  for (int i = 0; i < numBlinks; i++) {  // ciclo para parpadear el LED
    digitalWrite(LedIndicador, HIGH);          // encender el LED
    delay(blinkDelay);                   // esperar un tiempo
    digitalWrite(LedIndicador, LOW);
    delay(blinkDelay);
  }
  digitalWrite(LedIndicador, LOW);
  delay(3000);

}
