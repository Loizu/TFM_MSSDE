#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ADAMO-BD70";
const char* password = "3XO73ZHOH76VQW";

const char* serverName = "http://192.168.1.102/write_data.php?";

// GPIO where the DS18B20 is connected to.
const int oneWireBus = 25;

// GPIO where soil moisture sensors are connected to.
const int humiditySensorPin1 = 32; // nº13
const int humiditySensorPin2 = 33; // nº5
const int humiditySensorPin3 = 34; // nº4
const int humiditySensorPin4 = 35; // nº15

// Variable that stores the number of devices connected to the microcontroller (connected and detected).
uint8_t deviceCount;

// Variables where all connected DS18B20 sensors' addresses are stored.
uint8_t address0[8], address1[8], address2[8], address3[8], address4[8];

// These are the addresses corresponding to each sensor connected to this ESP32.
// They will be used to find the index of each sensor on the bus.
uint8_t sensorN9_addr[8] = {40, 152, 234, 58, 11, 00, 00, 251};
uint8_t sensorN15_addr[8] = {40, 117, 211, 02, 00, 00, 128, 246};
uint8_t sensorN10_addr[8] = {40, 119, 142, 60, 11, 00, 00, 241};
uint8_t sensorN12_addr[8] = {40, 198, 175, 59, 11, 00, 00, 114};


// These variables will store the index given to each sensor while the setup() is performed.
uint8_t sensorN1_index = 0, sensorN15_index = 0, sensorN12_index = 0, sensorN10_index = 0, sensorN9_index = 0;

// Setup a oneWire instance to communicate with any OneWire devices.
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor.
DallasTemperature sensors(&oneWire);

// Variable that stores the time (in seconds) since initialization.
unsigned int currentTime = 0;

void setSensorIndexes() {
  uint8_t sensor0_addr = sensors.getAddress(address0, 0);
  uint8_t sensor1_addr = sensors.getAddress(address1, 1);
  uint8_t sensor2_addr = sensors.getAddress(address2, 2);
  uint8_t sensor3_addr = sensors.getAddress(address3, 3);
  uint8_t sensor4_addr = sensors.getAddress(address4, 4);

  bool has_sensor1_passed = false, has_sensor15_passed = false, has_sensor12_passed = false, has_sensor10_passed = false, has_sensor9_passed = false;
  bool has_sensor1_passed1 = false, has_sensor15_passed1 = false, has_sensor12_passed1 = false, has_sensor10_passed1 = false, has_sensor9_passed1 = false;
  bool has_sensor1_passed2 = false, has_sensor15_passed2 = false, has_sensor12_passed2 = false, has_sensor10_passed2 = false, has_sensor9_passed2 = false;

  for (int i = 0; i < 8; i++) {
    if (address0[i] != sensorN1_addr[i]) {
      sensorN1_index = 1;
      has_sensor1_passed = true;
    }
    if (address0[i] != sensorN15_addr[i]) {
      sensorN15_index = 1;
      has_sensor15_passed = true;
    }
    if (address0[i] != sensorN12_addr[i]) {
      sensorN12_index = 1;
      has_sensor12_passed = true;
    }
    if (address0[i] != sensorN10_addr[i]) {
      sensorN10_index = 1;
      has_sensor10_passed = true;
    }
    if (address0[i] != sensorN9_addr[i]) {
      sensorN9_index = 1;
      has_sensor9_passed = true;
    }
  }
  for (int i = 0; i < 8; i++) {
    if (address1[i] != sensorN1_addr[i] && has_sensor1_passed) {
      sensorN1_index = 2;
      has_sensor1_passed1 = true;
    }
    if (address1[i] != sensorN15_addr[i] && has_sensor15_passed) {
      sensorN15_index = 2;
      has_sensor15_passed1 = true;
    }
    if (address1[i] != sensorN12_addr[i] && has_sensor12_passed) {
      sensorN12_index = 2;
      has_sensor12_passed1= true;
    }
    if (address1[i] != sensorN10_addr[i] && has_sensor10_passed) {
      sensorN10_index = 2;
      has_sensor10_passed1 = true;
    }
    if (address1[i] != sensorN9_addr[i] && has_sensor9_passed) {
      sensorN9_index = 2;
      has_sensor9_passed1 = true;
    }
  }
  for (int i = 0; i < 8; i++) {
    if (address2[i] != sensorN1_addr[i] && has_sensor1_passed1) {
      sensorN1_index = 3;
      has_sensor1_passed2 = true;
    }
    if (address2[i] != sensorN15_addr[i] && has_sensor15_passed1) {
      sensorN15_index = 3;
      has_sensor15_passed2 = true;
    }
    if (address2[i] != sensorN12_addr[i] && has_sensor12_passed1) {
      sensorN12_index = 3;
      has_sensor12_passed2= true;
    }
    if (address2[i] != sensorN10_addr[i] && has_sensor10_passed1) {
      sensorN10_index = 3;
      has_sensor10_passed2 = true;
    }
    if (address2[i] != sensorN9_addr[i] && has_sensor9_passed1) {
      sensorN9_index = 3;
      has_sensor9_passed2 = true;
    }
  }
  for (int i = 0; i < 8; i++) {
    if (address3[i] != sensorN1_addr[i] && has_sensor1_passed2) {
      sensorN1_index = 4;
    }
    if (address3[i] != sensorN15_addr[i] && has_sensor15_passed2) {
      sensorN15_index = 4;
    }
    if (address3[i] != sensorN12_addr[i] && has_sensor12_passed2) {
      sensorN12_index = 4;
    }
    if (address3[i] != sensorN10_addr[i] && has_sensor10_passed2) {
      sensorN10_index = 4;
    }
    if (address3[i] != sensorN9_addr[i] && has_sensor9_passed2) {
      sensorN9_index = 4;
    }
  }
}


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Start the DS18B20 sensor
  sensors.begin();

  deviceCount = sensors.getDeviceCount();
  Serial.print("Nº of devices on MC1: ");
  Serial.println(deviceCount);
  if(deviceCount > 0){
      setSensorIndexes();
      pinMode(humiditySensorPin1, INPUT);
      pinMode(humiditySensorPin2, INPUT);
      pinMode(humiditySensorPin3, INPUT);
      pinMode(humiditySensorPin4, INPUT);
  }
  else{
    Serial.println("There were not enough detected devices, exiting program...");
  }
}

void loop() {
  if(deviceCount > 0 && WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    sensors.requestTemperatures();
    double temperature1 = sensors.getTempCByIndex(sensorN15_index);
    double temperature2 = sensors.getTempCByIndex(sensorN12_index);
    double temperature3 = sensors.getTempCByIndex(sensorN10_index);
    double temperature4 = sensors.getTempCByIndex(sensorN9_index);
    double temperature5 = sensors.getTempCByIndex(sensorN1_index);
    
    double humidity2 = map(analogRead(humiditySensorPin1), 4095, 0, 0, 100);
    double humidity3 = map(analogRead(humiditySensorPin2), 4095, 0, 0, 100);
    double humidity4 = map(analogRead(humiditySensorPin3), 4095, 0, 0, 100);
    double humidity5 = map(analogRead(humiditySensorPin4), 4095, 0, 0, 100);

    String var1 = "&board_id=" + String(1);
    String var2 = "&time_s=" + String(currentTime);
    String var3 = "sensor_type=" + String(0); // 0 means it is a temperature sensor (1 means it's a soil moisture sensor).
    String var4 = "&sensor_id=" + String(15);
    String var5 = "&value=" + String(temperature1);

    String httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
  
    http.begin(serverName + httpRequestData);
    int httpResponseCode = http.GET();
    http.end();

    var4 = "&sensor_id=" + String(12);
    var5 = "&value=" + String(temperature2);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();

    var4 = "&sensor_id=" + String(10);
    var5 = "&value=" + String(temperature3);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();

    var4 = "&sensor_id=" + String(9);
    var5 = "&value=" + String(temperature4);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();

    var3 = "&sensor_type=" + String(1); // 1 -> Soil moisture sensor

    var4 = "&sensor_id=" + String(13);
    var5 = "&value=" + String(humidity2);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();

    var4 = "&sensor_id=" + String(5);
    var5 = "&value=" + String(humidity3);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();
    
    var4 = "&sensor_id=" + String(4);
    var5 = "&value=" + String(humidity4);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();

    var4 = "&sensor_id=" + String(15);
    var5 = "&value=" + String(humidity5);
    httpRequestData = var1 + var2 + var3 + var4 + var5 + "";
    http.begin(serverName + httpRequestData);
    httpResponseCode = http.GET();
    http.end();

    
    currentTime += 2;
    delay(2000);
  }
}
