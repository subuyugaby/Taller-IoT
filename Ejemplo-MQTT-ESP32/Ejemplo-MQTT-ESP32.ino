/*
 *Programa conexión WiFi y envio de datos a la plataforma FlowFuse con la tarjeta de CO2  
 *Conexión y envio de información publicaran nuestros datos hacia el mqtt broker
 *Programa realizado por Angel Isidro & Oscar Rodas - 18 julio 2024 - Versión 1 
 */

// Libraries used
#include <WiFi.h>
#include <PubSubClient.h>  //Libreria para publicación y recepción de datos.
#include <Wire.h>          //Conexión de dispositivos I2C




// WiFi Credentials
// Modify for assign your credentials
const char *ssid = "galileo";
const char *password = "";


//#include "SparkFun_SCD4x_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD4x
//SCD4x mySensor;


// MQTT MOSQUITTO
const char *user = "monair";
const char *passwd = "MONair2023";

const char *clientID = "";  // Modify to assign to a database

// MQTT Server
const char *mqtt_server = "galiot.galileo.edu";

//  Dashboard name
// Modify to assign to a dashboard and database<<
#define TEAM_NAME "IoT_Roberth"//Cambiar con el codigo de tu estacion
// Helper functions


//  Data variables
double temp = 0;
char msg[50];
char msg1[50];
char cnt[50];
char msg_r[50];
char topic_name[250];

// network variables
WiFiClient espClient;
PubSubClient mqtt_client(espClient);


void setup() {
  Serial.begin(115200);

  //WiFi setup
  setupWiFi();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  //MQTT setup
  setupMQTT();

  Serial.println(F("SCD4x Example"));
  Wire.begin();

}

void loop() {
  //checking MQTT conection
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();

  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(callback);
  delay(5000);//Cambiar frecuencia de envio de datos 

  if (mqtt_client.connect(clientID, user, passwd)) {
    Serial.println("");
    Serial.println("*** Cliente conectado a MQTT Server");

      Serial.print(F("\tTemperature(C):"));
      Serial.print(20.0, 1);

      temp = 20.0;
      String str13(temp);
      str13.toCharArray(msg, 50);
      (mqtt_client.publish(getTopic("temp"), msg));


      Serial.println();
  }
  Serial.print("Datos publicados en MQTT Server: ");

  delay(10000);
}

//This functions connects to WiFi, it indicates with the NeoPixel when is connecting to some network and when finaly is connected
void setupWiFi() {
  //Inicializamos WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  //Waiting for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Displays connection information
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("************** Connection information **************");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    //Indicates connection

  } else {
    Serial.println("WiFi not connected.");
  }
}

// Helper function definitions
void publish(char *topic, char *payload) {
  Serial.println(topic_name);
  mqtt_client.publish(topic_name, payload);
}

//Example: TEAM_NAME/hume
char *getTopic(char *topic) {
  sprintf(topic_name, "%s/%s", TEAM_NAME, topic);
  return topic_name;
}

void reconnect() {
  // Loop until we're reconnected
  int cont = 1;
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    Serial.println("connected");


    if (mqtt_client.connect(clientID, user, passwd)) {
      //mqtt_client.subscribe(getTopic("rgb"));
    } else {

      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      cont += 1;
      if (cont > 150) {
        ESP.restart();
      }
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg_r[i] = (char)payload[i];
  }
  msg_r[length] = 0;
  Serial.print("'");
  Serial.print(msg_r);
  Serial.println("'");
}

//This functions connects to MQTT
void setupMQTT() {

  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(callback);
}
