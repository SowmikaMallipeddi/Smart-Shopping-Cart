#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9

#include <Servo.h>
Servo servo;

int position = 0;

int amount = 0;

int p;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);  // Set the LCD address to 0x27 for a 16x2 display

const int addButtonPin = 3;  // Button to add items
const int removeButtonPin = 4;

int esp_pin = 2;

//String itemList[10];  // Array to store items (max 10 items)
//int itemCount = 0;    // Number of items in the list

void setup() {
  Serial.begin(9600);
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522
  lcd.begin(20,4);  // Initialize the LCD

  servo.attach(6);
  pinMode(esp_pin, INPUT);
  pinMode(addButtonPin, INPUT_PULLUP);  // Enable internal pull-up for the button
  pinMode(removeButtonPin, INPUT_PULLUP);
  lcd.setCursor(0,0);
  lcd.print("SMART TROLLEY - DIET");
  delay(2000);
  
}

void loop() {
  // Check if the add button is pressed
  
  if(digitalRead(esp_pin)==HIGH)
  {
    clearLCDLine(0,1);
    lcd.setCursor(0,1);
    lcd.print("TROLLEY CONNECTED:");
    delay(1000);
  if (digitalRead(addButtonPin) == HIGH) {
    clearLCDLine(0,1);
    lcd.setCursor(0,1);
    lcd.print("ADDING ITEM...");
    delay(1000);

    // Call the scan function
    String tagID = scanRFID();
    if (tagID != "") {
      clearLCDLine(0,1);
      clearLCDLine(0,2);
      lcd.setCursor(0,1);
    lcd.print("ITEM ADDED:");
    clearLCDLine(11,1);
    
    delay(1000);
    addItem(tagID);  // Add the scanned tag to the list
    }
  }
  if (digitalRead(removeButtonPin) == HIGH) {
    clearLCDLine(0,1);
    lcd.setCursor(0,1);
    lcd.print("REMOVING ITEM...");
    delay(1000);
    open_lid();

    // Call the scan function
    String tagID = scanRFID();
    if (tagID != "") {
      clearLCDLine(0,1);
      clearLCDLine(0,2);
      lcd.setCursor(0,1);
    lcd.print("ITEM REMOVED:");
    clearLCDLine(13,1);
    delay(1000);
    removeItem(tagID);  // Add the scanned tag to the list
    }
  }
}
else
{
  clearLCDLine(0,0);
    lcd.setCursor(0,0);
    lcd.print("SMART TROLLEY - DIET");
    delay(1000);
  clearLCDLine(0,1);
    lcd.setCursor(0,1);
    lcd.print("PLEASE SCAN QR CODE:");
    delay(1000);
    clearLCDLine(0,2);
    clearLCDLine(0,3);
  delay(100);
}
}

// Function to scan an RFID tag
String scanRFID() {
  clearLCDLine(0,2);
  lcd.setCursor(0,2);
  lcd.print("PLEASE SCAN...");

  String tagID = "";
  while(true) {
    // Check if a new RFID card is present
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        tagID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        tagID += String(mfrc522.uid.uidByte[i], HEX);
      }
      tagID.toUpperCase();
      mfrc522.PICC_HaltA();  // Stop reading
      break;
    }
  }


  delay(2000);

  return tagID;
}

// Function to add an item to the list
void addItem(String tagID) {
  if(tagID == "E3D70A01")
  {
    clearLCDLine(11,1);
    lcd.setCursor(11,1);
   
    lcd.print("BISCUITS");
    clearLCDLine(0,2);
    
    lcd.setCursor(0,2);
   
    lcd.print("BISCUITS PRICE:");

    clearLCDLine(15,2);
    
    lcd.setCursor(16,2);
  
    p = 50;
    lcd.print(p);
    
 clearLCDLine(0,3);
 lcd.setCursor(0,3);
 lcd.print("Amount:");

 clearLCDLine(7,3);
 lcd.setCursor(8,3);
 amount = amount + p;
 lcd.print(amount);
 
    
  
    delay(1000);
  }
  if(tagID == "BD720A01")
  {
    clearLCDLine(13,1);
    lcd.setCursor(13,1);
   
    lcd.print("CHOCOS");
    delay(1000);


    lcd.setCursor(0,2);
   
    lcd.print("CHOCOS PRICE:");

    clearLCDLine(12,2);
    
    lcd.setCursor(14,2);
 
    p = 100;
    lcd.print(p);
  

     clearLCDLine(0,3);
 lcd.setCursor(0,3);
 lcd.print("Amount:");

 clearLCDLine(7,3);
 lcd.setCursor(8,3);
 amount = amount + p;
 lcd.print(amount);
 
    delay(1000);
  }

  open_lid();
  
}

void removeItem(String tagID) {
 
  if(tagID == "E3D70A01")
  {
    clearLCDLine(13,1);
    lcd.setCursor(13,1);
   
    lcd.print("BISCUITS");
    delay(1000);

    lcd.setCursor(0,2);
   
    lcd.print("BISCUITS PRICE:");

    clearLCDLine(15,2);
    
    lcd.setCursor(16,2);
  
    p = 50;
    lcd.print(p);clearLCDLine(0,3);
 lcd.setCursor(0,3);
 lcd.print("Amount:");

 clearLCDLine(7,3);
 lcd.setCursor(8,3);
 amount = amount - p;
 lcd.print(amount);
 delay(1000);
  }
   if(tagID == "BD720A01")
  {
    clearLCDLine(13,1);
    lcd.setCursor(13,1);
   
    lcd.print("CHOCOS");

    lcd.setCursor(0,2);
   
    lcd.print("CHOCOS PRICE:");

    clearLCDLine(12,2);
    
    lcd.setCursor(14,2);
  
    p = 100;
    lcd.print(p);
    clearLCDLine(0,3);
 lcd.setCursor(0,3);
 lcd.print("Amount:");

 clearLCDLine(7,3);
 lcd.setCursor(8,3);
 amount = amount - p;
 lcd.print(amount);
    delay(1000);
  } 
}

void open_lid()
{
   for (position = 0; position <= 180; position++) 
  { 
    servo.write(position);  // Set position of Servo motor 1
    delay(10);                     
  }

  delay(10000);
  
  // Rotating Servo motor 1 in clockwise from 180 degree to 0 degree
  for (position = 180; position >= 0; position--) 
  { 
    servo.write(position);  // Set position of Servo motor 1
    delay(15);               // Short delay to control the speed
  }
  
}

void clearLCDLine(int p,int line)
{
for(int n = p; n < 20; n++) 
{ // 20 indicates symbols in line. For 2x16 LCD write - 16
lcd.setCursor(n,line);
lcd.print(" ");
}
lcd.setCursor(p,line); // set cursor in the beginning of deleted line
}
