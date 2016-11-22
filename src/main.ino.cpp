# 1 "/tmp/tmpJdqObI"
#include <Arduino.h>
# 1 "/home/alexandru/work/NodeMCU-test/src/main.ino"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "WiFiManager.h"




ADC_MODE(ADC_VCC);
MDNSResponder mdns;
ESP8266WebServer server(8080);

char thingName[] = "AlexThing";
void configModeCallback (WiFiManager *myWiFiManager);
void handleRoot();
void blink();
void handleNotFound();
void setup();
void loop();
#line 16 "/home/alexandru/work/NodeMCU-test/src/main.ino"
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void handleRoot() {
  server.send(200, String("text/plain"), "Hello from: " + String(thingName)
  + String(ESP.getVcc()) );
  blink();
}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  ESP.getVcc();
  Serial.begin(115200);
  Serial.println("Setup");
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if(!wifiManager.autoConnect(thingName)) {
    Serial.println("Failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }

  if (mdns.begin(thingName, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }




  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
  Serial.println("Connected...");

}

void loop() {

  server.handleClient();

}