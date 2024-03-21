#include <ESP8266WiFi.h>

const char* ssid = "Escaperoom";
const char* password = "iwantout";
const uint id = 1;
const uint HB_f = 10000; // 10sec

const int buttonPin = 2; 
unsigned long TALHB;

const uint port = 9001;
IPAddress gateway;

WiFiClient client;

void check_HB(){
    if(millis() - TALHB > HB_f){
        client.println(""); // send HB
        Serial.println("Sent a heartbeat");
        TALHB = millis();
    }
}

void setup(){
    Serial.begin(115200);
    // ---------------- PINS ----------- // 
    pinMode(buttonPin, INPUT); 
    
    delay(1000);
    

// ------------ WIFI ------------- // 
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting...");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(250);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connecting to host");
    gateway = WiFi.gatewayIP();
    while (!client.connect(gateway, port))
    {
        Serial.print(".");
        delay(250);
    }
    Serial.println("Connected to host");
    delay(2000);
    Serial.println("Sending whoami packet");
    client.println(id);
    TALHB = millis();

}

void loop(){
    bool flag = true;
    check_HB();
    sendMessage(); 
}

void sendMessage()
{
  // is button pressed
  if (digitalRead(buttonPin) == HIGH)
  {
    Serial.println("Should be sending shite");
    client.println(66);
  }

}

