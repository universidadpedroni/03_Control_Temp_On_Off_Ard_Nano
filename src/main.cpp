#include <Arduino.h>
#include "config.h"
#include <Streaming.h>
#include <blink.h>

// Librerías del Sensor
#include <DHT.h>

// Librería del display
#include <TM1637Display.h>

void rutinaDeControl(unsigned long interval);
bool controlBangBangSinHisteresis(float setpoint, float temp);
bool controlBangBangConHisteresis(float setpoint, float temp,float emax);
void displayTemp(float temp);

// Sensor
DHT dht(DHT_PIN, DHT_TYPE);

// PIN LED
blink parpadeo(LED_BUILTIN);

// Display
TM1637Display display(DISP_CLK, DISP_DIO);

void setup() {
  delay(2000);
  Serial.begin(BAUDRATE);
  Serial << SEPARADOR << CR;
  Serial << NAME << COMP << __DATE__ << F(", ") << __TIME__ << CR;
  Serial << SEPARADOR << CR << INSTRUCCIONES;
  parpadeo.init();
  pinMode(RELAY_PIN,OUTPUT);
  pinMode(CONTROL_PIN,INPUT_PULLUP);
  dht.begin();
  display.setBrightness(0x0f);
  

  
}

void loop() 
{
  rutinaDeControl(2000);
  digitalRead(CONTROL_PIN)? parpadeo.update(250) : parpadeo.update(500);
  // control sin hist: parp rápido. control con hist. parpadeo lento.
  delay(100);
}


bool controlBangBangSinHisteresis(float setpoint, float temp)
{
  bool accion = false;
  setpoint - temp > 0? accion =  true: accion = false;
  return accion;

}

bool controlBangBangConHisteresis(float setpoint, float temp,float emax)
{
  float e = setpoint - temp;
  static bool accion = false;
  // El controlador solo cambia de estado fuera de los límites de la banda de paso
  if (abs(e) > emax) e >= emax? accion = true: accion = false;
  return accion;



}

// función que devuelve la temperatura
void rutinaDeControl(unsigned long interval)
{
  // TEMPORIZACIÓN
  const float setpoint = 24;
  float temp = 0;
  bool accionDeControl = false;
	static unsigned long previousMillis = 0;        // will store last time LED was updated
	//const long interval = 1000;           // interval at which to blink (milliseconds)
	unsigned long currentMillis = millis();
	static bool estadoPin=false;
	
	if(currentMillis - previousMillis > interval) 
	{
	// TEMPORIZACIÓN


     // Lectura de temperatura
     temp = dht.readTemperature();
     
     // Cálculo de la Ley de control  
     if (digitalRead(CONTROL_PIN))
     {
       accionDeControl = controlBangBangSinHisteresis(setpoint,temp);
     }
     else
     {
       accionDeControl = controlBangBangConHisteresis(setpoint, temp, 1.0);
     }
     
     // Ejecución
     digitalWrite(RELAY_PIN,accionDeControl);
    
     // Extra: Mostrar temperatura en display 
     displayTemp(temp);
  }
    
}

void displayTemp(float temp)
{
  //Serial << _FLOAT(temp,2) << CR;
     if (temp != NAN)
     { 
       display.showNumberDec(temp * 10,false,3,0);
       display.setSegments(C,3,3);
       
     }

}