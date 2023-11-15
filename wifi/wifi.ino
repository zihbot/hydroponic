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
  client.setFingerprint("28 15 A0 AF 28 69 27 44 26 18 0D BA 92 F2 61 65 52 98 21 76 74 E0 86 9D 09 7B 07 01 1A EF FC 65 9C 9F 25 AE BC 2B 05 3C 5C B3 EC CD 04 7A 7E 4E CF 33 1A 17 A7 39 45 CC 0C 76 1D C3 5A 04 06 2F 1C BC 39 AB 93 09 E8 E3 50 0F 06 FC AF A0 7F D8 97 18 CF E7 97 F4 E0 28 E2 A6 31 B4 9D FF 7C CB EA C8 BD 4D 06 CF B3 FE 48 03 C0 09 8F B1 0C 00 4C 8A FF F9 DC D9 C4 36 04 2D BA 56 3F 65 ED A4 10 ED C3 3B 14 FD 16 73 05 B0 99 AC 79 CD E2 F4 F4 40 2D E8 DC 00 75 2F 91 61 C7 18 8F A7 92 24 99 3A 19 17 00 43 8F 08 78 FB 0F EB 41 72 62 94 02 E0 BD 5A DE C4 61 5B 19 47 FE C2 65 13 73 EE 2E 72 4E DC C3 93 D8 71 99 08 54 D4 CD 61 25 A2 A4 61 A9 56 AD 6B 48 41 AE 2C F3 C2 B4 A5 2B 73 38 6D 07 FA 89 0A EC 6E 6C C7 1C 89 AF 7E B2 B1 53 52 F5 9A 0A 92 77 9F 02 6A FB B7 71 46 9B D5");
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