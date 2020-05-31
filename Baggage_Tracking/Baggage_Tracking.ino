#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

const char* server = "garbage-monitoring-syste-f1f56.appspot.com";

static const int RXPin = 5, TXPin = 4;   // GPIO 4=D2(conneect Tx of GPS) and GPIO 5=D1(Connect Rx of GPS)
static const uint32_t GPSBaud = 9600; //if Baud rate 9600 didn't work in your case then use 4800

TinyGPSPlus gps; // The TinyGPS++ object

WiFiClient client;

SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device


float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS

char auth[] = "G-QNFYPNepq8AcNDfUf6QtgUCstqSm3";              //Your Project authentication key
char ssid[] = "IOT_J";                                       // Name of your network (HotSpot or Router name)
char pass[] = "10101010";                                      // Corresponding Password

const char *ssid1 =  "Param";    
const char *pass1 =  "87654321";


//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
  

void setup()
{
  Serial.begin(9600);
  Serial.println();
  ss.begin(GPSBaud);
  
  Serial.println("Connecting to ");
       Serial.println(ssid1);
 

       WiFi.begin(ssid1, pass1);
 
      while (WiFi.status() != WL_CONNECTED)
     {
            delay(1000);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
  
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(("No GPS detected: check wiring."));
  }
}

void loop()
{
  delay(5000);
  checkGPS();
 
    while (ss.available() > 0) 
    {
       //sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read()))
        displayInfo();
  }
}

void displayInfo()
{ 

  if (gps.location.isValid() ) 
  {
    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);

    if(WiFi.status()==WL_CONNECTED){

  if (client.connect(server,80))   
        {  
                            
          String postStr = "";
          postStr +="lat=";
           postStr += String(latitude);
           postStr +="&long=";
           postStr += String(longitude);
           postStr += "\r\n\r\n";
 
          client.print("POST /location HTTP/1.1\n");
          client.print("Host: garbage-monitoring-syste-f1f56.appspot.com\n");
          client.print("Connection: close\n");
          client.print("Content-Type: application/x-www-form-urlencoded\n");
          client.print("Content-Length: ");
          client.print(postStr.length());
          client.print("\n\n");
          client.print(postStr);
                        }
                        else{
                          Serial.println("Connection Failed");
                        }
                        
          client.stop();
}
delay(20000);
    
 }
  

  Serial.println();
}
