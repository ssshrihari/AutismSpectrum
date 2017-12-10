

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>


//HARDWARE VARIABLES
byte randNumber = 1;
byte prevnum = 1;
int firsttime = 1;
int i = 0;
int t = 500;

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




//WIFI VARIABLES
const char* server = "api.thingspeak.com";
String apiKey ="8P2FH33P8C5NJZXQ";
const char* ssid = "shri";      //fill in your wifi name
const char* pass = "rtyufghv";  //fill in your wifi password
//const char* ssid = "";        //fill in your wifi name
//const char* pass = "";    //fill in your wifi password


int a[100]={0};
int k=0;
void connectWifi()
{
  Serial.print("Connecting to "+*ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
  delay(100);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");
}
//void sendfloatTS(float temp2)
//{
//   WiFiClient client;
//
//   if (client.connect(server, 80))
//   { 
//   Serial.println("WiFi Client connected ");
// 
//   //String postStr = apiKey;
//   //postStr += "&field2=";
//   //postStr += String(temp2);
//   //postStr += "\r\n\r\n";
// 
//   client.print("POST /update HTTP/1.1\n");
//   client.print("Host: api.thingspeak.com\n");
//   client.print("Connection: close\n");
//   //client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
//   client.print("Content-Type: application/x-www-form-urlencoded\n");
//   client.print("Content-Length: ");
//   //client.print(postStr.length());
//   client.print("\n\n");
//   //client.print(postStr);
//   
//   delay(10);
// 
//   }
//   
// client.stop();
//}//end send

void sendDurationTS(float LED_status)
{
   WiFiClient client;

   if (client.connect(server, 80))
   { 
   Serial.println("WiFi Client connected ");
 
 String postStr = apiKey;
 postStr += "&field1=";
 postStr += String(LED_status);
 postStr += "\r\n\r\n";
 
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   
   delay(10);
 
   }
   
 client.stop();
}//end send

void setup() 
{
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

  digitalWrite(ledr[0], HIGH);

    Serial.begin(9600);
  randomSeed(analogRead(0));
  Serial.print("");
   connectWifi();
   for(i=0;i<72;i++)
   {
    a[i]=i;
   }
   
   for(i=0;i<72;i++)
   {
    sendDurationTS(a[i]);
    delay(16000);
   }
}

void loop()
{

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

  //LEVEL NUMBER ONE
  if (levelcount == 0) //LEVEL ONE
  {
    if (sublevel < 3) //3 SUBLEVELS IN LEVEL 1
    {
      buttonState1 = analogRead(button);
      if (buttonState1 >= 460 && buttonState1 <= 480 && checker == 0)
      {
        startTime = millis();
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        checker = 1;
        
        delay(t);
      }
      buttonState2 = analogRead(button);
      if (buttonState2 >= 180 && buttonState2 <= 190 && checker == 1 )
      {
        endTime = millis();
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        duration = endTime - startTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 2;
        delay(t);
      }
      buttonState3 = analogRead(button);
      if (buttonState3 >= 71 && buttonState3 <= 90 && checker == 2)
      {
        startTime = millis();
        digitalWrite(led3, LOW);
        digitalWrite(led4, HIGH);
        duration = startTime - endTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 3;
        delay(t);
      }
      buttonState4 = analogRead(button);
      if (buttonState4 >= 61 && buttonState4 <= 70 && checker == 3)
      {
        endTime = millis();
        digitalWrite(led4, LOW);
        digitalWrite(led5, HIGH);
        duration = endTime - startTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 4;
        delay(t);

      }
      buttonState5 = analogRead(button);
      if (buttonState5 >= 50 && buttonState5 <= 60 && checker == 4)
      {
        startTime = millis();
        digitalWrite(led5, LOW);
        digitalWrite(led6, HIGH);

        duration = startTime - endTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 5;
        delay(t);
      }
      buttonState6 = analogRead(button);
      if (buttonState6 >= 30 && buttonState6 <= 40 && checker == 5)
      {
        endTime = millis();
        digitalWrite(led6, LOW);
        digitalWrite(led7, HIGH);

        duration = endTime - startTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 6;
        delay(t);
      }
      buttonState7 = analogRead(button);
      if (buttonState7 >= 24 && buttonState7 <= 29 && checker == 6)
      {
        startTime = millis();
        digitalWrite(led7, LOW);
        digitalWrite(led8, HIGH);

        duration = startTime - endTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 7;
        delay(t);
      }
      buttonState8 = analogRead(button);
      if (buttonState8 >= 20 && buttonState8 <= 23 && checker == 7)
      {
        endTime = millis();
        digitalWrite(led8, LOW);
        digitalWrite(led9, HIGH);

        duration = endTime - startTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        checker = 8;
        delay(t);
      }
      buttonState9 = analogRead(button);
      if (buttonState9 >= 15 && buttonState9 <= 19 && checker == 8)
      {
        startTime = millis();
        digitalWrite(led9, LOW);
        digitalWrite(led1, HIGH);
        checker = 0;
        duration = startTime - endTime;
        a[k]=duration;
        k++;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
        sublevel++;
      }
      if (sublevel == 3) //GO TO NEXT LEVEL
      {
        Serial.print("\nCompleted Testing level 1\n");
        levelcount = 1;
        digitalWrite(led1, HIGH);
        checker = 0;
        sublevel = 0;

      }
    }


  }

/*
  //LEVEL NUMBER TWO*
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


  //LEVEL THREE
  if (levelcount == 2) //level 2
  {
    if (sublevel < 3) //3 SUBLEVELS IN LEVEL 1
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
      if (buttonState2 >= 180 && buttonState2 <= 190 && checker == 5)
      {
        endTime = millis();
        digitalWrite(led2, LOW);
        digitalWrite(led6, HIGH);
        checker = 6;
        duration = endTime - startTime;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState3 = analogRead(button);
      if (buttonState3 >= 71 && buttonState3 <= 90 && checker == 2)
      {
        startTime = millis();
        digitalWrite(led3, LOW);
        digitalWrite(led9, HIGH);
        checker = 3;
        duration = startTime - endTime;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState4 = analogRead(button);
      if (buttonState4 >= 61 && buttonState4 <= 70 && checker == 4)
      {
        startTime = millis();
        digitalWrite(led4, LOW);
        digitalWrite(led2, HIGH);
        checker = 5;
        duration = startTime - endTime;
        Serial.print ("button\n ");
        Serial.println (duration);

        delay(t);
      }
      buttonState5 = analogRead(button);
      if (buttonState5 >= 50 && buttonState5 <= 60 && checker == 8)
      {
        startTime = millis();
        digitalWrite(led5, LOW);
        digitalWrite(led1, HIGH);
        checker = 0;
        duration = startTime - endTime;
        Serial.print ("button\n ");
        Serial.println (duration);
        sublevel++;
        delay(t);
      }
      buttonState6 = analogRead(button);
      if (buttonState6 >= 30 && buttonState6 <= 40 && checker == 6)
      {
        startTime = millis();
        digitalWrite(led6, LOW);
        digitalWrite(led8, HIGH);
        checker = 7;
        duration = startTime - endTime;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState7 = analogRead(button);
      if (buttonState7 >= 24 && buttonState7 <= 29 && checker == 1)
      {
        endTime = millis();
        digitalWrite(led7, LOW);
        digitalWrite(led3, HIGH);
        checker = 2;
        duration = endTime - startTime;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState8 = analogRead(button);
      if (buttonState8 >= 20 && buttonState8 <= 23 && checker == 7)
      {
        endTime = millis();
        digitalWrite(led8, LOW);
        digitalWrite(led5, HIGH);
        checker = 8;
        duration = endTime - startTime;
        Serial.print ("button\n ");
        Serial.println (duration);
        delay(t);
      }
      buttonState9 = analogRead(button);
      if (buttonState9 >= 15 && buttonState9 <= 19 && checker == 3)
      {
        endTime = millis();
        digitalWrite(led9, LOW);
        digitalWrite(led4, HIGH);
        checker = 4;
        duration = endTime - startTime;
        Serial.print ("button\n ");
        Serial.println (duration);
      }
      if (sublevel == 3)
      {
        Serial.print("\nCompleted Testing level 3\n");

        levelcount = 3;
        digitalWrite(ledr[0], HIGH);

        sublevel = 0;
      }
    }

  }
  //LEVEL FOUR
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
