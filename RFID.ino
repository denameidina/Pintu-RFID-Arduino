#include <RFID.h>

/*
  Read a card using a mfrc522 reader on your SPI interface
  Pin layout should be as follows (on Arduino Uno):
  MOSI: Pin 11 / ICSP-4
  MISO: Pin 12 / ICSP-1
  SCK: Pin 13 / ISCP-3
  SS/SDA: Pin 10
  RST: Pin 9
*/

#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9
#define RELAY 4
#define LED_BIRU 7
#define LED_MERAH 6
#define PUSH_BUTTON 5

RFID rfid(SS_PIN, RST_PIN);

int serNum[5];
int cards[][5] = {
  {208, 137, 86, 23, 24},
  {134, 181, 53, 217, 223},
  {198, 229, 45, 39, 41},
  {38, 6, 10, 39, 13}
};

bool access = false;

void setup() {
  Serial.begin(250000);
  SPI.begin();
  rfid.init();

  pinMode(RELAY, OUTPUT);
  pinMode(LED_BIRU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  pinMode(PUSH_BUTTON, INPUT_PULLUP);

  digitalWrite(RELAY, HIGH);
  digitalWrite(LED_BIRU, LOW);
  digitalWrite(LED_MERAH, LOW);
}

void loop() {
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      Serial.print(rfid.serNum[0]);
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.print(rfid.serNum[3]);
      Serial.print(" ");
      Serial.print(rfid.serNum[4]);
      Serial.println("");

      for (int x = 0; x < sizeof(cards); x++) {
        for (int i = 0; i < sizeof(rfid.serNum); i++ ) {
          if (rfid.serNum[i] != cards[x][i]) {
            access = false;
            break;
          } else {
            access = true;
          }
        }
        if (access) break;
      }

    }

    if (access || digitalRead(PUSH_BUTTON) == LOW) {
      Serial.println("Welcome!");
      digitalWrite(LED_BIRU, HIGH);
      digitalWrite(RELAY, LOW);
      delay(5000);
      digitalWrite(LED_BIRU, LOW);
      digitalWrite(RELAY, HIGH);

    } else {
      Serial.println("Not allowed!");
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_MERAH, HIGH);
        delay(500);
        digitalWrite(LED_MERAH, LOW);
        delay(500);
      }
    }
  }
  rfid.halt();

  if (digitalRead(PUSH_BUTTON) == LOW) {
      while(digitalRead(PUSH_BUTTON) == LOW){
        
      }
      digitalWrite(LED_BIRU, HIGH);
      digitalWrite(RELAY, LOW);
      delay(5000);
      digitalWrite(LED_BIRU, LOW);
      digitalWrite(RELAY, HIGH);

    }

}


