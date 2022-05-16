# Home Automation Using Google Assistance
This project deals with the controlling of home appliances using IoT.

## Hardware Requirement
1. Power Supply (5V 2A Charger)
2. Arduino Uno Board
3. ESP8266 WiFi Module
4. Relay - 2no


## Thingspeak server details
In this project we used thingspeak as a cloud service.  
From user the data will be send to thingspeak server and that data will be received in the hardware and trigger respective device.
Steps to setup thingspeak APIs
1. Login to Thingspeak (https://thingspeak.com/login?skipSSOCheck=true)
2. Create a Channel -> new channel -> 
3. Open channel -> API Keys -> API Requests -> (take write channel feed API)

## Read / GET
In the code, we use channel number and field number to get the last updated value.

## Write / Push
We have to push the data to cloud. Using 3 rd point (write API) post the data to server. (This action might be done using any type of third party application)


## Third party Application Configuration
Here we used IFTTT application for the switching inderectly. IFTTT is If This Then That.
1. We have created an applet
2. If This -> Google Assistance
3. Then That -> Webhook
4. In 2nd point, select Google assistance -> select simple phrase -> provide 3 sentense to detect -> provide the response sentense -> Update
5. In 3rd point, select Webhook -> provide URL link -> select method to GET -> select application/json -> update.
6. After configuring Google Assistance and Webhook, check the applet.

## Runtime Usage
After loading the code to Arduino Uno and powering up. Give an input using Google Assistance with provided commadn (Ex: "Switch on the light"). 
Then via webhook the write API will be executed and the data will be send to thingspeak server. In hardware it will continuously check the last updated value.
Based on value the device will be triggered.
