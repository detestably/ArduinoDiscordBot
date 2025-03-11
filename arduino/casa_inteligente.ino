#include <ESP8266WiFi.h> // Ou #include <WiFi.h> para ESP32
#include <ESP8266WebServer.h> // Ou #include <WebServer.h> para ESP32
#include <ArduinoJson.h>

const char* ssid = "SUA_REDE_WIFI";
const char* password = "SENHA_DA_REDE";

ESP8266WebServer server(80); // Ou WebServer server(80); para ESP32

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }

    Serial.println("Conectado ao WiFi");
    Serial.println(WiFi.localIP());

    server.on("/comando", HTTP_POST, handleCommand);
    server.begin();
}

void loop() {
    server.handleClient();
}

void handleCommand() {
    if (server.hasArg("plain") == false) {
        server.send(400, "text/plain", "Corpo da requisição inválido");
        return;
    }

    String body = server.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);

    String command = doc["command"];
    String action = doc["action"];

    String response = "Comando recebido: " + command + " " + action;

    if (command == "ligar") {
        if (action == "luz da sala") {
            digitalWrite(D1, HIGH); // Exemplo: acender um LED conectado ao pino D1
            response = "Luz da sala ligada.";
        } else if (action == "luz do quarto") {
            digitalWrite(D2, HIGH); // Exemplo: acender um LED conectado ao pino D2
            response = "Luz do quarto ligada.";
        }
    } else if (command == "desligar") {
        if (action == "luz da sala") {
            digitalWrite(D1, LOW); // Exemplo: apagar um LED conectado ao pino D1
            response = "Luz da sala desligada.";
        } else if (action == "luz do quarto") {
            digitalWrite(D2, LOW); // Exemplo: apagar um LED conectado ao pino D2
            response = "Luz do quarto desligada.";
        }
    } else if (command == "ajustar") {
        if (action.startsWith("luz da sala para")) {
            int intensity = action.substring(17).toInt();
            analogWrite(D1, intensity); // Exemplo: ajustar intensidade de um LED conectado ao pino D1
            response = "Intensidade da luz da sala ajustada para " + String(intensity) + "%.";
        }
    } else if (command == "status") {
        if (action == "luz da sala") {
            bool status = digitalRead(D1);
            response = "Luz da sala está " + String(status ? "ligada" : "desligada") + ".";
        } else if (action == "luz do quarto") {
            bool status = digitalRead(D2);
            response = "Luz do quarto está " + String(status ? "ligada" : "desligada") + ".";
        }
    }

    server.send(200, "text/plain", response);
}
