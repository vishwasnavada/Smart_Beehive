#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
WiFiClient client; // wifi client object
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

const char *ssid     = "Ironman";
const char *password = "@Roomno52";

char ThingSpeakAddress[] = "api.thingspeak.com"; // Thingspeak address
String api_key = "PX0XCY4PV5ACM2ZB";             // Thingspeak API WRITE Key for your channel
const int UpdateInterval = 0.33 * 60 * 1000000;  // e.g. 0.33 * 60 * 1000000; //20-Sec update interval for development tests, to fast for practical purposes and Thingspeak!
//const int UpdateInterval = 10 * 60 * 1000000;  // e.g. 15 * 60 * 1000000; for a 15-Min update interval (15-mins x 60-secs * 1000000uS)
String ModelNo;
int Classification;

void setup() {
  Serial.begin(115200);
  Serial2.begin(460800);
  WiFi.begin(ssid, password);
  Serial.println("Start WiFi");
  while (WiFi.status() != WL_CONNECTED ) {
    Serial.print(".");
    delay(500);
  }



  UpdateThingSpeak("field1=" + String(ModelNo) + "&field2=" + String(Classification)); //Send the data as text
 /* ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");*/
}

void loop() {

}

void UpdateThingSpeak(String DataForUpload) {
  if (Serial2.available())
  {
    // Allocate the JSON document
    // This one must be bigger than for the sender because it must store the strings
    StaticJsonDocument<300> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial2);

    if (err == DeserializationError::Ok)
    {
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)
      Serial.print("ModelNumber = ");
      String ModelNo = doc["ModelNumber"];
      Serial.println(ModelNo);
      Serial.print("Classification = ");
      int Classification = doc["Classification"];
      Serial.println(Classification);
      if (Classification == 1) Serial.println ("Active and Healthy");
      else if (Classification == 2) Serial.println ("No queen alert");
      else if  (Classification == 3) Serial.println ("queen present");
      else  if(Classification == 4) Serial.println ("Queen piping");
      else Serial.println ("Unknown");
    }
  }
  WiFiClient client;
  if (!client.connect(ThingSpeakAddress, 80)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    Serial.println(DataForUpload);
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(DataForUpload.length());
    client.print("\n\n");
    client.print(DataForUpload);
  }
  client.stop();
}
