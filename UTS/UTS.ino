//blynk
#define BLYNK_TEMPLATE_ID "TMPLv1FbAHhw"
#define BLYNK_DEVICE_NAME "banjircoba"
#define BLYNK_AUTH_TOKEN "O0HLiiuj2JIS5M7Y9dUcuPd_BL4e7Gk0"

//blynk
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Replace with your network credentials
char ssid[] = "De-72";
char pass[] = "gakdisandi";


#define sensorLDR D6
#define triggerPin  D8
#define echoPin     D7

#include <dht.h>
#define sensor D5
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

//// Initialize Telegram BOT
#define BOTtoken "5143804909:AAEF4UDID0sfHaCLFWuZQHAqxsRs0ooAATw"  // your Bot Token (Get from Botfather)
//
//// Use @myidbot to find out the chat ID of an individual or a group
//// Also note that you need to click "start" on a bot before it can
//// message you
#define CHAT_ID "821934636"
#define CHAT_Aula "1034450435"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

dht DHT;
BlynkTimer timer;
String hum = "Kelembaban : ";
String temp = "Suhu : ";
String light = "";
String pesan = "";
long duration, jarak;

void cek() {
  DHT.read11(sensor);

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;

}

void cahaya(){
  if(analogRead(sensorLDR)<1000){
    light = "Terang";
  }else{
    light = "Mendung";
  }
}

void sendBlynk(){
  if (jarak < 30){
    if((DHT.temperature <= 25 && DHT.humidity >= 60) && light.equalsIgnoreCase("Mendung")){
      pesan = "Waspada air meningkat dan berpotensi curah hujan tinggi";
    } 
    else if((DHT.temperature > 25 && DHT.temperature < 29) && (DHT.humidity < 60 && light.equalsIgnoreCase("Terang"))) {
      pesan = "Hati-hati debit air meningkat, namun cuaca terang";
    }
    else {
      pesan = "Waspada air belum menguap";
    }
  }
   else if(light.equalsIgnoreCase("Mendung")) {
      pesan = "Kondisi aman, namun cuaca mendung";
  } else {
      pesan = "Kondisi aman";
  }
    
  String hasilTemp = "Suhu: " + String(DHT.temperature)+ " °C";
  String hasilHum = "Kelembaban: " + String(DHT.humidity)+ " %";
  String hasilLDR = "Cuaca: " +  light + " (" + analogRead(sensorLDR) + ")";
  String hasilJrk = "Jarak: " + String(jarak)+" Cm";

  Serial.println(hasilTemp);
  Serial.println(hasilHum);
  Serial.println(hasilLDR);
  Serial.println(hasilJrk);
  
  Serial.println(pesan);
  Blynk.virtualWrite(V0, analogRead(sensorLDR));
  Blynk.virtualWrite(V4, DHT.humidity);
  Blynk.virtualWrite(V5, DHT.temperature);
  Blynk.virtualWrite(V6, light);
  Blynk.virtualWrite(V7, jarak);
  Blynk.virtualWrite(V1, pesan);

//  bot.sendMessage(CHAT_ID, hasilTemp, "");
//  bot.sendMessage(CHAT_ID, hasilHum, "");
//  bot.sendMessage(CHAT_ID, hasilLDR, "");
//  bot.sendMessage(CHAT_ID, hasilJrk, "");
//  bot.sendMessage(CHAT_ID, pesan, "");
  
  bot.sendMessage(CHAT_Aula, hasilTemp, "");
  bot.sendMessage(CHAT_Aula, hasilHum, "");
  bot.sendMessage(CHAT_Aula, hasilLDR, "");
  bot.sendMessage(CHAT_Aula, hasilJrk, "");
  bot.sendMessage(CHAT_Aula, pesan, "");
}

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
//  Serial.println("Contoh Penggunaan Sensor LDR");
  delay(3000);

  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
   // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   Blynk.begin(auth, ssid, pass);

  // Setup a timer function to be called every second
  timer.setInterval(1000L, cek);
  
  bot.sendMessage(CHAT_ID, "Bot started up", "");
  
}

void loop() {
 cahaya();
 cek();
 sendBlynk();
}
