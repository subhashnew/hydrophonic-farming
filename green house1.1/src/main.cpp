#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

//LED for the Fertilezer Unit
#define LED D4
#define LED_F D5

//LED for the resevior unit
#define LED_RL D3 //for water level low
#define LED_RH D2 //for water level high
#define LED_RD D1 //for water drain

const char* SSID = "Dialog 4G 304";
const char* PWD = "subnew19658";

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "CO326/proj";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//connect to the wifi
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

 /*
 ================  The fertilizer unit =====================
 1. Check whether the first character = F (70), second character = 1 (49) 
 2. Settig the F1_Ph_value by combining two numbers in 3rd,4th character , Deduct 48 to get the int value from ascii values
 */
//++++++++++++++++++++++++ F1 +++++++++++++++++++++++++++++++++++++++++++
//issue found : manual mode also going to the automatic if condition
//Solution : && payload[2]<58 & payload[2]>47 (only for 10 numbers)

 if(payload[0] == 70 &&  payload[1] == 49){//automatic mode
    int F1_Ph_value  = ((int)payload[2]-48)*10+(int)(payload[3])-48;
    Serial.println(F1_Ph_value);
      if(F1_Ph_value<55 && F1_Ph_value >0 ){//turn on LED
      digitalWrite(LED_F, HIGH);
      }
      else if(F1_Ph_value > 55 && F1_Ph_value < 90 ){
      digitalWrite(LED_F, LOW);
      }
  }
  if(payload[0] == 70 &&  payload[1] == 49 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED_F, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED_F, HIGH);
    }
  }

  /*
  =================  Resevior Unit   =======================
  
  */

   /*
  +++++++++++++++++++++  R2 - Resevior Level low   ++++++++++++++++++++++
  1. Check whether the first character = R (82), second character = 2 (50)
  */
  if(payload[0] == 82 &&  payload[1] == 50){   //automatic mode
    int water_low  = ((int)payload[2])-48;
    Serial.println(water_low);
      if(water_low == 0 ){   //turn on LED
      digitalWrite(LED_RL, HIGH);
      }
      else if(water_low == 1){
      digitalWrite(LED_RL, LOW);
      }
  }
  if(payload[0] == 82 &&  payload[1] == 50 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED_RL, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED_RL, HIGH);
    }
  }
  
  /*
  +++++++++++++++++++++  R3 - Resevior Level High   ++++++++++++++++++++++
  1. Check whether the first character = R (82), second character = 3 (51)
  */
  if(payload[0] == 82 &&  payload[1] == 51){//automatic mode
    int water_high  = ((int)payload[2]-48);
    Serial.println(water_high);
      if(water_high == 1 ){//turn on LED
      digitalWrite(LED_RH, HIGH);
      }
      else if(water_high == 0){
      digitalWrite(LED_RH, LOW);
      }
  }
  if(payload[0] == 82 &&  payload[1] == 51 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED_RH, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED_RH, HIGH);
    }
  }

  /*
  +++++++++++++++++++++  R4 - Drain   ++++++++++++++++++++++
  1. Check whether the first character = R (82), second character = 4 (52)
  */
  if(payload[0] == 82 &&  payload[1] == 52){//automatic mode
    int drain  = ((int)payload[2]-48);
    Serial.println(drain);
      if(drain == 1 ){//turn on LED
      digitalWrite(LED_RD, HIGH);
      }
      else if(drain == 0){
      digitalWrite(LED_RD, LOW);
      }
  }
  if(payload[0] == 82 &&  payload[1] == 52 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED_RD, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED_RD, HIGH);
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
