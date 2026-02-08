#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Bitmaps for Icons ---
const unsigned char standing_bmp [] PROGMEM = { 0x01, 0x80, 0x03, 0xc0, 0x03, 0xc0, 0x01, 0x80, 0x0f, 0xf0, 0x1d, 0xb8, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80 };
const unsigned char sujud_bmp [] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x0f, 0x80, 0x0f, 0x80, 0x07, 0x00, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// --- Pins ---
const int trigPin = 2;
const int echoPin = 3;
const int btn2 = 4;
const int btn4 = 5;
const int motorPin = 6;

// --- Variables ---
int targetRakat = 0;
int currentRakat = 1;
int currentSujud = 0;
bool isPraying = false;
unsigned long lastSujudTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // INPUT_PULLUP means connect button to GND, no 5V needed!
  pinMode(btn2, INPUT_PULLUP); 
  pinMode(btn4, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
  
  // 1. Intro Screen
  showIntro();
}

void loop() {
  if (!isPraying) {
    // Check buttons (LOW means pressed because of PULLUP)
    if (digitalRead(btn2) == LOW) { 
      targetRakat = 2; 
      startPrayer(); 
    }
    if (digitalRead(btn4) == LOW) { 
      targetRakat = 4; 
      startPrayer(); 
    }
  } else {
    checkSensor();
  }
}

void showIntro() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(15, 20);
  display.println("Assalamu Alaikum");
  
  // Split this long line into two!
  display.setCursor(35, 40); // Centered "My Muslim"
  display.println("My Muslim"); 
  display.setCursor(10, 50); // Changed to 10 so "Brothers & Sisters" fits!
  display.println("Brothers & Sisters");
  display.display();
  delay(6000); // Show for 3 seconds
  
  // Show Menu
  display.clearDisplay();
  display.setCursor(10, 10); display.println("Select Prayer:");
  display.setCursor(10, 30); display.println("[Btn 1] -> 2 Rakat");
  display.setCursor(10, 45); display.println("[Btn 2] -> 4 Rakat");
  display.display();
}

void startPrayer() {
  isPraying = true;
  currentRakat = 1;
  currentSujud = 0;
  lastSujudTime = millis(); // Reset timer so it doesn't count immediately
  updateDisplay();
}

void checkSensor() {
  // Ultrasonic Logic
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = (duration / 2) / 29.1;

  // Logic: Detect Sujud (Close distance < 20cm)
  // Must be at least 4 seconds since last sujud to avoid double counting
  if (distance < 20 && distance > 0 && (millis() - lastSujudTime > 4000)) {
    
    // Confirm it's a real sujud (wait 1s to see if you stay down)
    delay(1000); 
    
    // Haptic Feedback (Bzzzt)
    digitalWrite(motorPin, HIGH); 
    delay(200); 
    digitalWrite(motorPin, LOW);
    
    currentSujud++;
    lastSujudTime = millis();
    updateDisplay();

    // Logic for Rakat Change
    if (currentSujud == 2) {
      // If we hit 2 Sujuds, wait a moment, then go to next Rakat
      delay(2000); // Wait 2s while showing "Sujud 2"
      
      if (currentRakat == targetRakat) {
        finishPrayer();
      } else {
        currentRakat++;
        currentSujud = 0; // Reset sujud for the new Rakat
        updateDisplay();  // Show new Rakat count
      }
    }
  }
}

void updateDisplay() {
  display.clearDisplay();
  
  // Draw Icon: If Sujud 0 (Standing) use Standing Icon, else Sujud Icon
  display.drawBitmap(90, 10, (currentSujud == 0) ? standing_bmp : sujud_bmp, 16, 16, WHITE);
  
  display.setTextSize(1);
  display.setCursor(0, 10); 
  display.print("Rakat: "); display.println(currentRakat);
  
  display.setCursor(0, 30); 
  display.print("Sujud: "); display.println(currentSujud);
  
  display.display();
}

void finishPrayer() {
 display.clearDisplay();
  display.setTextSize(1);
  
  // "Mashallah!" is 10 chars wide (~60px)
  // (128 - 60) / 2 = 34
  display.setCursor(34, 20); 
  display.println("Mashallah!");
  
  // "Habibti" is 7 chars wide (~42px)
  // (128 - 42) / 2 = 43
  display.setCursor(43, 40); 
  display.println("Habibti"); 
  
  // "Allahumma Barik <3" is 18 chars wide (~108px)
  // (128 - 108) / 2 = 10
  display.setCursor(10, 50); 
  display.println("Allahumma Barik <3");
  display.display();
  delay(5000); // Show success for 5 seconds
  
  // Reset everything
  isPraying = false;
  showIntro();
}