#include "ssd1306.h"
#include "nano_gfx.h"
#include <SoftwareSerial.h>
#include <CDBLEProx.h>

void ble_event(BLE_PROXIMITY_EVENT eventArgs);
SoftwareSerial sw(7,8);
CDBLEProximity ble(&sw,ble_event);

char buf[4];

static void textDemo()
{
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_clearScreen();
    ssd1306_printFixed(24,  8, "Beacon Status", STYLE_NORMAL);
    ssd1306_printFixed(0, 16, "Bold text", STYLE_BOLD);
    ssd1306_printFixed(0, 24, "Italic text", STYLE_ITALIC);
    ssd1306_negativeMode();
    ssd1306_printFixed(0, 32, "Inverted bold", STYLE_BOLD);
    ssd1306_positiveMode();
    delay(3000);
    ssd1306_clearScreen();
    ssd1306_printFixedN(0, 0, "N3", STYLE_NORMAL, FONT_SIZE_8X);
    delay(3000);
}

void printToOled(int lineNo, const char *label, const char *value)
{
    int oledPos;
    char oledStr[24];
    oledStr[0] = '\0';
    switch (lineNo) {
        case 1:
            oledPos = 24;
            break;
        case 2:
            oledPos = 32;
            break;
        case 3:
            oledPos = 40;
            break;
        case 4:
            oledPos = 48;
            break;
    }
    strcat(oledStr, label);
    strcat(oledStr, ": ");
    strcat(oledStr, value);
    ssd1306_printFixed(0, oledPos, oledStr, STYLE_NORMAL);
}

void ble_event(BLE_PROXIMITY_EVENT eventArgs) {
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_LOST) {
        printToOled(1, "-==-", "No Beacon");
        printToOled(2, "----", "");
        printToOled(3, "----", "");
        printToOled(4, "----", "");
        Serial.println("No device");  
        Serial.println("");
    }
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_APPROACH) {
        Serial.println("New device");
        printToOled(1, "-!!-", "Beacon Found");
        Serial.print("UUID: "); Serial.println(eventArgs.device.address);
        Serial.print("MAC: "); Serial.println(eventArgs.device.mac);
        printToOled(2, "MAC ", eventArgs.device.mac.c_str());
        Serial.print("HL : "); Serial.println(eventArgs.device.hilo);
        printToOled(3, "HILO", eventArgs.device.hilo.c_str());
        Serial.print("HI : "); Serial.println(eventArgs.device.hi);
        Serial.print("LO : "); Serial.println(eventArgs.device.lo);
        Serial.print("SIG: "); Serial.println(eventArgs.device.rssi);
        sprintf(buf, "%03i", eventArgs.device.rssi);
        printToOled(4, "RSSI", buf);
        Serial.println("");
    }
    if (eventArgs.eventID==BLE_EVENT_ON_DEVICE_MOVED) {
        Serial.println("Device moved");
        printToOled(1, "-++-", "Beacon Moved");
        Serial.print("UUID: "); Serial.println(eventArgs.device.address);
        Serial.print("MAC: "); Serial.println(eventArgs.device.mac);
        printToOled(2, "MAC ", eventArgs.device.mac.c_str());
        Serial.print("HL : "); Serial.println(eventArgs.device.hilo);
        printToOled(3, "HILO", eventArgs.device.hilo.c_str());
        Serial.print("HI : "); Serial.println(eventArgs.device.hi);
        Serial.print("LO : "); Serial.println(eventArgs.device.lo);
        Serial.print("SIG: "); Serial.println(eventArgs.device.rssi);
        sprintf(buf, "%03i", eventArgs.device.rssi);
        printToOled(4, "RSSI", buf);
        Serial.println("");
    }
}

void setup()
{
    Serial.begin(9600);
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_128x64_i2c_init();
    ssd1306_fillScreen( 0x00 );
    ssd1306_setFixedFont(ssd1306xled_font6x8);
    ssd1306_clearScreen();
    ssd1306_printFixed(22,  8, "Beacon Status", STYLE_NORMAL);
    ble.begin();
}

void loop() {
    ble.update();
}
