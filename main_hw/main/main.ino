#include <DHT.h>

// Cấu hình cảm biến DHT
#define DHTPIN 23 // Chân GPIO 4 của ESP32 kết nối với DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // Khởi tạo Serial Monitor
  dht.begin();          // Khởi tạo cảm biến DHT11
}

void loop() {
  // Đọc dữ liệu từ DHT11
  float temperature = dht.readTemperature(); // Đọc nhiệt độ
  float humidity = dht.readHumidity();       // Đọc độ ẩm

  // Kiểm tra dữ liệu hợp lệ
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Không đọc được dữ liệu từ DHT11!");
    return;
  }

  // In dữ liệu lên Serial Monitor
  Serial.print("Nhiệt độ: ");
  Serial.print(temperature);
  Serial.print("°C, Độ ẩm: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000); // Chờ 2 giây trước khi đọc lại dữ liệu
}
