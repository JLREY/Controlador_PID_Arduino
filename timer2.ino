//Más información https://hetpro-store.com/TUTORIALES/arduino-timer/
volatile unsigned int cuenta = 0;
//#define T 300 //Mili Segundos
unsigned  T = periodo * 1000;

void ConfigTimer2() {
  SREG = (SREG & 0b01111111); //Desabilitar interrupciones
  TIMSK2 = TIMSK2 | 0b00000001; //Habilita la interrupcion por desbordamiento
  TCCR2B = 0b00000011; //Configura preescala para que FT2 sea de 250 kHz
  SREG = (SREG & 0b01111111) | 0b10000000; //Habilitar interrupciones //Desabilitar interrupciones
}

ISR(TIMER2_OVF_vect) {
  cuenta++;
  if (cuenta > T) {
    rutina_int();
    cuenta = 0;
  }
}
