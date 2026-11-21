/**
 * LAB 4 - VÍ DỤ 03c: GIẢI MÃ NMEA THÀNH DỮ LIỆU ĐỊNH VỊ BẰNG THƯ VIỆN PTITCUBE
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;
PTIT_EPS eps;
PTIT_GPS gps;

void setup() {
    cdh.begin();

    // Kích hoạt hạ áp cấp nguồn GPS và ngoại vi qua thư viện EPS
    eps.setBuckState(true);

    gps.init();
}

void loop() {
    gps.update();

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 2000) {
        lastPrint = millis();

        int sats = gps.getSatellites();
        if (sats > 0) {
            cdh.printf("GPS: %d sats | Lat: %.6f, Lng: %.6f | Alt: %.1fm | Spd: %.1fkm/h | HDOP: %.1f\n",
                       sats, gps.getLat(), gps.getLng(),
                       gps.getAltitude(), gps.getSpeed(), gps.getHDOP());
        } else {
            cdh.println("GPS: Đang quét tìm vệ tinh...");
        }
    }
}
