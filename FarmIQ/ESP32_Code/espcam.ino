#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "esp_camera.h"

#define WIFI_SSID "realme 12x 5G"
#define WIFI_PASSWORD "qwerty123"

// AI THINKER ESP32-CAM Pin Definition
#define PWDN_GPIO_NUM  32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  0
#define SIOD_GPIO_NUM  26
#define SIOC_GPIO_NUM  27
#define Y9_GPIO_NUM    35
#define Y8_GPIO_NUM    34
#define Y7_GPIO_NUM    39
#define Y6_GPIO_NUM    36
#define Y5_GPIO_NUM    21
#define Y4_GPIO_NUM    19
#define Y3_GPIO_NUM    18
#define Y2_GPIO_NUM    5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM  23
#define PCLK_GPIO_NUM  22

#define LED_GPIO_NUM   4  // Flash LED Pin

AsyncWebServer server(80);

void startCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if (psramFound()) {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.println("Camera initialization failed! Retrying...");
        delay(3000);
        esp_camera_init(&config);  // Retry initialization
    } else {
        Serial.println("Camera initialized successfully!");
    }

    pinMode(LED_GPIO_NUM, OUTPUT);
    digitalWrite(LED_GPIO_NUM, LOW);  // LED off by default
}

void handleImageRequest(AsyncWebServerRequest *request) {
    digitalWrite(LED_GPIO_NUM, HIGH); // LED On (Flash)
    delay(500);  // Brief delay for clear capture

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        digitalWrite(LED_GPIO_NUM, LOW);
        request->send(500, "text/plain", "Camera capture failed");
        return;
    }

    digitalWrite(LED_GPIO_NUM, LOW); // LED Off after capture
    request->send_P(200, "image/jpeg", fb->buf, fb->len);
    esp_camera_fb_return(fb);
}

void setup() {
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.print("Wi-Fi connected with IP: ");
    Serial.println(WiFi.localIP());

    startCamera();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", 
        "<h1>ESP32-CAM Web Server</h1><p>Click <a href='/capture'>here</a> to view the live image.</p>");
    });

    server.on("/capture", HTTP_GET, handleImageRequest);

    server.begin();
}

void loop() {
    // Empty, handled by AsyncWebServer
}
