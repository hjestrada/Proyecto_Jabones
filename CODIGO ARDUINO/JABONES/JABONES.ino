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

//------Variables de Caudal-----------------------

volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion = 7.11; //para convertir de frecuencia a caudal
float volumen = 0;
long dt = 0; //variación de tiempo por cada bucle
long t0 = 0; //millis() del bucle anterior


//-------Función que se ejecuta en interrupción----------

void ContarPulsos() {

  NumPulsos++;  //incrementamos la variable de pulsos
}

//---Función para obtener frecuencia de los pulsos--------

int ObtenerFrecuecia() {

  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia = NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}



void setup() {
  Serial.begin(9600);

  pinMode(ElectValvula, OUTPUT);
  pinMode(MotoBomba, OUTPUT);
  pinMode(LedIndicador, OUTPUT);
  pinMode(sensorCaudalPin, INPUT);
  attachInterrupt(0, ContarPulsos, RISING); //(Interrupción 0(Pin2),función,Flanco de subida)
  t0 = millis();

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
    //IndicadorFuncion(total / 1000);
    LecturaCaudal();

  }
  volumen = 0; //-----
}

void LecturaCaudal() {

  float frecuencia = ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m = frecuencia / factor_conversion * 1000; //calculamos el caudal en L/m
  dt = millis() - t0; //calculamos la variación de tiempo
  t0 = millis();
  volumen = volumen + (caudal_L_m / 60) * (dt / 1000); // volumen(L)=caudal(L/s)*tiempo(s)

  //-----Enviamos por el puerto serie---------------
  //Serial.print ("Caudal: ");
  //Serial.print (caudal_L_m, 3);
  // Serial.print ("L/min\tVolumen: ");
  Serial.print (volumen, 3);
  Serial.println (" L");

  if (volumen < total) {
    digitalWrite(ElectValvula, HIGH);
    digitalWrite(MotoBomba, HIGH);
    Serial.println ("MotoBomba y Electrovalvula Activada");
  }
  else {
    digitalWrite(ElectValvula, LOW);
    digitalWrite(MotoBomba, LOW);
    Serial.println ("MotoBomba y Electrovalvula apagada");

  }

}

void IndicadorFuncion(int numBlinks) {

  int blinkDelay = 180; // duración de cada parpadeo en milisegundos

  for (int i = 0; i < numBlinks; i++) {  // ciclo para parpadear el LED
    digitalWrite(LedIndicador, HIGH);          // encender el LED
    delay(blinkDelay);                   // esperar un tiempo
    digitalWrite(LedIndicador, LOW);
    delay(blinkDelay);
  }
  digitalWrite(LedIndicador, LOW);
  delay(3000);

}
