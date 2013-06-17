//
// J1850 emitter, v1.3
// Mats Ekberg (c) 2013
// 
// This software simulates a data stream from a J1850 compliant 
// OBD2 device. 
//
// http://www.systemconnection.com/downloads/PDFs/OBD-IIJ1850Whitepaper.pdf
//
// Chip:  Arduino Uno at 16MHz
//

//#define PRINT

#define LED_PIN 13


#define MES1_PIN 7
#define MES2_PIN 6
#define MES3_PIN 5

// Timing for start of frame
#define SOF_TIME      200

// Timing for end of frame
#define EOF_TIME      200

// Timing for a short bit
#define SHORTBIT_TIME   64-5

// Timing for a long bit
#define LONGBIT_TIME   128-5

// defclarations for faster bit manipulation than Arduino functions
#define J1850_PIN   8      // digital pin 8
#define PORT        PORTB  // .. on PORT B
#define BITNO       0      // -- bit no 0

#define MASK        0x01 //_BV(BITNO)

//
// Initialization
//
void setup(void) 
{  
  pinMode(J1850_PIN, OUTPUT);
  digitalWrite(J1850_PIN, LOW);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(MES1_PIN, INPUT_PULLUP);
  pinMode(MES2_PIN, INPUT_PULLUP);
  pinMode(MES3_PIN, INPUT_PULLUP);

  delay(2000);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  Serial.println(F("j1850emitter/v1.3"));



}

// passive one => long pulse & low
// active one => short pulse & high
void sendOne() 
{
  uint8_t pin = PORT;
  PORT ^= MASK;
  delayMicroseconds((PORT & MASK) == 0 ? LONGBIT_TIME : SHORTBIT_TIME);
}

// passive zero => short pulse & low
// active zero => long pulse & high
void sendZero() 
{
  uint8_t pin = PORT;
  PORT ^= MASK;
  delayMicroseconds((PORT & MASK) == 0 ? SHORTBIT_TIME : LONGBIT_TIME);
}

void sendSOF() 
{
  digitalWrite(J1850_PIN, HIGH);
  delayMicroseconds(SOF_TIME);
}

void sendEOF() 
{
  digitalWrite(J1850_PIN, LOW);
  delayMicroseconds(EOF_TIME);
}

void sendMessage(uint8_t* message, uint8_t length) 
{
  sendSOF();
  for (uint8_t index = 0; index < length; index++) 
  {
    uint8_t bitc = 8;
    uint8_t thebyte = message[index];
    while (bitc--) 
    {
      if (thebyte & 0x80) { 
        sendOne();
      } 
      else {
        sendZero();
      }
      thebyte <<= 1;
    }
  }
  sendEOF();

  Serial.print(F(">"));
  for (uint8_t index = 0; index < length; index++) {
    uint8_t thebyte = message[index];

    if (thebyte < 16) Serial.print("0");
    Serial.print(thebyte, HEX);
  }
  Serial.println(F("<"));

}

uint8_t ones[] = {
  0xFF, 0xFF};
uint8_t zeros[] = {
  0x00, 0x00};

uint8_t rpm[] = {
  0x28, 0x1b, 0x10, 0x02, 0x10, 0x00};
uint8_t spd[] = {
  0x48, 0x29, 0x10, 0x02, 0x07, 0x00};
uint8_t gear[] = {
  0xa8, 0x3b, 0x10, 0x03, 0x0f};
uint8_t fuel[] = {
  0xa8, 0x83, 0x61, 0x12, 0xd8};
uint8_t temp[] = {
  0xa8, 0x49, 0x10, 0x10, 0x85};
uint8_t turnl[] = {
  0x48, 0xda, 0x40, 0x39, 0x01};
uint8_t turnr[] = {
  0x48, 0xda, 0x40, 0x39, 0x02};



//
// Background loop - print message when available
//
void loop(void)
{
  delay(800);
  digitalWrite(LED_PIN, HIGH);

  uint8_t sel = digitalRead(MES1_PIN) == LOW ? 0x04 : 0x00;
  sel |= digitalRead(MES2_PIN) == LOW ? 0x02 : 0x00;
  sel |= digitalRead(MES3_PIN) == LOW ? 0x01 : 0x00;
  switch (sel) {
  case 0:  
    sendMessage(zeros, sizeof(zeros)); 
    break;
  case 1:  
    sendMessage(ones, sizeof(ones)); 
    break;
  case 2:  
    sendMessage(rpm, sizeof(rpm)); 
    break;
  case 3:  
    sendMessage(spd, sizeof(spd)); 
    break;
  case 4:  
    sendMessage(gear, sizeof(gear)); 
    break;
  case 5:  
    sendMessage(temp, sizeof(temp)); 
    break;
  case 6:  
    break;
  case 7:  
    sendMessage(rpm, sizeof(rpm)); 
    delay(50);
    sendMessage(spd, sizeof(spd)); 
    delay(50);
    sendMessage(gear, sizeof(gear)); 
    delay(50);
    sendMessage(temp, sizeof(temp)); 
    delay(50);
    sendMessage(turnl, sizeof(turnl)); 
    delay(50);
    sendMessage(turnr, sizeof(turnr)); 
    delay(50);
    break;
  }
  digitalWrite(LED_PIN, LOW);
}








