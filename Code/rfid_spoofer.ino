//Pin to connect to the circuit
//Setting the pin LOW will tune the coil
//meaning it will respond as a high signal to the reader
//Setting the pin to HIGH will detune coil
//meaning the reader will see it as a low signal
int coil_pin = 9;

void setup()
{
  //Set pin as output
  pinMode(coil_pin, OUTPUT);
    
  //Start it as low
  digitalWrite(coil_pin, LOW);
}

//Does manchester encoding for signal and sets pins.
//Needs clock and signal to do encoding
void set_pin_manchester(int clock_half, int signal)
{
  //manchester encoding is xoring the clock with the signal
  int man_encoded = clock_half ^ signal;
  
  //if it's 1, set the pin LOW (this will tune the antenna and the reader sees this as a high signal)
  //if it's 0, set the pin to HIGH  (this will detune the antenna and the reader sees this as a low signal)
  if(man_encoded == 1)
  {
     //fsk encode
     for (int i=0; i<5;i++)
     {
       digitalWrite(coil_pin, LOW);
       delayMicroseconds(40); 
       digitalWrite(coil_pin, HIGH);
       delayMicroseconds(40); 
     }
  }
  else
  {
     //fsk encode
     for (int i=0; i<6;i++)
     {
       digitalWrite(coil_pin, LOW);
       delayMicroseconds(32); 
       digitalWrite(coil_pin, HIGH);
       delayMicroseconds(32);
     }

  }
}

void loop()
{
  //block 1 
 int fsksync[32]= { 0,0,0,0,0,0,0,0, 1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0};
 
 for (int i=0; i<32;i++)
 {
      if (fsksync[i]==1)
      {
         //fsk encode 1
         for (int i=0; i<5;i++)
         {
            digitalWrite(coil_pin, LOW);
            delayMicroseconds(40); 
            digitalWrite(coil_pin, HIGH);
            delayMicroseconds(40); 
         }
      }
      else 
      {
           //fsk encode 0
        for (int i=0; i<6;i++)
        {
          digitalWrite(coil_pin, LOW);
          delayMicroseconds(32); 
          digitalWrite(coil_pin, HIGH);
          delayMicroseconds(32);
          
        }
      }
 
 } 
  
  //this is the card data we're spoofing.  It's basically 10 hex F's
//  int data_to_spoof[52] =   {  0,0,0,1,1,1,0,1,       0,0,0,0, 0,0,1,0 ,0,0,1,         1,           0,0,1,0,0,0,1,      0,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,1,1,  1 };
                                                             
    int data_to_spoof[52]                                                       
//                               preamble            cardformat             even parity    facilility code 17       card bits match  3699763                odd parity
   for(int i = 0; i < 52; i++)
  {
      set_pin_manchester(0, data_to_spoof[i]);
     // delayMicroseconds(256);
    
      set_pin_manchester(1, data_to_spoof[i]);
      //delayMicroseconds(256); 
  }
}
