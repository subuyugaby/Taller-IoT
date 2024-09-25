#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_PM25AQI.h"   // Librería para el sensor PMSA0031

// El cable de datos está conectado al pin digital 32 de Arduino
#define ONE_WIRE_BUS 32

// Se configura una instancia de OneWire para comunicarse con dispositivos de un solo hilo (OneWire)
OneWire oneWire(ONE_WIRE_BUS);

// Se pasa la referencia de OneWire al sensor de temperatura Dallas
DallasTemperature sensors(&oneWire);

// Funciones para el sensor PMSA0031
Adafruit_PM25AQI particulas = Adafruit_PM25AQI(); // Instancia del sensor de calidad de aire
PM25_AQI_Data data; // Variable para almacenar los datos del sensor de partículas

void setup(void) {
  // Inicia la comunicación serial para propósitos de depuración
  Serial.begin(9600);
  
  // Inicia la biblioteca Dallas Temperature
  sensors.begin();

  /** Configuración del sensor PMSA0031 **/
  Serial.println("");
  Serial.print("Checking PM25: ");
  
  // Intenta conectar con el sensor PMSA0031 a través de I2C
  if (!particulas.begin_I2C()) {  // Si no se encuentra el sensor
    Serial.println("Could not find PM 2.5 sensor!");
    while (1)
      delay(10);  // Espera indefinidamente si no se encuentra el sensor
  } else {
    Serial.println("PM25 found!");  // Confirma que el sensor fue encontrado
  }
}

void loop(void) { 
  // Solicita una nueva lectura de temperatura de todos los dispositivos en el bus
  sensors.requestTemperatures();
  double ds18 = sensors.getTempCByIndex(0);  // Obtiene la temperatura del primer sensor en grados Celsius
  
  // Muestra la temperatura en la consola serial
  Serial.print("Celsius temperature: ");
  Serial.print(sensors.getTempCByIndex(0));  // Muestra la temperatura en Celsius
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));  // Muestra la temperatura en Fahrenheit
  delay(1000);  // Espera 1 segundo antes de la próxima lectura

  // Intenta leer los datos del sensor de partículas
  if (!particulas.read(&data)) {
    Serial.println("Could not read from AQI");  // Si no puede leer del sensor, muestra un mensaje de error
    delay(50);  // Intenta nuevamente después de un breve delay
    return;
  } else {
    // Si la lectura fue exitosa, muestra los valores de partículas PM2.5 y PM10
    Serial.print("Reading from pm2.5: ");
    Serial.print(data.pm25_env);  // Muestra el valor de PM2.5
    Serial.print(" "); 
    Serial.print("Reading from pm10: ");
    Serial.print(data.pm100_env);  // Muestra el valor de PM10
    Serial.print(" "); 
  }
}
