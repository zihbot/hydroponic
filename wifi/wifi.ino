#include <HttpClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>

// defines WIFI_SSID, WIFI_PASSWORD, ROOT_URL
#include "config.h"

auto ISRG_ROOT_FINGERPRINT = "55 1F 58 A9 BC B2 A8 50 D0 0C B1 D8 1A 69 20 27 29 08 AC 61 75 5C 8A 6E F8 82 E5 69 2F D5 F6 56 4B B9 B8 73 10 59 D3 21 97 7E E7 4C 71 FB B2 D2 60 AD 39 A8 0B EA 17 21 56 85 F1 50 0E 59 EB CE E0 59 E9 BA C9 15 EF 86 9D 8F 84 80 F6 E4 E9 91 90 DC 17 9B 62 1B 45 F0 66 95 D2 7C 6F C2 EA 3B EF 1F CF CB D6 AE 27 F1 A9 B0 C8 AE FD 7D 7E 9A FA 22 04 EB FF D9 7F EA 91 2B 22 B1 17 0E 8F F2 8A 34 5B 58 D8 FC 01 C9 54 B9 B8 26 CC 8A 88 33 89 4C 2D 84 3C 82 DF EE 96 57 05 BA 2C BB F7 C4 B7 C7 4E 3B 82 BE 31 C8 22 73 73 92 D1 C2 80 A4 39 39 10 33 23 82 4C 3C 9F 86 B2 55 98 1D BE 29 86 8C 22 9B 9E E2 6B 3B 57 3A 82 70 4D DC 09 C7 89 CB 0A 07 4D 6C E8 5D 8E C9 EF CE AB C7 BB B5 2B 4E 45 D6 4A D0 26 CC E5 72 CA 08 6A A5 95 E3 15 A1 F7 A4 ED C9 2C 5F A5 FB FF AC 28 02 2E BE D7 7B BB E3 71 7B 90 16 D3 07 5E 46 53 7C 37 07 42 8C D3 C4 96 9C D5 99 B5 2A E0 95 1A 80 48 AE 4C 39 07 CE CC 47 A4 52 95 2B BA B8 FB AD D2 33 53 7D E5 1D 4D 6D D5 A1 B1 C7 42 6F E6 40 27 35 5C A3 28 B7 07 8D E7 8D 33 90 E7 23 9F FB 50 9C 79 6C 46 D5 B4 15 B3 96 6E 7E 9B 0C 96 3A B8 52 2D 3F D6 5B E1 FB 08 C2 84 FE 24 A8 A3 89 DA AC 6A E1 18 2A B1 A8 43 61 5B D3 1F DC 3B 8D 76 F2 2D E8 8D 75 DF 17 33 6C 3D 53 FB 7B CB 41 5F FF DC A2 D0 61 38 E1 96 B8 AC 5D 8B 37 D7 75 D5 33 C0 99 11 AE 9D 41 C1 72 75 84 BE 02 41 42 5F 67 24 48 94 D1 9B 27 BE 07 3F B9 B8 4F 81 74 51 E1 7A B7 ED 9D 23 E2 BE E0 D5 28 04 13 3C 31 03 9E DD 7A 6C 8F C6 07 18 C6 7F DE 47 8E 3F 28 9E 04 06 CF A5 54 34 77 BD EC 89 9B E9 17 43 DF 5B DB 5F FE 8E 1E 57 A2 CD 40 9D 7E 62 22 DA DE 18 27";

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
  client.setFingerprint(ISRG_ROOT_FINGERPRINT);
  HttpClient https(client);

  auto result = https.get(ROOT_URL, 443, "/");
  Serial.println("Response code: " + String(result));
  Serial.println("Response body: " + https.readString());

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