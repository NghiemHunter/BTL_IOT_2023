#define BLYNK_TEMPLATE_ID "TMPL63fCcjcFX"
#define BLYNK_TEMPLATE_NAME "BTLIOT2023"
#define BLYNK_AUTH_TOKEN "JcZXe4jo7UlA5O3czAi4ZOSpe4Hvkop_"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PubSubClient.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "P916_HH02A";
char wifi_password[] = "quangbale1302@";

const char* mqtt_server = "io.adafruit.com";
const char* mqtt_user = "nghiemhunter1904";
const char* mqtt_password = "aio_doKd02bngC6PA1qhYeKzFulW0lcs";

const char* temperature_feed = "temperature";
const char* humidity_feed = "humidity";
const char* predict_feed = "predict";
const char* suggest_feed = "suggest";

WiFiClient espClient;
PubSubClient client(espClient);

float temperature = 35;
int humidity = 70;
char predict[50];
char suggest[50];

void setup() {
  Serial.begin(115200);

  Blynk.begin(auth, ssid, wifi_password);
  client.setServer(mqtt_server, 1883);
  predictWeather(temperature,humidity,predict,suggest);

}

void loop() {
  // Khởi tạo Blynk
  Blynk.run();

  // Ghi thông tin lên Blynk
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, predict);
  Blynk.virtualWrite(V4, suggest);

  // Ghi thông tin ra Serial
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(predict);
  Serial.println(suggest);
  
  // Kết nối MQTT lên Adafruit io.adafruit.com
  if(!client.connected()) reconnect();
  client.loop();

  // Gửi dữ liệu đến Adafruit IO
  // Tạo chuỗi chủ đề (topic) và dữ liệu nhiệt độ
  String temperatureTopic = String(mqtt_user) + "/feeds/" + temperature_feed ;
  String temperatureData = String(temperature);

  // Gửi dữ liệu nhiệt độ lên Adafruit IO
  client.publish(temperatureTopic.c_str(), temperatureData.c_str());

  // Tạo chuỗi chủ đề (topic) và dữ liệu độ ẩm
  String humidityTopic = String(mqtt_user) + "/feeds/" + humidity_feed ;
  String humidityData = String(humidity);

  // Gửi dữ liệu độ ẩm lên Adafruit IO
  client.publish(humidityTopic.c_str(), humidityData.c_str());

  // Tạo chuỗi chủ đề (topic) và dữ liệu dự đoán
  String predictTopic = String(mqtt_user) + "/feeds/" + predict_feed;
  String predictData = String(predict);

  // Gửi dữ liệu dự đoán lên Adafruit IO
  client.publish(predictTopic.c_str(), predictData.c_str());

  // Tạo chuỗi chủ đề (topic) và dữ liệu gợi ý hành động
  String suggestTopic = String(mqtt_user) + "/feeds/" + suggest_feed;
  String suggestData = String(suggest);

  // Gửi dữ liệu gợi ý lên Adafruit IO
  client.publish(suggestTopic.c_str(), suggestData.c_str());

  temperature = random(-50,101);
  humidity = random(0,101);
  predictWeather(temperature,humidity,predict,suggest);

  delay(5000); // Đợi 1 giây trước khi đọc dữ liệu mới
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe("topic");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retry in 5 seconds");
      delay(5000);
    }
  }
}

// Hàm con dự đoán thời tiết dựa trên nhiệt độ và độ ẩm
void predictWeather(int temperature, int humidity, char* predict, char* suggest) {
    if (temperature > 35 && humidity > 80) {
        strcpy(predict, "Nắng nóng và ẩm");
        strcpy(suggest, "Mang theo ô và nước");
    } else if (temperature > 35 && humidity <= 80) {
        strcpy(predict, "Nắng nóng và khô");
        strcpy(suggest, "Mang theo ô và giữ ẩm cho da");
    } else if (temperature > 21 && temperature <= 35 && humidity > 80) {
        strcpy(predict, "Nắng và ẩm");
        strcpy(suggest, "Đeo kính mắt và mang theo nước");
    } else if (temperature > 21 && temperature <= 35 && humidity <= 80) {
        strcpy(predict, "Bình thường");
        strcpy(suggest, "Có thể không cần mang theo gì đặc biệt");
    } else if (temperature < 21 && humidity < 50) {
        strcpy(predict, "Thời tiết lạnh và khô");
        strcpy(suggest, "Mang theo áo khoác");
    } else if (temperature < 21 && humidity >= 50) {
        strcpy(predict, "Thời tiết lạnh và ẩm");
        strcpy(suggest, "Mang theo áo khoác và giữ ẩm cho da");
    } else {
        strcpy(predict, "Bình thường");
        strcpy(suggest, "Có thể không cần mang theo gì đặc biệt");
    }
}
