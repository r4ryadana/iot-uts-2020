// Mengimport library yang akan digunakan
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Mendefinisikan pin komponen ke arduino
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 5
#define Buzzer 8

// Meninisialisasikan library pada pin
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
Servo DoorLock;
MFRC522 rfid(SS_PIN, RST_PIN );

void setup() 
{
  pinMode(Buzzer,OUTPUT); // output buzzer
  DoorLock.attach(SERVO_PIN);
  Serial.begin(9600); // koneksi PC ke arduino
  lcd.begin(16,2); // mengatur ukuran lcd
  SPI.begin();
  rfid.PCD_Init();
}

void loop() 
{
  
  DoorLock.write(0); // Servo mengunci
  lcd.clear();
  lcd.print("TEMPEL KARTU"); // menampilkan tulisan
    
  if ( ! rfid.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! rfid.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  Serial.print("UID tag :"); // menampilkan tulisan
  String content= "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size; i++) // Validasi HEXA kartu
  {
     Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ") // menampilkan tulisan;
     Serial.print(rfid.uid.uidByte[i], HEX); // menampilkan tulisan
     content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("KARTU : "); // menampilkan tulisan
  content.toUpperCase();
  // jika alamat UID kartu dikenal maka menjalankan fungsi pintu
  if (content.substring(1) == "42 E4 14 1E") // Alamat UID Kartu 1
        {
              pintu();                 
        }
  else if (content.substring(1) == "BD 31 15 2B") // Alamat UID Kartu 2
        {
              pintu();                 
        }               
  // jika alamat UID kartu tidak dikenal maka akan mengeluarkan output pada lcd
  else
          {
            Serial.println("\GAK KENAL !"); // menampilkan tulisan
            lcd.clear();
            lcd.print("ANDA TIDAK"); // menampilkan tulisan
            lcd.setCursor(0,1);
            lcd.print("DIKENAL!"); // menampilkan tulisan
            // buzzar menyala secara berulang ulang selama 7 detik dengan jeda setengah detik
            for(int i = 0; i < 7; i++)
            {
              digitalWrite(Buzzer, HIGH); // buzzer menyala
            
              delay(500);
              digitalWrite(Buzzer, LOW); // buzzer mati
              
              delay(500);              
            }
            delay(1000);            
          }                 
}
// ketika alamat UID kartu dikenal maka akan mengeluarkan output pada lcd
void pintu()
  {
            Serial.println("hallo A"); // menampilkan tulisan
            Serial.println();
            lcd.setCursor(0,1);
            lcd.print("SILAKAN MASUK"); // menampilkan tulisan
            delay(3000); 
            // buzzer menyala selama berulang ulang selama 3 detik
            digitalWrite(Buzzer, HIGH); // buzzer menyala
            
              delay(100);
              digitalWrite(Buzzer, LOW); // buzzer mati
              
              delay(100);
  
            // Servo terbuka
            DoorLock.write(180);            
            delay(200);
            lcd.clear();                                    
            lcd.print("Silahkan"); // menampilkan tulisan
            lcd.setCursor(0,1);
            lcd.print("Masuk"); // menampilkan tulisan
            delay(2000);
            lcd.clear();
             digitalWrite(Buzzer, HIGH); // buzzer menyala
            
              delay(500);
              digitalWrite(Buzzer, LOW); // buzzer mati
              
              delay(500);
            // Hitung mundur tutup servo
               for(int i = 5; i > 0; i--)
            {
              lcd.print("Pintu menutup"); // menampilkan tulisan
              lcd.setCursor(0,1);
              lcd.print("dalam "); // menampilkan tulisan
              lcd.print(i);
              delay(1000);
              lcd.clear();
              digitalWrite(Buzzer, HIGH); // buzzer menyala
            
              delay(500);
              digitalWrite(Buzzer, LOW); // buzzer mati
              
              delay(500);
            }
             delay(5000);

            // Servo tertutup
            DoorLock.write(0);
            delay(200);
            lcd.clear();
            lcd.print("Pintu tertutup"); // menampilkan tulisan
            lcd.setCursor(0,1);
            delay(2000);                                   
  }
