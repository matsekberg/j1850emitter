//
// J1850 emitter, v1
// Mats Ekberg (c) 2013
// 
// This software simulates a data stream from a J1850 compliant 
// OBD2 device. 
//
// Chip:  Arduino Mini Pro w ATmega168 at 16MHz
//

//#define PRINT

#define LED_PIN 13

#define J1850_PIN 8

#define MES1_PIN 7
#define MES2_PIN 6
#define MES3_PIN 5

// Timing for start of frame
#define SOF_TIME      200

// Timing for end of frame
#define EOF_TIME      200

// Timing for a one-bit
#define BITONE_TIME   128-15

// Timing for a zero-bit
#define BITZERO_TIME   64-15


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
  Serial.println(F("j1850emitter.v1"));

}


void sendOne() 
{
  digitalWrite(J1850_PIN, digitalRead(J1850_PIN) == HIGH ? LOW : HIGH );
  delayMicroseconds(BITONE_TIME);
}

void sendZero() 
{
  digitalWrite(J1850_PIN, digitalRead(J1850_PIN) == HIGH ? LOW : HIGH );
  delayMicroseconds(BITZERO_TIME);
}

void sendSOF() 
{
  digitalWrite(J1850_PIN, HIGH);
  delayMicroseconds(SOF_TIME);
}

void sendEOF() 
{
  digitalWrite(J1850_PIN, digitalRead(J1850_PIN) == HIGH ? LOW : HIGH );
  delayMicroseconds(+EOF_TIME);
}

void sendMessage(uint8_t* message, uint8_t length) 
{
  sendSOF();
  for (uint8_t index = 0; index < length; index++) 
  {
    #ifdef PRINT
    Serial.println();
    #endif
    uint8_t bitc = 8;
    uint8_t thebyte = message[index];
    while (bitc--) 
    {
      if (thebyte & 0x80) { 
        sendOne();
        #ifdef PRINT
        Serial.print("1");
        #endif
      } else {
        sendZero();
        #ifdef PRINT
        Serial.print("0");
        #endif
      }
      thebyte <<= 1;
    }
  }
  sendEOF();
  #ifdef PRINT
  Serial.println();
  Serial.println("-----");
  #endif
}

uint8_t ones[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t zeros[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t message[] = {0xFF, 0x00, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};


//
// Background loop - print message when available
//
void loop(void)
{
  delay(300);
  digitalWrite(LED_PIN, HIGH);
  
  if (digitalRead(MES1_PIN) == LOW)
    sendMessage(ones, sizeof(ones));  
  if (digitalRead(MES2_PIN) == LOW)
    sendMessage(zeros, sizeof(zeros));  
  if (digitalRead(MES3_PIN) == LOW)
    sendMessage(message, sizeof(message));  
  digitalWrite(LED_PIN, LOW);
}







