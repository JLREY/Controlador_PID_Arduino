/*https://www.youtube.com/channel/UCLFBRxt5z4OMn4PbalHlkNQ
 * ELECTRONICA Y LO QUE SE NOS OCURRA
 *  JLRey Cabrera
 */
#define planta A0
#define ref A2
#define control 13

volatile float yd, y, u, error, eant, area, derivada, band;
int Uout;
float coef_fil;

float kp = 2;
float ki = 4;
float kd = 0.2;
float periodo  = 0.001; //segundos

void setup() {
  pinMode(ref, INPUT);
  pinMode(planta, INPUT);
  pinMode(control, OUTPUT);

  Serial.begin(115200);
  ConfigTimer2();
  configFiltro(periodo/10); //Frecuencia de corte del filtro
  y = analogRead (planta) * 5 / 1023;
  yd = analogRead (ref) * 5 / 1023;
  error = 0;
  area = 0;
}

void loop() {
  if (band == 1) {
    eant = derivada;
    error = yd - y;
    area = area + periodo * error; //Area bajo la curva
    derivada = (error + coef_fil * eant) / (1 + coef_fil); //Utilizando el estimador de derivadas por filtro RC

    u = kp * error + ki * area + kd * derivada;
    if (u > 5)//Truncamiento de la señal
      u = 5;
    else if (u < 0)
      u = 0;
    Uout = u * 255 / 5; //Acondicionamiento de la señal de 0 a 255
    band = 0;
  }
  analogWrite(control, Uout);
  //grafica_respuesta();
}

void rutina_int() {
  y = analogRead (planta) * 5 / 1023;
  yd = analogRead (ref) * 5 / 1023;
  band = 1;
}

void configFiltro(float fc) {
  float C = 0.000001;
  float R = 1 / (fc * 2 * 3.1416 * C);
  coef_fil = R * C / periodo;
}

void grafica_respuesta() {
  Serial.print("error= ");
  Serial.print(error);
  Serial.print(",");
  Serial.print("y= ");
  Serial.print(y);
  Serial.print(",");
  Serial.print("yd= ");
  Serial.println(yd);
}
