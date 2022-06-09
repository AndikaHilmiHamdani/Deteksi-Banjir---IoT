#define sensorLDR D6
#define triggerPin  D8
#define echoPin     D7

#include <dht.h>
#define sensor 14
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "De-72";
const char* password = "gakdisandi";

// Initialize Telegram BOT
#define BOTtoken "5143804909:AAEF4UDID0sfHaCLFWuZQHAqxsRs0ooAATw"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "821934636"
#define CHAT_Aula "1034450435"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

dht DHT;
String humidity = "Kelembaban : ";
String temp = "Suhu : ";
int nilaiSensor = analogRead(sensorLDR);

void cek() {
  DHT.read11(sensor);

  Serial.print(temp);
  Serial.println(DHT.temperature);
  Serial.print(humidity);
  Serial.println(DHT.humidity);
  Serial.print("Cahaya: ");
  Serial.print(cahaya());
  Serial.println(nilaiSensor);
  

  long duration, jarak;

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  Serial.print("jarak : ");
  Serial.print(jarak);
  Serial.println(" cm"); 
 
  String hasilTemp = "Suhu: " + String(DHT.temperature)+ " °C";
  String hasilHum = "Kelembaban: " + String(DHT.humidity)+ " %";
  String hasilLDR = "Cuaca: " + cahaya();
  String hasilJrk = "Jarak: " + String(jarak)+" Cm";
  bot.sendMessage(CHAT_ID, hasilTemp, "");
  bot.sendMessage(CHAT_ID, hasilHum, "");
  bot.sendMessage(CHAT_ID, hasilLDR, "");
  bot.sendMessage(CHAT_ID, hasilJrk, "");

  bot.sendMessage(CHAT_Aula, hasilTemp, "");
  bot.sendMessage(CHAT_Aula, hasilHum, "");
  bot.sendMessage(CHAT_Aula, hasilLDR, "");
  bot.sendMessage(CHAT_Aula, hasilJrk, "");

   if (jarak < 30){
    if(DHT.temperature <= 25 && DHT.humidity >= 60) {
      Serial.println("Waspada air meningkat dan berpotensi curah hujan tinggi");
      bot.sendMessage(CHAT_ID, "Waspada air meningkat dan berpotensi curah hujan tinggi", "");
      bot.sendMessage(CHAT_Aula, "Waspada air meningkat dan berpotensi curah hujan tinggi", "");
    } 
    else if((DHT.temperature > 25 && DHT.temperature < 29) && DHT.humidity < 60) {
      Serial.println("Hati-hati debit air meningkat, namun cuaca cerah");
      bot.sendMessage(CHAT_ID, "Hati-hati debit air meningkat, namun cuaca cerah", "");
      bot.sendMessage(CHAT_Aula, "Hati-hati debit air meningkat, namun cuaca cerah", "");
    }
    else {
      Serial.println("Waspada air belum menguap");
      bot.sendMessage(CHAT_ID, "Waspada air belum menguap", "");
      bot.sendMessage(CHAT_Aula, "Waspada air belum menguap", "");
    }
  }
   else {
      Serial.println("Kondisi aman");
      bot.sendMessage(CHAT_ID, "Kondisi aman", "");
      bot.sendMessage(CHAT_Aula, "Kondisi aman", "");
    }
  delay(1000);
  
}

String cahaya(){
  String hasil;
  if(nilaiSensor == 0){
      hasil = "terang" ;
  } else {
      hasil = "mendung";   
  }
  return hasil;
}

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Contoh Penggunaan Sensor LDR");
  delay(3000);

  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
   // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Bot started up", "");
  
}

void loop() {
 cek();
}
