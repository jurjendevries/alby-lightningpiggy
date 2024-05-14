#define LILYGO_T5_V213

#include <WiFi.h>
#include <GxEPD.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Define the pins for the e-paper display
#define EPD_CS 5
#define EPD_DC 17
#define EPD_RSET 16
#define EPD_BUSY 4

GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RSET);
GxEPD_Class display(io, /*RST=*/ EPD_RSET, /*BUSY=*/ EPD_BUSY);

const char* ssid = "YOUR_SSID_HERE";
const char* password = "YOUR_WIFI_KEY_HERE";
const char* apiToken = "YOUR_ALBY_API_KEY_HERE";

void setup() {
    Serial.begin(115200);
    connectToWiFi();
    display.init();
    display.setRotation(1);
    display.eraseDisplay();
    display.setTextColor(GxEPD_BLACK);
    testDisplayFunctions();
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
}

void testDisplayFunctions() {
    display.setTextSize(2);
    display.setCursor(10, 20);
    display.print("Alby Lightning Piggy");
    delay(1000);

    display.setTextSize(1);
    display.setCursor(10, 50);
    display.print(getBitcoinBalance());
    delay(1000);

    display.setCursor(10, 70);
    display.print(getLNURLpAddress());
    delay(1000);

    displayTransactions(90); // Start from y = 90 pixels
    display.update();
}

String getBitcoinBalance() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("https://api.getalby.com/balance");
        http.addHeader("Authorization", String("Bearer ") + apiToken);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            http.end();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            long balance = doc["balance"];
            const char* unit = doc["unit"];
            return String("Balance: ") + balance + " " + unit;
        } else {
            http.end();
            return "Failed to get balance";
        }
    }
    return "Not connected to WiFi";
}

String getLNURLpAddress() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("https://api.getalby.com/user/value4value");
        http.addHeader("Authorization", String("Bearer ") + apiToken);
        int httpCode = http.GET();

        if (httpCode == 200) {
            String payload = http.getString();
            http.end();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            String lightningAddress = doc["lightning_address"];
            int index = lightningAddress.indexOf('@');
            String username = lightningAddress.substring(0, index);

            return "LNURLp: https://getalby.com/.well-known/lnurlp/" + username;
        } else {
            http.end();
            return "Failed to get LNURLp";
        }
    }
    return "Not connected to WiFi";
}

void displayTransactions(int startY) {
    HTTPClient http;
    http.begin("https://api.getalby.com/invoices/incoming?items=3");
    http.addHeader("Authorization", String("Bearer ") + apiToken);
    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        JsonArray transactions = doc.as<JsonArray>();

        int posY = startY;
        if (transactions.size() == 0) {
            display.setCursor(10, posY);
            display.print("Oh no, ask for your pocket money");
        } else {
            for (JsonObject transaction : transactions) {
                display.setCursor(10, posY);
                int amount = transaction["amount"];
                String comment = transaction["comment"] | "No comment";
                display.print(String(amount) + " sats - " + comment);
                posY += 10;  // Adjusted spacing
            }
        }
    } else {
        display.setCursor(10, startY);
        display.print("Failed to fetch transactions");
    }
    http.end();
}

void loop() {
    // The microcontroller will sleep and wake up every 6 hours to execute the setup again
}
