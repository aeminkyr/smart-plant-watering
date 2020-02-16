#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

// Set these to run example.
#define FIREBASE_HOST "Your Firebase Host Url"
#define FIREBASE_AUTH "Your Firebase Auth Token"
#define WIFI_SSID "Your Wifi Ssid"
#define WIFI_PASSWORD "Your Wifi Password"

#define dht_dpin D6
DHT dht(dht_dpin, DHTTYPE); 
String  STemp, SHumid, MoustDivided, MoustDivided2;
//String path ="/PWD/Nem";

//String STemp;
//String SHumid;
int vr = A0; // variable resistor connected 
int sdata = 0; // The variable resistor value will be stored in sdata.
int pompapin = D0;
int lambapin = D1;
int lambdata = 0;
float sulama,   SMoust;
unsigned long startTime=0, currentTime=0;
const unsigned long period=600000;
int MoustTimes;
 int count = 0;
int milimeter;

void setup()
{
  // Debug console
    pinMode(pompapin, OUTPUT);

  Serial.begin(9600);
    dht.begin();

  pinMode(vr ,INPUT);
  // connect to wifi.
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
      {
    Serial.print(".");
    delay(500);
      }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
//startTime = millis();
 digitalWrite(pompapin,HIGH);

}

void loop()
{
 // currentTime = millis();
  
 float h = dht.readHumidity();
 float t = dht.readTemperature();       
 sdata = analogRead(vr);
 STemp = String(t);
 SHumid= String(h);
 SMoust = sdata; 

 milimeter = count*4.2 ;
 /* Şimdilik iptal
 if(currentTime- startTime > period){
  
 Firebase.pushString("hum", SHumid); //Push ile data sürekli eklendi
Firebase.pushString("temp", STemp);
Firebase.pushString("nem", SMoust);
  
  }
*/
//Serial.println(SMoust); 


Firebase.setFloat("s/moust",SMoust); //set string ile var olan data sürekli güncellendi
Firebase.setString("s/hum",SHumid);
Firebase.setString("s/temp",STemp);


  /*
   * FirebaseObject object = Firebase.get("PWD/");
String Nem = object.getString("Nem");
//String Nem = object.getString("Nem");
 Serial.println(Nem); 

            

            Firebase.setFloat("number", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }*/
 String milimeterDel = Firebase.getString("s/milimeterDel");
if (milimeterDel == "\"0\""){
  count = 0;
   Firebase.setString("s/milimeterDel","1");
  
      

  }


  
  
  //Serial.print("Pompa: ");
  //Serial.println(Firebase.getString("s/pompa"));
 String pompS = Firebase.getString("s/pompa");
 String lambS = Firebase.getString("s/lamba");
 String MoustDivided = Firebase.getString("s/moustUser");
 //String MoustDivided2 = MoustDivided.substring(1, MoustDivided.length() - 1).substring(0, MoustDivided.length() - 2); //gelen string verinin kırpılma işlemi. textbox için

int MoustTimes = (MoustDivided.toInt())*6; //integer dönüşümü ve yüzdelik deger icin carpim.
  


  //Burası uzaktan konrtol için
 if( pompS == "\"1\""){
  digitalWrite(pompapin, LOW);
  Firebase.setString("s/pompa_durumu","1");
  sayac();
  Firebase.setInt("s/milimeter",milimeter);
 
  }else if(pompS="\"0\"") {
    digitalWrite(pompapin, HIGH);
    Firebase.setString("s/pompa_durumu","0");
    }
    
    if(MoustTimes>SMoust){
  Serial.println("Sulama yapılıyor");
   // Firebase.setString("s/pompa_durumu","1");
   
    Firebase.setString("s/pompa","\"1\"");
  /*
  Serial.println(MoustDivided);
  
  Serial.println("MoustDivided2:");
  Serial.print(MoustDivided2);
  */
  }else if(MoustTimes<SMoust) { 
    Serial.println("Su yeterli");
       // Firebase.setString("s/pompa_durumu","0");
    Firebase.setString("s/pompa","\"0\"");

    /* 
    Serial.println(MoustDivided);
Serial.println("MoustDivided2:");
  Serial.print(MoustDivided2);*/
} 


  if(lambS == "\"1\""){
      digitalWrite(lambapin,HIGH);
    }else{digitalWrite(lambapin,LOW);}
   
}

void sayac(){
  for(;;){
    count = count + 1;
   /* Serial.println("Saniye: ");*/
 
 delay(1000);
 break;
    }
  
  
  }
