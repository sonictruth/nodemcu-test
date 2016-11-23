#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "WiFiManager.h"

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h>

// ADC_MODE(ADC_VCC);

char auth[] = "267fd313cf304419b503c178dcf60aab";

char thingName[] = "AlexThing";

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

BLYNK_WRITE(V1) {
  int a = param.asInt();
  Serial.println(a);
  analogWrite(LED_BUILTIN, a);
}

// Main webserver
/*
MDNSResponder mdns;
ESP8266WebServer server(8080);

void handleRoot() {
  server.send(200, String("text/plain"), "Hello from: " + String(thingName)
  + String(ESP.getVcc()) );
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
*/

void setup() {
  Serial.begin(115200);

  // pinMode(LED_BUILTIN, OUTPUT);
  // ESP.getVcc();

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if(!wifiManager.autoConnect(thingName)) {
    Serial.println("Failed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }

  // Main webserver setup
  // if (mdns.begin(thingName, WiFi.localIP())) {
  //  Serial.println("MDNS responder started");
  // }
  // server.on("/", handleRoot);
  // server.onNotFound(handleNotFound);
  // server.begin();
  // Serial.println("HTTP server started");
  Blynk.config(auth);
  Serial.println("Connected...");

}

void loop() {
  // Main Webserver
  // server.handleClient();
  Blynk.run();
}
