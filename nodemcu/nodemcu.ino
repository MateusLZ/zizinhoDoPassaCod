// Importa as bibliotecas necessarias
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


// Substitua com as credenciais da sua rede
const char* ssid = "ARDUINO";
const char* password = "12345678";

// Seu endereço de IP estático (será atribuido à placa)
IPAddress local_IP(192, 168, 0, 219);
// O endereço do gateway
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);    //opcional
IPAddress secondaryDNS(8, 8, 4, 4);  //opcional

// const int pinoSaidaDigital = 14;  //17          //(LED da placa - pino 2)
const int pinoSaidaAnalogica = 13;  //5         //10; //GPIO5 -> D1
const int Sensor1 = 14;             //d0 4        //9; GPIO4 -> D2
const int Sensor2 = 16;             //d0 4        //9; GPIO4 -> D2
const int Sensor3 = 18;             //d0 4        //9; GPIO4 -> D2
const int pinoEntradaAnalogica = A0;
int instagram = 0;
String strValorDigital;                // Estado do LED - ON/OFF
const char* PARAM_INPUT_1 = "input1";  //armazena parametro recebido

int valorAnalogico = 0;
int valorDigital = 0;


// Cria um objeto AsyncWebServer na porta 80
AsyncWebServer server(80);

String getValorAnalogico() {
  String valorConvertido;
  if (instagram == 0) {
    if (digitalRead(Sensor1) == 1) {
      valorConvertido = 'A';

    } else {
      valorConvertido = 'a';
    }
    instagram = 1;
  } else {
    if (instagram == 1) {
      if (digitalRead(Sensor2) == 1) {
        valorConvertido = 'B';
      } else {
        valorConvertido = 'b';
      }
      instagram = 2;

    } else {
      if (instagram == 2) {
        if (digitalRead(Sensor3) == 1) {
          valorConvertido = 'C';
        } else {
          valorConvertido = 'c';
        }
        instagram = 0;
      }
    }
    Serial.println(valorConvertido);
  }

  return (valorConvertido);
}

// String getValorDigital() {
//   int valorDigital = digitalRead(pinoEntradaDigital);
//   if(valorDigital == LOW){ // LOW ou HIGH depende da ligação do circuito
//     strValorDigital = "ON";
//   }
//   else{
//     strValorDigital = "OFF";
//   }
//   Serial.println("valor digital lido: " + String(valorDigital) + " " + strValorDigital);
//   return String(strValorDigital);
// }

// void setValorDigital(bool estado) {
//   digitalWrite(pinoSaidaDigital, estado);
//   Serial.println("valor digital escrito: " + String(estado));
// }


void setValorAnalogico(String valorAnalogico) {
  int valor = valorAnalogico.toInt();  //retorna 0 se nao for numero
  if (valor >= 0 && valor < 255) {
    analogWrite(pinoSaidaAnalogica, valor);
    Serial.println("valor analogico escrito: " + valorAnalogico);
  } else {
    Serial.println("valor analogico fora da faixa: 0 a 254");
  }
}


void setup() {
  // Porta Serial para debug
  Serial.begin(115200);
  // pinMode(pinoSaidaDigital, OUTPUT); //saida digital
  // pinMode(pinoSaidaAnalogica, OUTPUT); //saida analogica
  // pinMode(pinoEntradaDigital, INPUT); //entrada digital
  //entrada analógica já vem configurada no pino A0

  // Configura o endereço IP estático
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Falha ao configurar em modo Station (STA)");
  }

  // Conecta na rede Wi-Fi com o SSID e senha (password)
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprime pela serial o endereço de IP e inicia o servidor web
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Rota para comandar a saida digital (Liga / ON)
  // server.on("/saidaDigital/on", HTTP_GET, [](AsyncWebServerRequest *request){
  //   setValorDigital(LOW); // LOW ou HIGH depende da ligação do circuito
  //   request->send_P(200, "text/plain", "ON");
  // });

  // Rota para comandar a saida digital (Desliga / OFF)
  // server.on("/saidaDigital/off", HTTP_GET, [](AsyncWebServerRequest *request){
  //   setValorDigital(HIGH); // LOW ou HIGH depende da ligação do circuito
  //   request->send_P(200, "text/plain", "OFF");
  // });

  // Rota para comandar a saida analogica (PWM) formato: <ESP_IP>/saidaAnalogica?input1=<inputMessage>
  server.on("/saidaAnalogica", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/saidaAnalogica?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();  //valor do paramtro
      //inputParam = PARAM_INPUT_1; //se quiser saber o nome do parametro
      setValorAnalogico(inputMessage);  //comanda o valor
    } else {
      inputMessage = "Nenhuma mensagem enviada";
      inputParam = "none";
    }
    request->send_P(200, "text/plain", inputMessage.c_str());
    Serial.println(inputMessage);
  });

  // // Rota para ler a entrada digital
  // server.on("/entradaDigital", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/plain", temp.c_str());
  // });

  // Rota para ler a entrada analogica
  server.on("/entradaAnalogica", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/plain", getValorAnalogico().c_str());
    // request->send_P(200, "text/plain", getValorAnalogico());
  });

  // Adiciona no cabeçalho para evitar erro de acesso do CORS
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  // Inicia o servidor
  server.begin();
}

void loop() {

}