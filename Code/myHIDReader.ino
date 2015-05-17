

#define MAX_BITS 100                 // max number of bits 
#define WEIGAND_WAIT_TIME  3000      // time to wait for another weigand pulse.  

unsigned char databits[MAX_BITS];    // stores all of the data bits
unsigned char bitCount;              // number of bits currently captured
unsigned char flagDone;              // goes low when data is currently being captured
unsigned int weigand_counter;        // countdown until we assume there are no more bits

unsigned long facilityCode=0;        // decoded facility code
unsigned long cardCode=0;            // decoded card code

// interrupt that happens when INTO goes low (0 bit)
void ISR_INT0()
{
  Serial.print("0");
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
  
}

// interrupt that happens when INT1 goes low (1 bit)
void ISR_INT1()
{
  Serial.print("1");
  databits[bitCount] = 1;
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
}

void setup()
{
  pinMode(2, INPUT);     // DATA0 (INT0)
  pinMode(3, INPUT);     // DATA1 (INT1)
  
  Serial.begin(9600);
  Serial.println("RFID Readers");
  
  // binds the ISR functions to the falling edge of INTO and INT1
  attachInterrupt(0, ISR_INT0, FALLING);  
  attachInterrupt(1, ISR_INT1, FALLING);
  

  weigand_counter = WEIGAND_WAIT_TIME;
}

void loop()
{
  // This waits to make sure that there have been no more data pulses before processing data
  if (!flagDone) {
    if (--weigand_counter == 0)
      flagDone = 1;	
  }
  
  // if we have bits and we the weigand counter went out
  if (bitCount > 0 && flagDone) 
  {
    unsigned char i;
    
    Serial.print("Read ");
    Serial.print(bitCount);
    Serial.print(" bits. ");
   
    // decode one card
    if (bitCount == 33)
    {      
      // facility code = bits 2 to 7
      for (i=2; i<8; i++)
      {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
      
      // card code = bits 8 to 32
      for (i=8; i<32; i++)
      {
         cardCode <<=1;
         cardCode |= databits[i];
      }
      
      printBits();
    }
   
    else {
      // you can add other formats if you want!
     Serial.println("Unable to decode."); 
    }
    
     // cleanup and get ready for the next card
     bitCount = 0;
     facilityCode = 0;
     cardCode = 0;
     for (i=0; i<MAX_BITS; i++) 
     {
       databits[i] = 0;
     }
     Serial.println("");
     
  }
}

void printBits()
{
      // I really hope you can figure out what this function does
      Serial.print("FC = ");
      Serial.print(facilityCode);
      Serial.print(", CC = ");
      Serial.println(cardCode); 
}
