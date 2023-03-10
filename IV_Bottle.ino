#include <WiFiServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

const char* ssid = "Note 4";
const char* password = "onetwothree";

int laser = 20;//define pins
int ldr_1 = 7;
int ldr_2 = 6;
int ldr_3 = 5;
int ldr_4 = 4;
int ldr_5 = 5;
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(laser,OUTPUT);
  pinMode(ldr_1,INPUT);
  pinMode(ldr_2,INPUT);
  pinMode(ldr_3,INPUT);
  pinMode(ldr_4,INPUT);
  pinMode(ldr_5,INPUT);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");;
  Serial.println("WiFi connected");

  //Start the server
  server.begin();
  Serial.println("Server started");

  //Print the IP address
  Serial.print ("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

 void loop(){
  
  //Check if a client has connected
  WiFiClient client = server.available();
  if (!client){
    return;
  }
  //wait untill the client sends some data
  Serial.println("New client");
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');

  if (request.indexOf("/measure") != -1)
  {
    sendlevel();
  }
 }

char level(){
  int L1 = 0, L2 = 0, L3 = 0, L4 = 0, L5 = 0;
  int L_decimal = 0;
    
    digitalWrite(laser,HIGH);
    delay(10);
    L1 = map(analogRead(ldr_1),0,220,0,1);  //map(analog voltage for ldr,mininum adc value,
    L2 = map(analogRead(ldr_2),0,220,0,1);  //adc value when laser detected,0,1)
    L3 = map(analogRead(ldr_3),0,220,0,1);
    L4 = map(analogRead(ldr_4),0,220,0,1);
    L5 = map(analogRead(ldr_5),0,220,0,1);

    L_decimal = (L1*16)+(L2*8)+(L3*4)+(L4*2)+L5;

    if (L_decimal < 16)
    return 100;

     else if (L_decimal >= 16 && L_decimal <= 23)
     return 80;

     else if (L_decimal >= 24 && L_decimal <= 27)
     return 60;

     else if (L_decimal >= 28 && L_decimal <= 29)
     return 40;

     else if (L_decimal >= 30 && L_decimal < 31)
     return 20;

     else
     return 0;  
}

void sendlevel(){
  
  WiFiClient client = server.available();
  if (!client){
    return;
  }

    while(!client.available()){
    delay(1);
  }
  String url = "/measured&LEVEL=" + level();
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(url);
  delay(10);
  
}
