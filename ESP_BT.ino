#include "BluetoothSerial.h"
#define LED 2
BluetoothSerial ESP_BT;

void setup() {
  Serial.begin(115200);
  Serial.println("serial berjalan...");
  ESP_BT.begin("ESP_BT_32");
  pinMode(LED, OUTPUT);
  digitalWrite(LED,HIGH);
  delay(200);
  digitalWrite(LED,LOW);
  // put your setup code here, to run once
}

void loop() {
  if(ESP_BT.available()> 0){
    Serial.print("status BT:");
    Serial.println(ESP_BT.available());
    delay(500);

    String input = ESP_BT.readString();
    input.trim();
    Serial.println(input);
    int PosisiTitikDua = input.indexOf(':');
    Serial.println(PosisiTitikDua);

    if(PosisiTitikDua != -1) {
      String Perangkat = input.substring(0, PosisiTitikDua);
      String aksi = input.substring(PosisiTitikDua+1);
      Serial.println(Perangkat);
      Serial.println(aksi);

      if(Perangkat == "led"){
        if(aksi == "mati"){
          digitalWrite(LED, LOW);
        }else{
          int kecerahan = aksi.toInt();
          analogWrite(LED, kecerahan);
          Serial.println(kecerahan);
          Serial.print(input);
        }

      }
    }

  }
    // put your main code here, to run repeatedly:
}
