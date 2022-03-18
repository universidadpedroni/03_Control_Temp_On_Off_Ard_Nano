#include <TM1637Display.h>

// strings
#define NAME F("Control de Temperatura tipo On Off\n")
#define COMP F("Fecha y hora de Comp: ")
#define INSTRUCCIONES F("Cuando el relay se enciende, calentar el sensor\n")
#define SEPARADOR F("------------------------------------")
#define TAB F("\t")
#define CR F("\n")

// velocidad de UART
const long BAUDRATE = 115200;

// configuración del sensor DHT 11
const int DHT_PIN = A5;
#define DHT_TYPE    DHT11     // DHT 11

// Pin Relay
const int RELAY_PIN = A4;

// Pin Selectot de ley de control
const int CONTROL_PIN = 3;

// configuración del display
const int DISP_CLK = A2;
const int DISP_DIO = A3;
const uint8_t C[] = {
  //SEG_B | SEG_A | SEG_F | SEG_G,
  SEG_A | SEG_D | SEG_E | SEG_F
  };

