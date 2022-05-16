/*
 * Home Automation using IoT (Arduino Uno & ESP8266)
 * The code uses WiFiEsp.h and ThingSpeak.h 
 * Date: 14/05/2022
 * Author: Rakesh Malepu
*/

#include "WiFiEsp.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

/* WIFI Credentials*/
char ssid[] = "Rakesh 2.4G";   // your network SSID (name) 
char pass[] = "Password@098";   // your network password

/* WiFi Clinet object creation*/
WiFiEspClient  client;

// Emulate Serial1 on pins 10/11 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(10, 11); // RX, TX
#define ESP_BAUDRATE  19200
#else
#define ESP_BAUDRATE  115200
#endif

/* ThingSpeak channel details */
unsigned long channelNumber = 231164;
unsigned int Relay1_fld = 1;  //Field Number 1
unsigned int Relay2_fld = 2;  //Field Number 2

/* Global Variables */
int Relay1 = 2;
int Relay2 = 3;
int relay1_status;
int relay2_status;
int previousStatus1;
int previousStatus2;

/* Setup Function to initialize */
void setup() {
  /* setup relays */
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  
 /* Initialize serial  */
  Serial.begin(115200);  
  /* initialize serial for ESP module */
  setEspBaudRate(ESP_BAUDRATE);
  Serial.print("Searching for ESP8266..."); 
  /* initialize ESP module */
  WiFi.init(&Serial1);
  /* check for the presence of the shield */
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
  /* Initialize ThingSpeak */   
  ThingSpeak.begin(client);  
}

/* Loop Function for continuous process */
void loop() {
  int statusCode = 0;
  /* Connect or reconnect to WiFi */
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected");
  }

  /* Switching Relay 1 based on cloud data */
  int ret = getRelayStatus(channelNumber, Relay1_fld);
  if(ret == 1){
    previousStatus1 = 1;
    relay1_status = 1;
  }
  else if(ret == 0){
    previousStatus1 = 0;
    relay1_status = 0;
  }
  else if(ret == -1) {
    relay1_status = previousStatus1;
  }
  else {
    relay1_status = previousStatus1;
  }
  if (relay1_status == 1) {
    Serial.println("Relay1 ON");  
    switchRelay(Relay1, LOW);
  }
  else if (relay1_status == 0){
    Serial.println("Relay1 OFF");
    switchRelay(Relay1, HIGH);
  }
  delay(200);

  /* Switching Relay 2 based on cloud data */
  ret = getRelayStatus(channelNumber, Relay2_fld);
  if(ret == 1){
    previousStatus2 = 1;
    relay2_status = 1;
  }
  else if(ret == 0){
    previousStatus2 = 0;
    relay2_status = 0;
  }
  else if(ret == -1) {
    relay2_status = previousStatus2;
  }
  else {
    relay2_status = previousStatus2;
  }
  if (relay2_status == 1) {
    Serial.println("Relay2 ON");
    switchRelay(Relay2, LOW);  
  }
  else if (relay2_status == 0){
    Serial.println("Relay2 OFF");
    switchRelay(Relay2, HIGH);
  }
  delay(2000); 
}

/*
 * Function to Switch the relay
 * relay - Number of the relay/digital pin 
 * trig - HIGH/LOW
 */
void switchRelay(int relay, int trig) {
  delay(500);
  digitalWrite(relay, trig);
}

/*
 * Function to get the relay status from cloud
 * chnl_num - channel number from thingspeak server
 * fld_num  - field number from thingspeak server
 * returns the last value from server on success / -1 on failure
 */
int getRelayStatus(unsigned long chnl_num, unsigned long fld_num) {
  
  /* Read in field 1 of the public channel recording  */
  int relayStatus = ThingSpeak.readIntField(chnl_num, fld_num);  

  /* Check the status of the read operation to see if it was successful */
  int statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200){
    return relayStatus;
  }
  else{
    return -1;
  }
}


/* This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports */
/* can use 115200, otherwise software serial is limited to 19200. */
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
  Serial1.begin(baudrate);
}
