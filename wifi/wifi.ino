#include <ESP8266WiFi.h>

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
  // WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
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
    delay(1000);
  }
}