#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

const char* ssid     = "Airtel_7975123062";
const char* password = "air76408";
char* host = "global-control-nodemcu-server.herokuapp.com";
const int espport= 80;

WebSocketsClient webSocket;
WiFiClient client;
#define USE_SERIAL Serial
String val="";
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
      USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
      
      // send message to server when Connected
      webSocket.sendTXT("Connected to ardunio");
    }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[WSc] get text: %s\n", payload);
      char* str;
      str = strtok((char*)payload,":");
      str = strtok(NULL,":");
      Serial.print("playload is: ");
      Serial.printf(str);
      Serial.print("\n");
      if(atoi(str)==1){
        digitalWrite(12,HIGH);
        webSocket.sendTXT("Arduino : DigitalWrite to HIGH");
      }else if(atoi(str)==0){
        digitalWrite(12,LOW);
        webSocket.sendTXT("Arduino : DigitalWrite to LOW");
      }


      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
        case WStype_PING:
            // pong will be send automatically
            USE_SERIAL.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            USE_SERIAL.printf("[WSc] get pong\n");
            break;
    }

}

void setup(){
    USE_SERIAL.begin(115200);
    Serial.begin(115200);
    pinMode(12, OUTPUT);
    WiFi.begin(ssid, password);
    USE_SERIAL.print("connecting");
    pinMode(D0, OUTPUT);
    while (WiFi.status() != WL_CONNECTED) {
        USE_SERIAL.print(".");
        delay(500);
    }
    USE_SERIAL.println();
    USE_SERIAL.print("connected: ");
    USE_SERIAL.println(WiFi.localIP());
    delay(1000);
    webSocket.begin(host,espport,"/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

void loop() {
    webSocket.loop();
//    USE_SERIAL.print(val);
//    if(val=='1'){
//      digitalWrite(D0,HIGH);
//    }else{
//      digitalWrite(D0,LOW);
//    }
}
