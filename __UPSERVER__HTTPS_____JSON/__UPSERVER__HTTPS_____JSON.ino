#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
const String host = "airsniffer.org";
  const int httpsPort = 443;
  String url = "/api/pollutantvalues";
  WiFiClientSecure client;
    
 
void setup() {
  Serial.begin(115200);                            //Serial connection
  WiFi.begin("TechMaster", "21112017");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
 
}
void loop() {
  //Chuoi JSON 
    StaticJsonBuffer<1000> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSgps = JSONbuffer.createObject();
    JsonObject& JSgpsLocation = JSONbuffer.createObject();
    JsonObject& JSsum = JSONbuffer.createObject();
    JsonObject& JSvalues = JSONbuffer.createObject();
    JsonObject& JSTS1 = JSONbuffer.createObject();
    JsonObject& JSTS11 = JSONbuffer.createObject();
    JsonObject& JSTS2 = JSONbuffer.createObject();
    JsonObject& JSTS22 = JSONbuffer.createObject();
    JsonObject& JSTS3 = JSONbuffer.createObject();
    JsonObject& JSTS33 = JSONbuffer.createObject();
    JsonObject& JSTS4 = JSONbuffer.createObject();
    JsonObject& JSTS44 = JSONbuffer.createObject();

   
    JSgpsLocation["sourceId"] = "kckUd7LtaQ";
    JSgpsLocation["macAddress"] = "00-00-00-00-00-04";
    JSgps["latitude"] = "10.870385";
    JSgps["longitude"] = "106.80308";
    JSgpsLocation["gpsLocation"] = JSgps;
    JSsum["source"] = JSgpsLocation;

    JSTS11["value"] = (int)5;
    JSTS22["value"] = (int)5;
    JSTS33["value"] = (int)5;
    JSTS44["value"] = (int)5;
    
    
    JSTS1["code"] = "HUM";
    JSTS1["sensor"] = "Plan Tower";
    JSTS1["value"] = JSTS11;
    JSTS2["code"] = "TEMP";
    JSTS2["sensor"] = "Plan Tower";
    JSTS2["value"] = JSTS22;
    JSTS3["code"] = "PM2.5";
    JSTS3["sensor"] = "Plan Tower";
    JSTS3["value"] = JSTS33;
    JSTS4["code"] = "PM10";
    JSTS4["sensor"] = "Plan Tower";
    JSTS4["value"] = JSTS44;

    JsonArray& values = JSsum.createNestedArray("values"); //JSON array
    values.add(JSTS1); 
    values.add(JSTS2); 
    values.add(JSTS3);
    values.add(JSTS4);
 
    String JSONmessageBuffer;
    
    JSsum.prettyPrintTo(JSONmessageBuffer); //tao message hien thi json
    Serial.println(JSONmessageBuffer);

    String a = String(JSONmessageBuffer); //chuyen doi tuong json thanh chuoi string
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status    
    
    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
    Serial.println("Success");
  }
  //giao tiep server
  client.print("POST "+url+" HTTP/1.1\n");
  client.print("Host: "+host+"\n");
  client.print("Connection: close\n");
  client.print("Content-Type: application/json\n");
  client.print("Content-Length: ");
  client.print(a.length());
  client.print("\n\n");
  client.print(a);
      
  String str = client.readStringUntil('\n');//return result from server to client
  Serial.print(str);
  
  delay(30000);  //Send a request every 30 seconds
}
