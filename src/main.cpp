#include <Arduino.h>
#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+
#define N 160  //How many frequencies

int results[N];

void SendData(int data[], int n) {
  byte checksum, LSB, MSB;
  // Serial.write(byte(0)); 
  checksum = 0;
  LSB = lowByte(n) | 0x80; // send low seven bits, with one in high bit to ensure a non-zero byte
  MSB = highByte(n << 1) | 0x80; // send bits 8 to 14, with one in high bit to ensure a non-zero byte
  // Serial.write(LSB); 
  checksum += LSB;
  // Serial.write(MSB); 
  checksum += MSB;
  for (int i = 0; i < n; i++) {
    LSB = lowByte(data[i]) | 0x80; // send low seven bits, with one in high bit to ensure a non-zero byte
    MSB = highByte(data[i] << 1) | 0x80; // send bits 8 to 14, with one in high bit to ensure a non-zero byte
    // Serial.write(LSB); 
    checksum += LSB;
    // Serial.write(MSB); 
    checksum += MSB;
  }
  char decChecksum[4]; // Buffer to store decimal checksum (up to 3 digits + null terminator)
  sprintf(decChecksum, "%d", checksum); // Convert checksum to decimal
  Serial.write(decChecksum); // Send decimal checksum
   Serial.write('\n'); // New line
}

void setup(){
  TCCR1A = 0b10000010;      //-Set up frequency generator
  TCCR1B = 0b00011001;      //-+
  ICR1 = 110;
  OCR1A = 55;

  pinMode(9, OUTPUT);       //-Signal generator pin
  pinMode(8, OUTPUT);       //-Sync (test) pin

  Serial.begin(115200);

  for (int i = 0; i < N; i++) //-Preset results
    results[i] = 0;       //-+
}

void loop(){
  unsigned int d;

  int counter = 0;
  for (unsigned int d = 0; d < N; d++)
  {
    int v = analogRead(0);  //-Read response signal
    CLR(TCCR1B, 0);         //-Stop generator
    TCNT1 = 0;              //-Reload new frequency
    ICR1 = d;               // |
    OCR1A = d / 2;          //-+
    SET(TCCR1B, 0);         //-Restart generator
    results[d] = v;
  }


  SendData(results, N);


  TOG(PORTB, 0);           //-Toggle pin 8 after each sweep (good for scope)
}

