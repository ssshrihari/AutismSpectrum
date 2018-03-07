#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <String.h>
#include "sha256.h"
#include "Base64.h"

// START: Azure Evet Hub settings
   
const char* KEY = "0+M34Ze/qM/1PzODJcaDCDsJ0KNTfjN7xD375GUYtyw=";    // event hub access key 
const char* KEY_NAME = "RootManageSharedAccessKey";                 // event hub key name  ( policy name) 
const char* HOST = "autism.servicebus.windows.net";              // event hub name (name of service bus)
const char* END_POINT = "/device1/messages";                 // name of the evnthub which we create inside eventhub namespace
// END: Azure Evet Hub settings 
// START: WiFi settings
const char* SSID = "Shri";
const char* PASSWORD = "rtyufghvcb";
// END: WiFi settings
String request;
String data;
String fullSas;
 WiFiClientSecure client;

//Project Variables
//HARDWARE VARIABLES
byte randNumber = 1;
byte prevnum = 1;
int firsttime = 1;
int i = 0;
int t = 10;

int button = A0;
int temp;


int led1 = D0;
int led2 = D1;
int led3 = D2;
int led4 = D3;
int led5 = D4;
int led6 = D5;
int led7 = D6;
int led8 = D7;
int led9 = D8;

int ledr[10] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;
int buttonState8 = 0;
int buttonState9 = 0;

int checker = 0;
int levelcount = 0;
int sublevel = 0;
int playagain = 0;
boolean start;


unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long duration = 0;


//Sending array
int values[200]={0};



void sendDurationTS(int LED_status)
{
WiFiClientSecure client;

  client.connect(HOST, 443);
 data = "{'deviceid':'esp8266'";
   data+= ", 'duration' : ";
   data+= LED_status;
   data+="}"; 
   request = String("POST ") + END_POINT + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Authorization: SharedAccessSignature " + fullSas + "\r\n" +                
               "Content-Type: application/atom+xml;type=entry;charset=utf-8\r\n" + 
               "Content-Length: " + data.length() + "\r\n\r\n" +
               data;
   client.print(request);
   Serial.println("Success");
  // Serial.println(request);
  
          
  /*int r=random(100,10000);
  int temp=random(25 , 45);
  int light= random(10 , 100);
  int soilm = random (10 , 70 );
  data = "{'deviceid':'esp8266'";
   data+= ", 'temperature' : ";
   data+= temp;
   data+=" , 'soilmoisture' : ";
   data+=soilm;
   data+=" , 'lightintensity' :";
   data+=light;
   data+="}"; 
   request = String("POST ") + END_POINT + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Authorization: SharedAccessSignature " + fullSas + "\r\n" +                
               "Content-Type: application/atom+xml;type=entry;charset=utf-8\r\n" + 
               "Content-Length: " + data.length() + "\r\n\r\n" +
               data;
    Serial.println(request);
  client.print(request);
 delay(100);*/
}












 
void setup() {
  
  
 Serial.begin(9600);
  // START: Naive URL Encode
  String url = "https://" + (String)HOST + (String)END_POINT;
  url.replace(":", "%3A");
  url.replace("/", "%2F");
  Serial.println(url);
  // END: Naive URL Encode

  // START: Create SAS
  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
  // Where to get secods since the epoch: local service, SNTP, RTC
  int expire = 1711104241;
  String stringToSign = url + "\n" + expire;

  // START: Create signature
  Sha256.initHmac((const uint8_t*)KEY, 44);
  Sha256.print(stringToSign);
  char* sign = (char*) Sha256.resultHmac();
  

  //Serial.println(String (Sha256.resultHmac));
  int signLen = 32;
  // END: Create signature

  // START: Get base64 of signature
  int encodedSignLen = base64_enc_len(signLen);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, signLen); 
  String encodedSas = (String) encodedSign;

  // Naive URL encode
  encodedSas.replace("=", "%3D");
  //Serial.println(encodedSas);
  // END: Get base64 of signature

  // SharedAccessSignature
   fullSas = "sr=" + url + "&sig="+ encodedSas + "&se=" + expire +"&skn=" + KEY_NAME;
  // END: create SAS
//Serial.println("SAS below");
//Serial.println(fullSas);
//Serial.println();
  // START: Wifi connection
  Serial.print("connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // END: Wifi connection

//PIN MODE SETUP
 pinMode(button, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  //Make led one blink in the beginning
   digitalWrite(led1, HIGH);
  randomSeed(analogRead(0));
  Serial.print("");
}


void loop() {

  
   /*The time is calculated as follows
     we take to variables start time and end time these two variables store the instances of time in millisecond using the method millis().
     at each instance of pressing a new button the time will be recorded and will be subtracted from the previous record.


  */
  /*
     There are three levels the first one is going 1->2->3->4->5....->9 .
     second one is 1->4->7->2->5->8->3->5->9
     third one is 1->7->3->9->4->2->6->8->4
     Each level is executed three times with the help of variable called sublevel

     To make sure that the required button is only pressed we use the checker at every instance of time
  */

  //delay(100);
  /* */
//Driver Program
 //LEVEL NUMBER ONE
  if (levelcount == 0) //LEVEL ONE
  {
    if (sublevel < 2) //3 SUBLEVELS IN LEVEL 1
    {
      buttonState1 = analogRead(button);
        Serial.println (buttonState1);
      if (buttonState1 >= 460 && buttonState1 <= 510 && checker == 0)
      {
        startTime = millis();
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        checker = 1;
        delay(t);
      }

      buttonState2 = analogRead(button);
            Serial.println (buttonState2);
      if (buttonState2 >= 150 && buttonState2 <= 300 && checker == 1 )
      {
        endTime = millis();
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 2;
        delay(t);
      }
      buttonState3 = analogRead(button);
        Serial.println (buttonState3);
      if (buttonState3 >= 75 && buttonState3 <= 140 && checker == 2)
      {
        startTime = millis();
        digitalWrite(led3, LOW);
        digitalWrite(led4, HIGH);
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 3;
        delay(t);
      }
      buttonState4 = analogRead(button);
        Serial.println (buttonState4);
      if (buttonState4 >=57 && buttonState4 <= 69 && checker == 3)
      {
        endTime = millis();
        digitalWrite(led4, LOW);
        digitalWrite(led5, HIGH);
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 4;
        delay(t);

      }
      buttonState5 = analogRead(button);
        Serial.println (buttonState5);
      if (buttonState5 >= 45 && buttonState5 <=55 && checker == 4)
      {
        startTime = millis();
        digitalWrite(led5, LOW);
        digitalWrite(led6, HIGH);

        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 5;
        delay(t);
      }
      buttonState6 = analogRead(button);
        Serial.println (buttonState6);
      if (buttonState6 >= 28 && buttonState6 <= 40 && checker == 5)
      {
        endTime = millis();
        digitalWrite(led6, LOW);
        digitalWrite(led7, HIGH);

        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 6;
        delay(t);
      }
      buttonState7 = analogRead(button);
        Serial.println (buttonState7);
      if (buttonState7 >= 24 && buttonState7 <= 27 && checker == 6)
      {
        startTime = millis();
        digitalWrite(led7, LOW);
        digitalWrite(led8, HIGH);

        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 7;
        delay(t);
      }
      buttonState8 = analogRead(button);
        Serial.println (buttonState8);
      if (buttonState8 >= 19 && buttonState8 <= 23 && checker == 7)
      {
        endTime = millis();
        digitalWrite(led8, LOW);
        digitalWrite(led9, HIGH);

        duration = endTime - startTime;
       // sendDurationTS(duration);
       values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 8;
        delay(t);
      }
      buttonState9 = analogRead(button);
        Serial.println (buttonState9);
      if (buttonState9 >= 12 && buttonState9 <= 18 && checker == 8)
      {
        startTime = millis();
        digitalWrite(led9, LOW);
        digitalWrite(led1, HIGH);
        checker = 0;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
        sublevel++;
      }
      if (sublevel == 1) //GO TO NEXT LEVEL
      {
        Serial.print("\nCompleted Testing level 1\n");
        levelcount = 0;
         digitalWrite(ledr[0], HIGH);
        digitalWrite(ledr[1], HIGH);
        digitalWrite(ledr[2], HIGH);
        digitalWrite(ledr[3], HIGH);
        digitalWrite(ledr[4], HIGH);
        digitalWrite(ledr[5], HIGH);
        digitalWrite(ledr[6], HIGH);
        digitalWrite(ledr[7], HIGH);
        digitalWrite(ledr[8], HIGH);
       for(i=0;i<8;i++){
          sendDurationTS(values[i]);
        }
        checker = 0;
        sublevel = 0;
         digitalWrite(ledr[0], HIGH);
        digitalWrite(ledr[1], LOW);
        digitalWrite(ledr[2], LOW);
        digitalWrite(ledr[3], LOW);
        digitalWrite(ledr[4], LOW);
        digitalWrite(ledr[5], LOW);
        digitalWrite(ledr[6], LOW);
        digitalWrite(ledr[7], LOW);
        digitalWrite(ledr[8], LOW);

      }
      
    }


  }


  /*
  if (levelcount == 1) //level 2
  {
    if (sublevel < 3) //3 SUBLEVELS IN LEVEL 1
    {
      buttonState1 = analogRead(button);
      if (buttonState1 >= 460 && buttonState1 <= 480 && checker == 0)
      {
        startTime = millis();
        digitalWrite(led1, LOW);
        digitalWrite(led4, HIGH);
        checker = 1;
        delay(t);
      }
      buttonState2 = analogRead(button);
      if (buttonState2 >= 180 && buttonState2 <= 190 && checker == 3)
      {
        endTime = millis();
        digitalWrite(led2, LOW);
        digitalWrite(led5, HIGH);
        duration = endTime - startTime;
       // sendDurationTS(duration);
       values[i++]=duration;
        Serial.print ("\nbutton\n ");
        Serial.println (duration);
        checker = 4;
        delay(t);
      }
      buttonState3 = analogRead(button);
      if (buttonState3 >= 71 && buttonState3 <= 90 && checker == 6 )
      {
        startTime = millis();
        digitalWrite(led3, LOW);
        digitalWrite(led6, HIGH);
        checker = 7;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState4 = analogRead(button);
      if (buttonState4 >= 61 && buttonState4 <= 70 && checker == 1 )
      {
        endTime = millis();
        digitalWrite(led4, LOW);
        digitalWrite(led7, HIGH);
        checker = 2;
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState5 = analogRead(button);
      if (buttonState5 >= 50 && buttonState5 <= 60 && checker == 4 )
      {
        startTime = millis();
        digitalWrite(led5, LOW);
        digitalWrite(led8, HIGH);
        checker = 5;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState6 = analogRead(button);
      if (buttonState6 >= 30 && buttonState6 <= 40 && checker == 7 )
      {
        endTime = millis();
        digitalWrite(led6, LOW);
        digitalWrite(led9, HIGH);
        checker = 8;
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState7 = analogRead(button);
      if (buttonState7 >= 24 && buttonState7 <= 29 && checker == 2 )
      {
        startTime = millis();
        digitalWrite(led7, LOW);
        digitalWrite(led2, HIGH);
        checker = 3;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState8 = analogRead(button);
      if (buttonState8 >= 20 && buttonState8 <= 23 && checker == 5)
      {
        endTime = millis();
        digitalWrite(led8, LOW);
        digitalWrite(led3, HIGH);
        checker = 6;
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState9 = analogRead(button);
      if (buttonState9 >= 15 && buttonState9 <= 19 && checker == 8 )
      {
        startTime = millis();
        digitalWrite(led9, LOW);
        digitalWrite(led1, HIGH);
        checker = 0;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
        sublevel++;
      }
      if (sublevel == 3)
      {
        Serial.print("\nCompleted Testing level 2\n");
        levelcount = 2;
        digitalWrite(led1, HIGH);
        checker = 0;
        sublevel = 0;


      }
    }
  }
*/

  /*//LEVEL THREE
  if (levelcount == 2) //level 2
  {
    if (sublevel < 2) //3 SUBLEVELS IN LEVEL 1
    {
      buttonState1 = analogRead(button);
      if (buttonState1 >= 460 && buttonState1 <= 480 && checker == 0 )
      {
        startTime = millis();
        digitalWrite(led1, LOW);
        digitalWrite(led7, HIGH);
        checker = 1;
        delay(t);
      }
      buttonState2 = analogRead(button);
       Serial.println (buttonState2);
      if (buttonState2 >= 180 && buttonState2 <= 190 && checker == 5)
      {
        endTime = millis();
        digitalWrite(led2, LOW);
        digitalWrite(led6, HIGH);
        checker = 6;
        duration = endTime - startTime;
       // sendDurationTS(duration);
       values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState3 = analogRead(button);
       Serial.println (buttonState3);
      if (buttonState3 >= 71 && buttonState3 <= 90 && checker == 2)
      {
        startTime = millis();
        digitalWrite(led3, LOW);
        digitalWrite(led9, HIGH);
        checker = 3;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState4 = analogRead(button);
       Serial.println (buttonState4);
      if (buttonState4 >= 61 && buttonState4 <= 70 && checker == 4)
      {
        startTime = millis();
        digitalWrite(led4, LOW);
        digitalWrite(led2, HIGH);
        checker = 5;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);

        delay(t);
      }
      buttonState5 = analogRead(button);
       Serial.println (buttonState5);
      if (buttonState5 >= 50 && buttonState5 <= 60 && checker == 8)
      {
        startTime = millis();
        digitalWrite(led5, LOW);
        digitalWrite(led1, HIGH);
        checker = 0;
        duration = startTime - endTime;
       // sendDurationTS(duration);
       values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        sublevel++;
        delay(t);
      }
      buttonState6 = analogRead(button);
       Serial.println (buttonState6);
      if (buttonState6 >= 30 && buttonState6 <= 40 && checker == 6)
      {
        startTime = millis();
        digitalWrite(led6, LOW);
        digitalWrite(led8, HIGH);
        checker = 7;
        duration = startTime - endTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState7 = analogRead(button);
       Serial.println (buttonState7);
      if (buttonState7 >= 24 && buttonState7 <= 29 && checker == 1)
      {
        endTime = millis();
        digitalWrite(led7, LOW);
        digitalWrite(led3, HIGH);
        checker = 2;
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState8 = analogRead(button);
       Serial.println (buttonState8);
      if (buttonState8 >= 20 && buttonState8 <= 23 && checker == 7)
      {
        endTime = millis();
        digitalWrite(led8, LOW);
        digitalWrite(led5, HIGH);
        checker = 8;
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState9 = analogRead(button);
       Serial.println (buttonState9);
      if (buttonState9 >= 15 && buttonState9 <= 19 && checker == 3)
      {
        endTime = millis();
        digitalWrite(led9, LOW);
        digitalWrite(led4, HIGH);
        checker = 4;
        duration = endTime - startTime;
        //sendDurationTS(duration);
        values[i++]=duration;
        Serial.print ("button\n ");
        Serial.println (duration);
      }
      if (sublevel == 2)
      {
        Serial.print("\nCompleted Testing level 3\n");

        levelcount = 3;
        digitalWrite(ledr[0], HIGH);
        digitalWrite(ledr[1], HIGH);
        digitalWrite(ledr[2], HIGH);
        digitalWrite(ledr[3], HIGH);
        digitalWrite(ledr[4], HIGH);
        digitalWrite(ledr[5], HIGH);
        digitalWrite(ledr[6], HIGH);
        digitalWrite(ledr[7], HIGH);
        digitalWrite(ledr[8], HIGH);
        for(i=0;i<32;i++){
          sendDurationTS(values[i]);
        }
        sublevel = 0;
        checker=0;
         digitalWrite(ledr[0], HIGH);
        digitalWrite(ledr[1], LOW);
        digitalWrite(ledr[2], LOW);
        digitalWrite(ledr[3], LOW);
        digitalWrite(ledr[4], LOW);
        digitalWrite(ledr[5], LOW);
        digitalWrite(ledr[6], LOW);
        digitalWrite(ledr[7], LOW);
        digitalWrite(ledr[8], LOW);
      }
    }

  }
 /*four*/
/*
  if (levelcount == 0)
  {
    while (i < 28)
    {
      buttonState1 = analogRead(button);
      if (buttonState1 >= 470 && buttonState1 <= 480 && checker == 0 )
      {
        digitalWrite(ledr[0], LOW);
        if (firsttime = 1)
        {

          startTime = millis();

          while (randNumber == 0)
          {
            randNumber = random(0, 8);
          }

          digitalWrite(ledr[randNumber], HIGH);
          firsttime = 0;
          start = true;

        }

        else
        {
          if (start)
          {
            endTime = millis();
            duration = endTime - startTime;
            Serial.print ("\nbutton\n ");
            Serial.println (duration);
            while (randNumber == 0)
              randNumber = random(0, 8);
            digitalWrite(ledr[randNumber], HIGH);
            start = false;
            i++;
          }
          else
          {
            startTime = millis();
            duration = startTime - endTime;
            Serial.print ("button\n ");
            Serial.println (duration);
            while (randNumber == 0)
              randNumber = random(0, 8);
            digitalWrite(ledr[randNumber], HIGH);
            start = true;
            i++;
          }
        }

        delay(t);
        checker = randNumber;
      }





      buttonState2 = analogRead(button);
      if (buttonState2 >= 170 && buttonState2 <= 180 && checker == 1)
      {

        digitalWrite(ledr[1], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 1)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 1)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }




      buttonState3 = analogRead(button);
      if (buttonState3 >= 71 && buttonState3 <= 80 && checker == 2)
      {
        digitalWrite(ledr[2], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 2)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 2)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }



      buttonState4 = analogRead(button);
      if (buttonState4 >= 55 && buttonState4 <= 62 && checker == 3)
      {
        digitalWrite(ledr[3], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 3)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 3)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }




      buttonState5 = analogRead(button);
      if (buttonState5 >= 40 && buttonState5 <= 50 && checker == 4)
      {
        digitalWrite(ledr[4], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 4)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 4)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }



      buttonState6 = analogRead(button);
      if (buttonState6 >= 22 && buttonState6 <= 30 && checker == 5)
      {
        digitalWrite(ledr[5], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 5)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 5)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }




      buttonState7 = analogRead(button);
      if (buttonState7 >= 17 && buttonState7 <= 21 && checker == 6)
      {
        digitalWrite(ledr[6], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 6)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 6)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }




      buttonState8 = analogRead(button);
      if (buttonState8 >= 11 && buttonState8 <= 15 && checker == 7 )
      {
        digitalWrite(ledr[7], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 7)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 7)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }




      buttonState9 = analogRead(button);
      if (buttonState9 >= 5 && buttonState9 <= 10 && checker == 8)
      {
        digitalWrite(ledr[8], LOW);
        if (start)
        {
          endTime = millis();
          duration = endTime - startTime;
          while (randNumber == 8)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = false;
        }
        else
        {
          startTime = millis();
          duration = startTime - endTime;
          while (randNumber == 8)
            randNumber = random(0, 8);
          digitalWrite(ledr[randNumber], HIGH);
          start = true;
        }
        checker = randNumber;
        Serial.print ("button\n ");
        Serial.println (duration);
        i++;
        delay(t);
      }

      if (i == 28) //GO TO NEXT LEVEL
      {
        levelcount = 0;
        digitalWrite(ledr[1], LOW);
        digitalWrite(ledr[2], LOW);
        digitalWrite(ledr[3], LOW);
        digitalWrite(ledr[4], LOW);
        digitalWrite(ledr[5], LOW);
        digitalWrite(ledr[6], LOW);
        digitalWrite(ledr[7], LOW);
        digitalWrite(ledr[8], LOW);
        digitalWrite(led1, HIGH);

        sublevel = 0;
        Serial.print("\n Completed Testing level 4\n");
        Serial.print("\n You can start again");

      }
    }

  }*/
  
}
