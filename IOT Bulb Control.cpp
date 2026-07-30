#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "wifi name";        
const char* password = "wifi password#"; 
#define RELAY2 27   

WebServer server(80);
bool relayState = false;  

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>ESP32 Bulb Control</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; background: linear-gradient(135deg,#74ebd5,#ACB6E5); margin:0; padding:0; }";
  html += "h1 { color: #222; margin-top: 20px; }";
  html += ".card { background: white; padding: 25px; margin: 30px auto; width: 90%; max-width: 400px; border-radius: 15px; box-shadow: 0 6px 12px rgba(0,0,0,0.2); }";
  html += ".button { display: inline-block; padding: 15px 40px; margin: 15px; font-size: 20px; font-weight: bold; text-decoration: none; border-radius: 10px; transition: 0.3s; }";
  html += ".on { background-color: #4CAF50; color: white; }";
  html += ".off { background-color: #f44336; color: white; }";
  html += ".button:hover { opacity: 0.8; transform: scale(1.05); }";
  html += ".status { font-size: 22px; margin-top: 15px; font-weight: bold; color: #333; }";
  html += "</style></head><body>";
  html += "<h1>ESP32 IoT Bulb Control</h1>";
  html += "<div class='card'><h2>Relay 2</h2>";
  html += "<a class='button on' href='/relay2on'>ON</a>";
  html += "<a class='button off' href='/relay2off'>OFF</a>";
  html += "<div class='status'>Status: ";
  html += relayState ? "<span style='color:green'>ON</span>" : "<span style='color:red'>OFF</span>";
  html += "</div></div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2, LOW);  

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/relay2on", [](){
    digitalWrite(RELAY2, HIGH);
    relayState = true;
    handleRoot();
  });
  server.on("/relay2off", [](){
    digitalWrite(RELAY2, LOW);
    relayState = false;
    handleRoot();
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
