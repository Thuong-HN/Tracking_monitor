#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid ="Deviot 2";
const char* pass = "12345679";
const char *ID = "System";
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  90
char msg[MSG_BUFFER_SIZE];
int value_string = 0;
String json;

IPAddress broker(192, 168, 1, 5);

WiFiClient wfclient;
PubSubClient client(wfclient);
String inputString="";
bool stringComplete;
bool stringTemp;
String CPU="";
String GPU="";
String Temp="";
String Chip="";
String RAM="";

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    if (client.connect(ID)) {
      //Serial.println("connected");
    } else {
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
  Serial.println('.');
  delay(50);
  }
  //Serial.println(WiFi.localIP());
  inputString.reserve(200);
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  serialEvent();
  Json_serial();
  inputString="";
  value_string = 0;
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {

}


void serialEvent() {
  while (Serial.available()) {          //  32u4 USB Serial Available?
    char inChar = (char)Serial.read();  // Read 32u4 USB Serial
    //Serial.print(inChar); // Debug Incoming Serial
    inputString += inChar;
    if (inChar == '|') {
      stringComplete = true;
      value_string++;
    }
}
}
void Json_serial(){
  DynamicJsonDocument doc(256);
  //sprintf (msg, "%s", inputString.c_str());
  doc["CPU"] = CPU;
  doc["GPU"] = GPU;
  doc["RAM"] = RAM;
  doc["Temp"] = Temp;
  doc["Chip"] = Chip;
  serializeJson(doc, json);
  if(CPU!=""&&RAM!=""){
    //client.publish("outTopic",inputString.c_str());
    client.publish("outTopic",json.c_str());
  }
  json=CPU=GPU=RAM=Chip=Temp="";
}
