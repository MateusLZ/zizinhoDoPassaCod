//exemplo base
//referência: https://raw.githubusercontent.com/vinicius0082/esp32-tagoIO/main/exemplos/mpu_esp32_mqtt_tagoIO


//Envio de Dados para tagoIO via MQTT

#include <ArduinoJson.h>
#include "EspMQTTClient.h"

//pinos de entrada e saída
const int pino_botao = 25; //input
const int pino_led = 26; //output
const int pino_sensor2 = 33; //output
const int pino_sensor3 = 32; //output

//variáveis para Json
char json_btn[100];
char json_sensor2[100];
char json_sensor3[100];

//variáveis internas
int valor_btn;
int valor_sensor2;
int valor_sensor3;


//configurações da conexão MQTT
EspMQTTClient client
(
  "FIESC_IOT", //nome da sua rede Wi-Fi
  "C6qnM4ag81", //senha da sua rede Wi-Fi
  "mqtt.tago.io",  // MQTT Broker server ip padrão da tago
  "Token",   // username
  "0bfce698-435f-4cdc-a4a4-5b0854c94824",   // Código do Token
  "TestClient",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

//configuração dos pinos
void setup()
{
  Serial.begin(115200);
  //configurações dos pinos
  pinMode(pino_botao, INPUT); //configura o pino como entrada
  pinMode(pino_sensor2, INPUT); //configura o pino como entrada
  pinMode(pino_sensor3, INPUT); //configura o pino como entrada
  pinMode(pino_led, OUTPUT); //configura o pino como entrada
  digitalWrite(pino_botao, HIGH); //ativa pull-up no pino de entrada
  digitalWrite(pino_sensor2, HIGH); //ativa pull-up no pino de entrada
  digitalWrite(pino_sensor3, HIGH); //ativa pull-up no pino de entrada
}



void leitura_sinais()
{
  valor_btn = digitalRead(pino_botao);
  valor_sensor2 = digitalRead(pino_sensor2);
  valor_sensor3 = digitalRead(pino_sensor3);
}

void converte_json()
{
  StaticJsonDocument<300> sjson_btn;

  sjson_btn["variable"] = "botao";
  sjson_btn["value"] = valor_btn;
  serializeJson(sjson_btn, json_btn);

    StaticJsonDocument<300> sjson_sensor2;

  sjson_sensor2["variable"] = "sensorDois";
  sjson_sensor2["value"] = valor_sensor2;
  serializeJson(sjson_sensor2, json_sensor2);

   StaticJsonDocument<300> sjson_sensor3;

  sjson_sensor3["variable"] = "sensorTres";
  sjson_sensor3["value"] = valor_sensor3;
  serializeJson(sjson_sensor3, json_sensor3);
}

void envia_msg()
{
  client.publish("node/btn", json_btn); // You can activate the retain flag by setting the third parameter to true
  client.publish("node/btn", json_sensor2); // You can activate the retain flag by setting the third parameter to true
  client.publish("node/btn", json_sensor3); // You can activate the retain flag by setting the third parameter to true
}

//loop do programa
void loop()
{ 
  leitura_sinais();
  converte_json();
  envia_msg();

  delay(1000);

  client.loop();
}

void processa_msg(const String payload)
{
  StaticJsonDocument<300> msg;

  DeserializationError err = deserializeJson(msg, payload);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  }
  Serial.print("var:");  
  String var = msg["variable"];
  Serial.println(var);
  if(var == "status")
  {
    Serial.print("value:");
    String val = msg["value"];
    Serial.println(val);
    if(val == "on")
      digitalWrite(pino_led, LOW);
    else
      digitalWrite(pino_led, HIGH);
  }
 
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
   client.subscribe("node/status", [] (const String &payload)  {
   Serial.println(payload);
   processa_msg(payload);
  });
}
