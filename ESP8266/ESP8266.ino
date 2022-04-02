#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WIFI //
//const char* ssid = "ST-MKT-1";
//const char* pass = "12345678";
const char* ssid = "Blake_ST";
const char* pass = "per1vat!UGK8dzk3qcy";

byte cont = 0;
byte max_intentos = 50;

// Server
//const char* host = "http://10.10.10.6:3000/api/v1/ambiente";
const char* host = "http://192.168.100.218:5000/api/v1/ambiente";

// DHT Sensor //
DHT dht(D1, DHT11);
float temp, hume;

void setup() {
  Serial.begin(9600);
  Serial.println("\n");

  // Inicia WiFi
  pinMode(D8, OUTPUT);
  digitalWrite(D8, LOW);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED and cont < max_intentos) {
    cont++;
    delay(500);
    Serial.print(".");
  }
  
  
  Serial.println("");

  // Parametros de red recibidos
  if (cont < max_intentos) {
    Serial.println("WiFi conectado");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("MacAddress: ");
    Serial.println(WiFi.macAddress());
    digitalWrite(D8, HIGH);    

    // Inicia DHT //
    dht.begin();

    // Inicia Relay
    pinMode(D2, OUTPUT);
    digitalWrite(D2, HIGH);
  }
  else {
    Serial.println("Error de Conexion");
    digitalWrite(D8, LOW);
  }

}

void loop() {

  // Comprobar estado WiFi
  if (WiFi.status()== WL_CONNECTED) {
    // Status WiFi
    digitalWrite(D8, HIGH);

    // DHT
    hume = dht.readHumidity();
    temp = dht.readTemperature();
    Serial.println("Temperatura: " + String(temp) + " C " + " Humedad: " + String(hume) + " %");

    // RELAY
    if (temp >= 20) {
      digitalWrite(D2, LOW);
    }else if (temp < 20) {
      digitalWrite(D2, HIGH);
    }

    // JSON DOC
    String temperatura = String(temp);
    String humedad = String(hume);

    DynamicJsonDocument doc(2048);
    doc["Temperatura"] = temperatura;
    doc["Humedad"] = humedad;

    // JSON serializacion
    String json;
    serializeJson(doc, json);

    // Client POST Json
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, host);
    http.addHeader("Content-Type", "application/json");
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

    int codigo_respuesta = http.POST(json);

    // Respuestas de POST
    if(codigo_respuesta > 0) {
      Serial.println("Codigo HTTP: " + String(codigo_respuesta));
      if(codigo_respuesta == 200) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    http.end();
    
  } else {
    digitalWrite(D8, LOW);
  }
  delay(5000);

}
