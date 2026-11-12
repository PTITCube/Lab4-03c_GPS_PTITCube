/**
 * LAB 4 - VÍ DỤ 03c: GIẢI MÃ NMEA THÀNH DỮ LIỆU ĐỊNH VỊ BẰNG THƯ VIỆN PTITCUBE
 * 
 * Mục tiêu:
 * - Sau khi đã quan sát các câu tin NMEA thô ở ví dụ 03a và 03b, ví dụ này sử dụng
 *   thư viện PTITCube (lớp đối tượng PTIT_GPS) để tự động bóc tách và chuyển đổi
 *   luồng byte NMEA phức tạp thành các thông số định vị thực tế mà người dùng hiểu được:
 *   + Tọa độ địa lý: Vĩ độ (Latitude) & Kinh độ (Longitude) dạng số thực (Decimal Degrees).
 *   + Cao độ so với mực nước biển (Altitude) tính bằng mét (m).
 *   + Vận tốc di chuyển (Speed) tính bằng km/h và hướng di chuyển (Course) tính bằng độ (°).
 *   + Số lượng vệ tinh đang bám khóa (Satellites in view/fix).
 *   + Chỉ số phẩm chất hình học sai số mặt bằng (HDOP - Horizontal Dilution of Precision).
 *   + Thời gian chuẩn quốc tế UTC (Ngày, Tháng, Năm, Giờ, Phút, Giây).
 * - Tự động quản lý IC chuyển mạch MUX 74HC4066 giữa GPS và Camera.
 */

#include <Arduino.h>
#include <PTITCube.h>

// Khai báo đối tượng GPS từ thư viện PTITCube
PTIT_GPS myGPS;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n==================================================");
    Serial.println("  PTITCUBE - LAB 4-03c: GIẢI MÃ ĐỊNH VỊ BẰNG PTITCUBE ");
    Serial.println("==================================================");
    Serial.println("[INFO] Khởi tạo module GPS qua thư viện PTITCube...");
    
    // myGPS.init(gpsSelectPin, camSelectPin):
    // Tự động cấu hình UART2 (9600 baud, RX=16, TX=17) và điều khiển MUX 74HC4066
    // (chốt Pin 14 LOW tắt Cam, Pin 12 HIGH mở GPS)
    myGPS.init(12, 14);

    Serial.println("[INFO] Đang lắng nghe luồng NMEA và tự động giải mã...");
    Serial.println("[LƯU Ý] Module GPS cần bắt được ít nhất 4 vệ tinh để khóa tọa độ 3D.");
    Serial.println("        Nếu đang ở trong phòng kín, hãy đặt CubeSat gần cửa sổ.");
    Serial.println("==================================================\n");
}

void loop() {
    // Luôn luôn gọi update() trong vòng lặp loop để nạp từng byte từ UART vào bộ giải mã
    myGPS.update();

    // Định kỳ mỗi 3 giây xuất dữ liệu định vị hoàn chỉnh đã chuyển đổi ra Serial Monitor
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 3000) {
        lastPrint = millis();

        int sats = myGPS.getSatellites();
        float lat = myGPS.getLat();
        float lng = myGPS.getLng();
        bool hasFix = (sats >= 4 && (lat != 0.0 || lng != 0.0));

        Serial.println("--------------------------------------------------");
        Serial.printf("[HỆ THỐNG] Uptime: %lu s | Trạng thái GPS: %s\n",
                      millis() / 1000,
                      hasFix ? "ĐÃ CHỐT TỌA ĐỘ (3D FIX)" : "ĐANG TÌM VỆ TINH (NO FIX)");

        if (!hasFix) {
            Serial.printf("  - Số vệ tinh phát hiện: %d vệ tinh (cần tối thiểu 4 vệ tinh)\n", sats);
            Serial.println("  - Đang quét tín hiệu vệ tinh ngoài không gian... Vui lòng chờ...");
        } else {
            Serial.println(">>> THÔNG TIN ĐỊNH VỊ ĐÃ CHUYỂN ĐỔI (HUMAN-READABLE) <<<");
            Serial.printf("  - Vĩ độ (Latitude)       : %.6f° %s\n", abs(lat), lat >= 0 ? "Bắc (N)" : "Nam (S)");
            Serial.printf("  - Kinh độ (Longitude)    : %.6f° %s\n", abs(lng), lng >= 0 ? "Đông (E)" : "Tây (W)");
            Serial.printf("  - Cao độ (Altitude)      : %.2f m (so với mực nước biển)\n", myGPS.getAltitude());
            Serial.printf("  - Tốc độ di chuyển       : %.2f km/h\n", myGPS.getSpeed());
            Serial.printf("  - Hướng di chuyển        : %.2f°\n", myGPS.getCourse());
            Serial.printf("  - Số vệ tinh bám khóa    : %d vệ tinh\n", sats);
            Serial.printf("  - Sai số mặt bằng (HDOP) : %.2f (%s)\n",
                          myGPS.getHDOP(),
                          myGPS.getHDOP() <= 1.5 ? "Lý tưởng" : (myGPS.getHDOP() <= 2.5 ? "Tốt" : "Trung bình"));
            Serial.printf("  - Thời gian chuẩn UTC    : %s\n", myGPS.getTimeString().c_str());
        }
        Serial.println("--------------------------------------------------");
    }
}
