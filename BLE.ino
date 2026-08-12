#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define LED_PIN 2
#define SERVICE_UUID "f2d90c1e-596a-47ba-811a-7cb5f5d46617"
#define CHARACTERISTIC_UUID "dbb5030e-fb4b-4ef3-9ba8-377ddad5ed5c"

// Class Callback yang berjalan OTOMATIS saat HP mengirim data ke ESP32
class MyCallbacks: public BLECharacteristicCallbacks { // melakukan pewarisan terhadap MyCallbacks terhadap dengan induk BLECharactericCallbacks, sehingga MyCallbacks akan mewarisi semua hal dari  BLECharacteristicCallbacks
    void onWrite(BLECharacteristic *pCharacteristic) { // BLE terdapat event onWrite, onRead, onStatus
      String value = pCharacteristic->getValue(); // Ambil data dari HP

      if (value.length() > 0) {
        Serial.print("Data diterima dari HP: ");
        Serial.println(value);

        // Eksekusi logika berdasarkan perintah
        if (value == "1") {
          analogWrite(LED_PIN, 255);
          Serial.println("-> LED Bawaan Menyala!");
        } else if (value == "0") {
          analogWrite(LED_PIN, 0);
          Serial.println("-> LED Bawaan Mati!");
        }
      }
    }
};
int hitunganRead = 0;

// Class Callback khusus untuk menangani request READ dari HP
class SensorCallbacks: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *pCharacteristic) {
      hitunganRead++;
      
      // Simulasi baca data (misal: hitung berapa kali HP udah minta data)
      String dataKirim = "Data ke-" + String(hitunganRead);

      // Set nilai terbaru ke Characteristic SEBELUM dikirim ke HP
      pCharacteristic->setValue(dataKirim.c_str());

      Serial.print("Event onRead terpicu! Mengirim: ");
      Serial.println(dataKirim);
    }
};
void setup() {
  Serial.begin(115200);
  Serial.println("set up BLE.....");
  //melakukan inisialisasi perangkat 
  BLEDevice::init("ESP_saya");
  // membuat server dengan variabel pointer 
  BLEServer *pServer = BLEDevice::createServer();
  // membuat service 
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // membuat characteristic -- menggunakan uuid unik untuk membedakan masing-masing perangkat 
  // yang memnggunakan BLE
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
  );
// Hubungkan fungsi Callback ke Characteristic
  pCharacteristic->setCallbacks(new SensorCallbacks());
  pCharacteristic->setCallbacks(new MyCallbacks());
  // membuat ini nilai awal
  pCharacteristic->setValue("BLE berhail di-Set up");
  // menjalankan service & advertising 
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // Membantu konektivitas di iPhone
  pAdvertising->setMinPreferred(0x12);  // opsional 
  BLEDevice::startAdvertising();
  Serial.println("SET UP BLE sudah berhasil dilakukan");

}
void loop( ) {
  delay(200);
}
