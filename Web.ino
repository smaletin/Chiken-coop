/*********
  Руи Сантос
  Более подробно о проекте на: https://randomnerdtutorials.com  
*********/

// Импортируем необходимые библиотеки:
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Вставьте ниже SSID и пароль для своей WiFi-сети:
const char* ssid = "MEIZU-OZCQLA";
//const char* ssid = "Topaz";
const char* password = "";//"08061973";

// Задаем GPIO-контакт, к которому подключен светодиод:
const int ledPin = 8;

// Создаем экземпляр класса «AsyncWebServer»
// под названием «server» и задаем ему номер порта «80»:
AsyncWebServer server(80);

// Меняем заглушку на текущее состояние светодиода:
String processor(const String& var){
  // Создаем переменную для хранения состояния светодиода:
  String ledState;

  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}
 
void setup(){
  // Включаем последовательную коммуникацию (для отладки):
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Инициализируем SPIFFS:
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
               //  "При монтировании SPIFFS произошла ошибка"
    return;
  }

  // Подключаемся к WiFi:
  WiFi.setHostname("Chiken-coop"); 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
               //  "Подключаемся к WiFi..."
  }

  // Печатаем в мониторе порта локальный IP-адрес ESP32:
  Serial.println(WiFi.localIP());

  // URL для корневой страницы веб-сервера:
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // URL для файла «style.css»:
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // URL для переключения GPIO-контакта на «HIGH»:
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // URL для переключения GPIO-контакта на «LOW»:
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Запускаем сервер:
  server.begin();
}
 
void loop(){
  
}
