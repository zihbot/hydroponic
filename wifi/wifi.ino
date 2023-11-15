#include <HttpClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>

// defines WIFI_SSID, WIFI_PASSWORD, ROOT_URL
#include "config.h"


void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);

  connect();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();
  //client.setFingerprint("c42c2e6d5dc7d65b2bde606a58498f2af8e2b09fa3d9324126eb8d1cb33d3bb0");
  //client.allowSelfSignedCerts();
  HttpClient https(client);

  auto result = https.get(ROOT_URL, 443, "/");
  Serial.println("Response code: " + String(result));
  Serial.println("Response body: " + https.readString());
  /*client.println("GET / HTTP/1.1");
  client.println("Host: " + String(ROOT_URL));
  client.println("Connection: close");
  client.println();*/

  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }
  delay(5000);
}

void connect() {
  digitalWrite(BUILTIN_LED, HIGH);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");
  auto ledState = LOW;
  while (WiFi.status() == WL_DISCONNECTED) {
    digitalWrite(BUILTIN_LED, ledState);
    ledState = (ledState == LOW) ? HIGH : LOW;
    delay(500);
    Serial.print(".");
  }
  digitalWrite(BUILTIN_LED, HIGH);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect.");
    digitalWrite(BUILTIN_LED, LOW);
    delay(2000);
    digitalWrite(BUILTIN_LED, HIGH);
    return;
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void scanNetworks() {
  auto nt = WiFi.scanNetworks();
  Serial.println("");
  Serial.print("Number of networks found: " + String(nt));

  for (int i = 0; i < nt; i++) {
    Serial.println(WiFi.SSID(i));
  }
}

void onNewClient(WiFiClient client) {
  Serial.println("New Client.");
  String currentLine = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html><html>");
          client.println(
              "<head><meta name=\"viewport\" content=\"width=device-width, "
              "initial-scale=1\">");
          client.println("<link rel=\"icon\" href=\"data:,\">");
          client.println("<body><h1>ESP32 Web Server</h1>");
          client.println("</body></html>");
          client.println();
        } else {
          Serial.println(currentLine);
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}