#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define LED D4
#define LED1 D4
#define LED2 D4
#define LED3 D4
#define LED4 D4
#define LED5 D4
#define LED6 D4
#define LED7 D4
#define LED8 D4

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
  
  
//-----------------------Growth Chamber PH value--------------------------------------------------------------------------------------------------------------------------------

 if(payload[0] == 71 &&  payload[1] == 80){//automatic mode
    int GP_Ph_value  = ((int)payload[2]-48)*10+(int)(payload[3])-48;
    Serial.println(GP_Ph_value);
      if(GP_Ph_value<55 && GP_Ph_value >0 ){//turn on LED
        digitalWrite(LED1, HIGH);
      }
      else if(GP_Ph_value > 55 && GP_Ph_value < 90 ){
        digitalWrite(LED1, LOW);
      }
   }
  if(payload[0] == 71 &&  payload[1] == 80 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED1, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED1, HIGH);
    }
  }
 
 //-----------------------Growth Chamber EC value--------------------------------------------------------------------------------------------------------------------------------
 
  if(payload[0] == 71 &&  payload[1] == 69){//automatic mode
    int GE_EC_value  = ((int)payload[2]-48)*10+(int)(payload[3])-48;
    Serial.println(GE_EC_value);
      if(GE_EC_value<12 && GE_EC_value >0 ){//turn on LED
        digitalWrite(LED2, HIGH);
      }
      else if(GE_EC_value > 12 && GE_EC_value < 30 ){
        digitalWrite(LED2, LOW);
      }
   }
  if(payload[0] == 71 &&  payload[1] == 69 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED2, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED2, HIGH);
    }
  }
 
 //-----------------------Growth Chamber DO value--------------------------------------------------------------------------------------------------------------------------------
 
 if(payload[0] == 71 &&  payload[1] == 68){//automatic mode
    int GP_Ph_value  = ((int)payload[2]-48)*10+(int)(payload[3])-48;
    Serial.println(GP_Ph_value);
      if(GP_Ph_value<50 && GP_Ph_value >0 ){//turn on LED
      digitalWrite(LED3, HIGH);
      }
      else if(GP_Ph_value > 50 && GP_Ph_value < 90 ){
      digitalWrite(LED3, LOW);
      }
   }
  if(payload[0] == 71 &&  payload[1] == 68 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED3, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED3, HIGH);
    }
  }
 
 //-----------------------Growth Chamber Water Level--------------------------------------------------------------------------------------------------------------------------------
 
 if(payload[0] == 71 &&  payload[1] == 87){//automatic mode
    int GW_Water_Level  = ((int)payload[2]-48)*10+(int)(payload[3])-48;
    Serial.println(GW_Water_Level);
      if(GW_Water_Level<10 && GW_Water_Level >0 ){//turn on LED
        digitalWrite(LED4, HIGH);
      }
      else if(GW_Water_Level > 10 && GW_Water_Level < 20 ){
        digitalWrite(LED4, LOW);
      }
   }
  if(payload[0] == 71 &&  payload[1] == 87 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED4, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED4, HIGH);
    }
  }
  
  
//--------------------------Light Intensity Control-----------------------------------------------------------------------------------------------------------
  //L1
  if(payload[0] == 76 &&  payload[1] == 49){//automatic mode
    int Light_intensity_value = ((int)payload[2]-48)*100+((int)(payload[3])-48)*10 + (int)(payload[4])-48;
    Serial.println(Light_intensity_value);
      if(400 > Light_intensity_value  ){//turn on LED
        digitalWrite(LED6, HIGH);
      }
      else if((700 >Light_intensity_value) && (400 < Light_intensity_value)){
        digitalWrite(LED6, LOW);
      }
   }
  if(payload[0] == 76 &&  payload[1] == 49 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED6, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED6, HIGH);
    }
  }
 
}

//--------------------------Temperature and Air Flow Control-------------------------------------------------------------------------------------------------
  //T1
  if(payload[0] == 84 &&  payload[1] == 49){//automatic mode
    int inside_temperature_value = ((int)payload[2]-48)*10+((int)(payload[3])-48);
    int outside_temperature_value = ((int)payload[4]-48)*10+((int)(payload[5])-48);
    Serial.println(Light_intensity_value);
      if((28 < inside_temperature_value) && ( 28 > outside_temperature_value)  ){//turn on LED
        digitalWrite(LED7, HIGH);
      }
      else if((19 < inside_temperature_value) && ( 28 > inside_temperature_value)){
        digitalWrite(LED7, LOW);
      }
   }
  if(payload[0] == 84 &&  payload[1] == 49 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED7, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED7, HIGH);
    }
  }
 
}

//--------------------------Humidity Control-----------------------------------------------------------------------------------------------------------------
  //H1
  if(payload[0] == 72 &&  payload[1] == 49){//automatic mode
    int humidity_value = ((int)payload[2]-48)*10+((int)(payload[3])-48);
    Serial.println(Light_intensity_value);
      if(60 > humidity_value ){//turn on LED
      digitalWrite(LED8, HIGH);
      }
      else if((70 > humidity_value) && (60 < humidity_value )){
      digitalWrite(LED8, LOW);
      }
   }
  if(payload[0] == 72 &&  payload[1] == 49 &&  payload[2] == 95){//manual mode
    if(payload[3] == 111 && payload[4] == 102 && payload[5] == 102){//turn off LED manually
      digitalWrite(LED8, LOW);
    }
  
    else if(payload[3] == 111 && payload[4] == 110 ){//turn on LED manually
      digitalWrite(LED8, HIGH);
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
