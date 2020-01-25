#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#define LED D5
#define FIREBASE_HOST "iot-proje-1f052.firebaseio.com"
#define FIREBASE_AUTH "fNdB20uW6IqU9d2OkbmoJx2lQyKZByhTaHHyekbN"
/*** Kablosuz Ağ Bilgileri ***/
const char*  WLAN_SSID  =   "krkcemruu";   //  
const char*   WLAN_PASSWORD = "emru12345" ;  //  
const char* thingSpeakHost = "api.thingspeak.com";  //184.106.153.149
String  writeAPIKey = "GBMZOBJO2M73QZYY";

int sayac;
float ortalamasicaklik;
/*** Değişken ve Pin Tanımlamaları ***/
int sicaklikSensor= A0;
float sicaklikdegeri;   //Analog değeri dönüştüreceğimiz sıcaklık değeri
float olculendeger;   //Ölçeceğimiz analog değer

WiFiClient client;

/*** ESP8266 WiFi Kurulum Fonksiyonu ***/
void WiFi_Setup()
{
 delay(10);
 Serial.println();
 Serial.print(WLAN_SSID);
 Serial.print("Kablosuz Agina Baglaniyor");
 WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
 // WiFi durum kontrolü
 while (WiFi.status() != WL_CONNECTED)
 {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print(WLAN_SSID);
  Serial.println("Kablosuz Agina Baglandi");
  Serial.println("IP adresi: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
 
  Serial.begin(115200);  // Seri port baud rate ayarı
  WiFi_Setup();          //Kablosuz ağ bağlantı fonksiyonu
  pinMode(A0,INPUT);
  pinMode(LED, OUTPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop()
{
  olculendeger = analogRead(sicaklikSensor); //A0'den değeri alacak
  olculendeger = (olculendeger/1024)*3300;//değeri mV'a dönüştürecek
  sicaklikdegeri = olculendeger /10,0; // mV'u sicakliğa dönüştürecek
  if (client.connect (thingSpeakHost, 80))
  {
         String postStr = "&field1=";
         postStr+= String (sicaklikdegeri);
         postStr+= "&field2=";
         postStr+= String (ortalamasicaklik);
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        client.print("\n\n");
  }
  if (sicaklikdegeri < 28)
  {
    digitalWrite(LED,LOW);
    Serial.println("LED OFF");
  }
  else
  {
    digitalWrite(LED,HIGH);
    Serial.println("LED ON");
  }
  ortalamasicaklik = ortalamasicaklik*sayac+sicaklikdegeri;
  sayac++;
  ortalamasicaklik=ortalamasicaklik/sayac;
  Firebase.setInt("sicaklik", sicaklikdegeri);
  Firebase.setInt("ortalamasicaklik", ortalamasicaklik);
  Serial.print("Su Anki Sicaklik....:");
  Serial.println(sicaklikdegeri);
  Serial.print("Ortalama Sicaklik....:");
  Serial.println(ortalamasicaklik);
  Serial.println ("Thingspeak veri gönderildi");  

client.stop();
delay (2500);

}
