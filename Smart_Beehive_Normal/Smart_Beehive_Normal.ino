#include <WiFi.h>
#include <ArduinoJson.h>
WiFiClient client; // wifi client object


const char *ssid     = "";
const char *password = "";

char ThingSpeakAddress[] = "api.thingspeak.com"; // Thingspeak address
String api_key = "";             // Thingspeak API WRITE Key for your channel
int ModelNo;
int Classification;
int buf;
void setup() {
  Serial.begin(115200);
  Serial2.begin(460800);
  WiFi.begin(ssid, password);
  Serial.println("Start WiFi");
  while (WiFi.status() != WL_CONNECTED ) {
    Serial.print(".");
    delay(500);
  }




}

void loop() {
  // delay (5000); //Increase delay to send the information to cloud
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
      //   Serial.print("ModelNumber = ");
      ModelNo = doc["ModelNumber"];
      //Serial.println(ModelNo);
      //Serial.print("Classification = ");
      Classification = doc["Classification"];

      if (Classification != buf)//Send the data to cloud only if there is a change of state
      {
        Serial.print("ModelNumber = ");
        Serial.println(ModelNo);
        Serial.print("Classification = ");
        Serial.println(Classification);
        if (Classification == 1) Serial.println ("Active and Healthy");
        else if (Classification == 2) Serial.println ("No queen alert");
        else if  (Classification == 3) Serial.println ("queen present");
        else if (Classification == 4)  Serial.println ("Queen piping");
        buf = Classification;
       String DataForUpload = "field1=" + String(ModelNo) + "&field2=" + String(Classification);
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
        delay(1000);
        client.stop();
      }
    }
  }


}
