/**
 * LAB 4 - VÍ DỤ 03c: GIẢI MÃ NMEA THÀNH DỮ LIỆU ĐỊNH VỊ BẰNG THƯ VIỆN PTITCUBE
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;
PTIT_GPS myGPS;

void setup() {
    cdh.begin(115200);
    myGPS.init(12, 14); // Chân MUX
}

void loop() {
    myGPS.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        lastPrint = millis();

        int sats = myGPS.getSatellites();
        if (sats > 0) {
            cdh.printf("[GPS] %d sats | Lat: %.6f, Lng: %.6f | Alt: %.1fm | Spd: %.1fkm/h | HDOP: %.1f\n",
                       sats, myGPS.getLat(), myGPS.getLng(),
                       myGPS.getAltitude(), myGPS.getSpeed(), myGPS.getHDOP());
        } else {
            cdh.println("[GPS] Đang quét tìm vệ tinh...");
        }
    }
}
