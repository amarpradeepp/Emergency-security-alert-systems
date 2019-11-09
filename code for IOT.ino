//serial1 is gsm
// serial 2 is gps
#include<LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2); //(rs,rw=gnd,e,d3,d4,d5,d6,)
const int buttonPin = 8;
int buttonState = 0;

const int pir = 9; //pir
const int vib = 10; //vibration sensor
const int buzzer = 11;
int e = 0;
int f = 0;
int g = 0;

//String str="";
char str[70];

int i=0,k=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String gpsString="";
char *test="$GPRMC";  //AT commanad
int temp=0,j;
int temp1=0,l;
int temp2=0,n;
int sys_status=1;

void initModule(String cmd, char *res, int t) // gsm module initialize
{
  while(1)
  {
    Serial.println(cmd);
    Serial1.println(cmd);
    Serial2.println(cmd);
    delay(100);
    while(Serial1.available()>0)
    {
      if(Serial1.find(res))
      {
        Serial.println(res);
        delay(t);
        return;
      }
      else
      {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}



void setup() 
{
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode (buzzer, OUTPUT); //buzzer
  pinMode(buttonPin, INPUT);
  pinMode (pir, INPUT); //pir
  pinMode (vib, INPUT); //cvibration
   
  lcd.begin(16,2);  
  lcd.print("universal security");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(2000);
  lcd.clear();
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  delay(1000);
  
  Serial.println("Initializing....");
  initModule("AT","OK",1000);
  initModule("ATE1","OK",1000);
  initModule("AT+CPIN?","READY",1000);  
  initModule("AT+CMGF=1","OK",1000);     
  initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
  Serial.println("Initialized Successfully");
  lcd.clear();
  lcd.print("Initialized");
  lcd.setCursor(0,1);
  lcd.print("Successfully");
  delay(2000);
  lcd.clear();
  
 
  lcd.print("Waiting For GPS");
  lcd.setCursor(0,1);
  lcd.print("     Signal    ");
  delay(2000);
  Serial2.begin(9600);
  get_gps();
  show_coordinate();
  delay(2000);
  lcd.clear();
  lcd.print("GPS is Ready");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  Serial.println("System Ready..");

}



void loop() 
{
  {
    int e = digitalRead(buttonPin); //button
    int f = digitalRead(pir);//pir
    int g = digitalRead(vib);//vibration
    /*serialEvent();
    if(temp)
    {
      get_gps();
      show_coordinate();
      Send();
      Serial.println("");
      Serial.println("SMS Sent");
      delay(2000);
      lcd.clear();
      lcd.print("System Ready");
    }
    if(temp1)
    {
      lcd.clear();
      lcd.setCursor (0,1);
      lcd.print("switching on    ");
      lcd.setCursor(1,0);
      lcd.print("please wait");
      Serial.println("system switched on");
      
      sendSMS("system on");
      Send();
    }
    if(temp2)
    {
      lcd.clear();
      lcd.setCursor (0,1);
      lcd.print("switching off    ");
      lcd.setCursor(1,0);
      lcd.print("please wait");
      Serial.println("system switched on");
      
      sendSMS("system off");
      //Send();
    }*/
    
    /* if(Serial1.find("Switch on"))
    {
      lcd.clear();
      lcd.setCursor (0,1);
      lcd.print("switching on    ");
      lcd.setCursor(1,0);
      lcd.print("please wait");
      Serial.println("system switched on");
      
      sendSMS("system on");
    }*/

    if(Serial1.find("SYSTEMON"))
    {  
      lcd.clear();
      sys_status=1;
      lcd.setCursor (0,1);
      lcd.print("switching on ");
      lcd.setCursor(1,0);
      lcd.print("sending sms");
      Serial.println("Sending SMS for switch on");
      sendSMS("system on");
      Serial.println("SMS Sent for System ON");
    }
   
    if(Serial1.find("LOCATE"))
    {  
      lcd.clear();
      lcd.setCursor (0,1);
      lcd.print("tracking vehicle");
      lcd.setCursor(1,0);
      lcd.print("sending sms");
      Serial.println("Sending SMS for tracking device"); 
      get_gps();
      show_coordinate();
      Send();
    }

    if(Serial1.find("SYSTEMOFF"))
    {  
      lcd.clear();
      sys_status=0;
      lcd.setCursor (0,1);
      lcd.print("switching off ");
      lcd.setCursor(1,0);
      lcd.print("sending sms");
      Serial.println("Sending SMS for switch off");
      sendSMS("system off");
      Serial.println("SMS Sent for System OFF");
      //Send();
      lcd.clear();
      lcd.print("system off");
    }

    if((e == HIGH) &&(f == LOW)&&(g == LOW))
    {
      if(sys_status == 1)
      {
        get_gps();
        show_coordinate();
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Sending SMS ");
        lcd.setCursor(1,0);
        lcd.print("for accident");
        Serial.println("Sending SMS for emergency response team");
        Send();
        delay(3000);
        Serial1.println("ATD8618621245;\r");
        delay(10000);
        Serial.println("");
        Serial.println("SMS Sent");
        delay(2000);
        lcd.clear();
        lcd.print("System Ready");      
        }
        else
        {
          lcd.print("System is OFF");
          Serial.println("");
          Serial.println("SMS Sent");
        }
    } 


    if ((e == LOW) &&(f == HIGH)&&(g == LOW))
    {
      if(sys_status == 1)
      {
        lcd.clear();
        lcd.print("motion detected");
        digitalWrite(buzzer,HIGH);
        delay(500);
        Serial.println("Sending SMS for pir");
        sendSMS("intruder detected");
        Serial.println("SMS Sent");
        delay(4000);
        Serial1.println("ATD8618621245;\r");
        delay(10000);
        lcd.clear();
        lcd.print("System Ready");      
      }
      else
      {
        lcd.print("System is OFF");
        Serial.println("");
        Serial.println("SMS Sent");
      }
    }



    if ((e == LOW) && (f == LOW) && (g == HIGH) )
    {
      if(sys_status == 1)
      {
        digitalWrite(buzzer,HIGH);
        delay(500);
        digitalWrite(buzzer,HIGH);
        delay(500);
        lcd.clear();
        lcd.print("vibration detected");
        Serial.println("Sending SMS for vibration");
        sendSMS("vibration detected");
        Serial.println("SMS Sent");
        delay(4000);
        Serial1.println("ATD8618621245;\r");
        delay(10000);
        lcd.clear();
        lcd.print("System Ready");      
      }
      else
      {
        lcd.print("System is OFF");
        Serial.println("");
        Serial.println("SMS Sent");
      }
    }
    
    if ((e == LOW) && (f == LOW) && (g == LOW) )
    {
      if(sys_status == 1)
      {
        lcd.clear();
        lcd.print("SYSTEM ON");
        digitalWrite(buzzer,LOW);
        delay(1000);
        lcd.clear();
        lcd.print("SYSTEM READY");      
      }
      else
      {
        lcd.clear();
        lcd.print("SYSTEM OFF");
        digitalWrite(buzzer,LOW);
        delay(1000);
        lcd.clear();
        lcd.print("SYSTEM OFF");
      }
    }
  }
  
}





void gpsEvent()
{
  gpsString="";
  while(1)
  {
    while (Serial2.available()>0)            //Serial incoming data from GPS
    {
      char inChar = (char)Serial2.read();
      gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
      i++;
      // Serial.print(inChar);
      if (i < 7)                      
      {
        if(gpsString[i-1] != test[i-1])         //check for right string
        {
          i=0;
          gpsString="";
        }
      }
      if(inChar=='\r')
      {
        if(i>60)
        {
          gps_status=1;
          break;
        }
        else
        {
          i=0;
        }
      }
    }
    if(gps_status)
      break;
  }
}

/*void serialEvent()
{
  while(Serial.available())
  {
    if(Serial.find("Track Vehicle"))
    {
      temp=1;
      temp1=0;
      temp2=0;
      break;
    }
    if(Serial.find("Switch On"))
    {
      //temp1=1;
      temp=0;
      temp1=1;
      temp2=0;
      break;
    }
    if(Serial.find("PoweR"))
    {
      //temp2=1;
      temp=0;
      temp1=0;
      temp2=1;
      break;
    }
    else
    {
      temp=0;
      temp1=0;
      temp2=0;
    }
  }
}*/

void get_gps()
{
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
  gps_status=0;
  int x=0;
  while(gps_status==0)
  {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
  }
}

void show_coordinate()
{
  lcd.clear();
  lcd.print("Lat:");
  lcd.print(latitude);
  lcd.setCursor(0,1);
  lcd.print("Log:");
  lcd.print(logitude);
  Serial.print("Latitude:");
  Serial.println(latitude);
  Serial.print("Longitude:");
  Serial.println(logitude);
  delay(2000);
  lcd.clear();    
}

void coordinate2dec()
{
  String lat_degree="";
  for(i=20;i<=21;i++)         
    lat_degree+=gpsString[i];
      
  String lat_minut="";
  for(i=22;i<=28;i++)         
    lat_minut+=gpsString[i];

  String log_degree="";
  for(i=32;i<=34;i++)
    log_degree+=gpsString[i];

  String log_minut="";
  for(i=35;i<=41;i++)
    log_minut+=gpsString[i];
    
  
  float minut= lat_minut.toFloat();
  minut=minut/60;
  float degree=lat_degree.toFloat();
  latitude=degree+minut;
     
  minut= log_minut.toFloat();
  minut=minut/60;
  degree=log_degree.toFloat();
  logitude=degree+minut;
}

void Send()
{ 
  Serial1.println("AT");
  delay(500);
  serialPrint();
  Serial1.println("AT+CMGF=1");
  delay(500);
  serialPrint();
  Serial1.print("AT+CMGS=");
  Serial1.print('"');
  Serial1.print("8618621245");    //mobile no. for SMS alert
  Serial1.println('"');
  delay(500);
  Serial1.println("PLEASE HELP AT THIS LOCATION"); 
  serialPrint();
  Serial1.print("Latitude:");
  Serial1.println(latitude);
  delay(500);
  serialPrint();
  Serial1.print(" longitude:");
  Serial1.println(logitude);
  delay(500);
  serialPrint();
  lcd.clear();
  lcd.print("SMS Sent");
  delay(1000);
  lcd.begin(16,2);
  serialPrint();
  Serial1.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
  Serial1.print(latitude,6);
  Serial1.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
  Serial1.print(logitude,6);
  Serial1.write(26);
  delay(2000);
  serialPrint();
}

void serialPrint()
{
  while(Serial1.available()>0)
  {
    Serial.print(Serial1.read());
  }
}

void sendSMS(String msg)
{
  lcd.clear();
  lcd.print("Sending SMS");
  Serial1.println("AT+CMGF=1");
  delay(500);
  Serial1.print("AT+CMGS=");
  Serial1.print('"');
  Serial1.print("+918618621245");    // number
  Serial1.print('"');
  Serial1.println();
  delay(500);
  Serial1.println(msg);
  delay(500);
  Serial1.write(26);
  delay(1000);
  lcd.clear();
  lcd.print("SMS Sent");
  delay(1000);
  lcd.begin(16,2);
}
