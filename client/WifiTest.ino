#include <SPI.h>
#include <WiFi.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 12

const char* ssid = "Escaperoom";
const char* password = "iwantout";
const uint id = 1;
const uint HB_f = 10000; // 10sec
const byte C_UID[] = {0x3A, 0xC9, 0x6A, 0xCB};

unsigned long TALHB;

const uint port = 9001;
IPAddress gateway;

WiFiClient client;
MFRC522 rfid(SS_PIN, RST_PIN);

void check_HB(){
    if(millis() - TALHB > HB_f){
        client.println(); // send HB
        Serial.println("Sent a heartbeat");
        TALHB = millis();
    }
}

void setup(){
    Serial.begin(115200);
    delay(1000);
    Serial.println("Setting up RFID reader");
    SPI.begin();
    rfid.PCD_Init();

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
    if(rfid.PICC_IsNewCardPresent()){
        if(rfid.PICC_ReadCardSerial()){
            MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
            Serial.print("UID:");
            if(rfid.uid.size == sizeof(C_UID)){
                for (int i = 0; i < rfid.uid.size; i++) {
                    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(rfid.uid.uidByte[i], HEX);
                    if(rfid.uid.uidByte[i] != C_UID[i]){
                        flag = false;
                        break;
                    }
                }
                client.println(flag);
            }
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
        }
    }
    delay(10);
}
