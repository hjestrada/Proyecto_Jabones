volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinSensor = 2;    //Sensor conectado en el pin 2
float factor_conversion = 7.11; //para convertir de frecuencia a caudal
float volumen = 0;
long dt = 0; //variación de tiempo por cada bucle
long t0 = 0; //millis() del bucle anterior

//------------Salidas--------------------
const int ElectValvula = 6;
const int MotoBomba = 5;
const int LedIndicador = 13;

//--------Botones de Acción--------------

const int botones[] = {10, 9, 8, 7, 11};
const int cantidad[] = {1, 2, 3, 4};

//------Variables Auxiliares--------------
volatile float total = 0;


//---Función que se ejecuta en interrupción---------------
void ContarPulsos ()
{
  NumPulsos++;  //incrementamos la variable de pulsos
}

//---Función para obtener frecuencia de los pulsos--------
int ObtenerFrecuecia()
{
  int frecuencia;
  NumPulsos = 0;   //Ponemos a 0 el número de pulsos
  interrupts();    //Habilitamos las interrupciones
  delay(1000);   //muestra de 1 segundo
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia = NumPulsos; //Hz(pulsos por segundo)
  return frecuencia;
}

void setup()
{

  pinMode(ElectValvula, OUTPUT);
  pinMode(MotoBomba, OUTPUT);
  pinMode(LedIndicador, OUTPUT);


  Serial.begin(9600);
  pinMode(PinSensor, INPUT);

  attachInterrupt(0, ContarPulsos, RISING); //(Interrupción 0(Pin2),función,Flanco de subida)
  Serial.println ("Envie 'r' para restablecer el volumen a 0 Litros");
  t0 = millis();

  for (int i = 0; i < sizeof(botones) / sizeof(botones[0]); i++) {
    pinMode(botones[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(botones[i]), buttonPressed, CHANGE);
  }
 
}



void loop ()
{

buttonPressed();
  if (Serial.available()) {
    if (Serial.read() == 'r')volumen = 0; //restablecemos el volumen si recibimos 'r'
  }
  float frecuencia = ObtenerFrecuecia(); //obtenemos la frecuencia de los pulsos en Hz
  float caudal_L_m = frecuencia / factor_conversion; //calculamos el caudal en L/m
  dt = millis() - t0; //calculamos la variación de tiempo
  t0 = millis();
  volumen = volumen + (caudal_L_m / 60) * (dt / 1000); // volumen(L)=caudal(L/s)*tiempo(s)

  //-----Enviamos por el puerto serie---------------
  Serial.print ("Caudal: ");
  Serial.print (caudal_L_m, 3);
  Serial.print ("L/min\tVolumen: ");
  Serial.print (volumen, 3);
  Serial.println (" L");

  if (volumen < total) {
    Serial.println (" motores encendidos");
    digitalWrite(ElectValvula, HIGH);
    digitalWrite(MotoBomba, HIGH);

  }
  else {
    Serial.println (" motores Apagados");
    digitalWrite(ElectValvula, LOW);
    digitalWrite(MotoBomba, LOW);
  }


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
    
  }
}
