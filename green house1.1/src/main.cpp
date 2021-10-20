#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define LED D4

const char* SSID = "Dialog 4G 304";
const char* PWD = "subnew19658";

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "CO326/proj";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//connect to the wifi
void connectToWiFi() {
  Serial.print("Connectiog");
 
  WiFi.begin(SSID, PWD);
  //Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nConnected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

//handling the incomming message
void handleMessage(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
     
 }
 Serial.println();
 Serial.println("-----------------------");

 if(payload[0] == 70 &&  payload[1] == 49){//automatic mode
    int F1_Ph_value  = ((int)payload[2]-48)*10+(int)(payload[3])-48;
    Serial.println(F1_Ph_value);
      if(F1_Ph_value<55 && F1_Ph_value >0 ){//turn on LED
      digitalWrite(LED, HIGH);
      }
      else if(F1_Ph_value > 55 && F1_Ph_value < 90 ){
      digitalWrite(LED, LOW);
      }
   }
  if(payload[0] == 70 &&  payload[1] == 49 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED, HIGH);
    }
  }
 
}



void setup() {
  Serial.begin(9600);
  connectToWiFi();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(handleMessage);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str())) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }

 }

 // publish and subscribe
 //client.publish(topic, "Hi EMQ X I'm ESP32");
 client.subscribe(topic);

}

void loop() {
  client.loop();

}
