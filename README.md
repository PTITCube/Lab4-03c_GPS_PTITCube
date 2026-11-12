# Lab4-03c_GPS_PTITCube

**Giải mã Dữ liệu Định vị Vệ tinh (GNSS/GPS) bằng Thư viện PTITCube**

Bài thực hành thuộc Phân hệ Cảm biến & Định vị (Lab 4) trong hệ thống PTITCube.

## 1. Mục tiêu
- Sau khi quan sát bản tin NMEA thô ở ví dụ 03a và 03b, sử dụng thư viện **PTITCube** (`PTIT_GPS`) để giải mã và chuyển đổi luồng byte NMEA sang các thông số định vị thực tế có ý nghĩa:
  - **Tọa độ địa lý:** Vĩ độ (Latitude) và Kinh độ (Longitude) dạng độ thập phân (Decimal Degrees).
  - **Cao độ (Altitude):** Tính bằng mét (m) so với mực nước biển trung bình (WGS84).
  - **Vận tốc (Speed) & Hướng di chuyển (Course):** Tính bằng km/h và độ (°).
  - **Số vệ tinh bám khóa (Satellites in view):** Cần tối thiểu 4 vệ tinh để xác định tọa độ 3D.
  - **Chỉ số sai số mặt bằng (HDOP):** Đánh giá độ tin cậy của vị trí hình học.
  - **Thời gian chuẩn quốc tế (UTC):** Năm-Tháng-Ngày và Giờ:Phút:Giây đồng bộ trực tiếp từ đồng hồ nguyên tử của vệ tinh định vị.
- Tự động điều khiển chân chuyển mạch MUX 74HC4066 (chọn GPS: GPIO 12, ngắt Camera: GPIO 14).

## 2. Phần cứng sử dụng
- **ESP32 NodeMCU OBC**
- **Module định vị GNSS:** Quectel L76X.
- **IC chuyển mạch MUX:** 74HC4066.
- **Cổng giao tiếp:** UART2 (RX2 = GPIO 16, TX2 = GPIO 17, Baudrate: `9600`).
- **Serial Monitor:** USB UART (Baudrate: `115200`).
