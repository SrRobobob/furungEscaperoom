#include <ESP8266WiFi.h>



const int LED = 4; 
const char* ssid = "Escaperoom";
const char* password = "iwantout";
const uint id = 66;
const uint HB_f = 10000; // 10sec
const byte C_UID[] = {0x3A, 0xC9, 0x6A, 0xCB};

unsigned long TALHB;

const uint port = 9001;
IPAddress gateway;

WiFiClient client;

void check_HB(){
    if(millis() - TALHB > HB_f){
        client.println(); // send HB
        Serial.println("Sent a heartbeat");
        TALHB = millis();
    }
}

void setup(){

    Serial.begin(115200);
    pinMode(LED, OUTPUT); //LED pin as output
    delay(1000);

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
    bool flag = false;
    check_HB();
    int received = client.read();
    Serial.println(received);
    delay(100);

    if (received > 0)
    {
      flag = true; 
    }
    

    if (flag)
    {
      digitalWrite(LED, HIGH); 
    }
    else
    {
      digitalWrite(LED, LOW); 
    }
      
    
    
    
    
}
