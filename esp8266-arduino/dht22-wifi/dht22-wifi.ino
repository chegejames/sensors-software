/************************************************************/
/* DHT22 -> luftdaten.info api                              */
/************************************************************/

// increment on change
#define SOFTWARE_VERSION "MFA-2015-002-DHT22"

/**********************************************/
/* DHT declaration 
/**********************************************/
#include "DHT.h"
#define DHTPIN 13  // D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#include <SDS011.h>

/**********************************************/
/*Twitter declaration
/**********************************************/
#include <Twitter.h>

Twitter twitter("864009046327783424-P5RiR3VzQe52vmahVPbgVzkoq2yTigS");




/**********************************************/
/* WiFi declarations
/**********************************************/
#include <ESP8266WiFi.h>


const int AOUTpin=A0;//the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
const int DOUTpin=14;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino

int limit;
int value;

float p10,p25;
SDS011 my_sds;



const char* ssid = "Nairobi Garage";
const char* password = "Livestream";
// const char* host = "192.168.1.12";
const char* host = "api.luftdaten.info";
// const int httpPort = 8000;
const int httpPort = 80;

char msg;



unsigned long starttime;
unsigned long sampletime_ms = 30000;
float prev_t;


/**********************************************/
/* WiFi connecting script
/**********************************************/
void connectWifi() {
  WiFi.begin(ssid, password); // Start WiFI
  
  Serial.print("Connecting ");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
/**********************************************/
/* send data to rest api
/**********************************************/
void sendData(const String& data, int pin=-1) {
  //  delay(60000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // create an URI for the request
  String url = "/v1/push-sensor-data/";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  Serial.println(data);

  String pinstr=String('-');
  if (pin > 0 ){pinstr=String(pin);}
  // send request to the server
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Pin: " + pinstr + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Sensor: esp8266-");
  client.println(ESP.getChipId());
  client.print("Content-Length: ");
  client.println(data.length(), DEC);
  client.println("Connection: close\r\n");

  client.println(data);

  delay(1);
  
  // Read reply from server and print them
  while(client.available()){
    char c = client.read();
    Serial.print(c);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

String Float2String(float value)
{
  // Convert a float to String with two decimals.
  char temp[15];
  String s;

  dtostrf(value,13, 2, temp);
  s = String(temp);
  s.trim();
  return s;
}

// DHT22 Sensor
void sensorDHT(){
  String data;
  String tweet;
 float h = dht.readHumidity(); //Read Humidity
 float t = dht.readTemperature(); //Read Temperature

 // Check if valid number if non NaN (not a number) will be send.
 if (isnan(t) || isnan(h)) {
   Serial.println("DHT22 couldn’t be read");
 } else {
   Serial.print("Humidity : ");
   Serial.print(h);
   Serial.print(" \t");
   Serial.print("Temperature : ");
   Serial.print(t);
   Serial.println(" C");
   
   // json for push to api: h t
   data = "{\"software_version\": \"";
   data += SOFTWARE_VERSION;
   data += "\",";
   data += "\"sensordatavalues\":[{";
   data += "\"value_type\":\"temperature\",\"value\":\"";
   data += Float2String(t);
   data += "\"},{";
   data += "\"value_type\":\"humidity\",\"value\":\"";
   data += Float2String(h);
   data += "\"}]}";
   //sendData(data, DHTPIN);

   tweet = "It's Temperature : "+Float2String(t)+" C""Humidity : "+Float2String(h)+;
   char text[tweet.length()+1];
   strcpy(text, tweet.c_str());
   if ((t - prev_t) > 2)
     sendTweet(text);
   prev_t = t;
 
 }
}

void sendTweet(char* msg){
  if (twitter.post(msg)) {
    // Specify &Serial to output received response to Serial.
    // If no output is required, you can just omit the argument, e.g.
    // int status = twitter.wait();
    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}



/**********************************************/
/* The Setup     
/**********************************************/
void setup() {
  
  Serial.begin(9600); //Output to Serial at 9600 baud
  delay(10);
  //my_sds.begin(D1,D2);
  starttime = millis(); // store the start time
  delay(1000);
  //connectWifi(); // Start ConnecWifi 
  Serial.print("\n"); 
  Serial.println("ChipId: ");
  Serial.println(ESP.getChipId());
  String chipid = (String) ESP.getChipId();
  char text[chipid.length()+1];
  strcpy(text, chipid.c_str());
  //sendTweet(text);
  
  
}

/**********************************************/
/* And action
/**********************************************/
void loop() {
  value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
  delay(1000);
  /*Serial.print("CO value: ");
  Serial.print(value);//prints the CO value
  Serial.print("\t");
  Serial.print("Limit: ");
  Serial.println(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
  delay(1000);
  
  //my_sds.read(&p25,&p10);
 
  Serial.print("P2.5: "+String(p25));
  Serial.print("\t");
  Serial.println("P10:  "+String(p10));
  delay(1000);*/
 
  sensorDHT();
  
}

