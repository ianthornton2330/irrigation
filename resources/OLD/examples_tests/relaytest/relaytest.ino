#define RELAY1  2                        
#define RELAY2  3                        
#define RELAY3  4                        
#define RELAY4  5
#define RELAY0  6

void setup()
{
// Initialise the Arduino data pins for OUTPUT
  pinMode(RELAY1, OUTPUT);       
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY0, OUTPUT);
}
 
 void loop()
{
     digitalWrite(RELAY0,HIGH);           // Turns ON Relays 1
   delay(1);                                      // Wait 2 seconds
   digitalWrite(RELAY0,LOW);          // Turns Relay Off
   
   digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
   delay(1);                                      // Wait 2 seconds
   digitalWrite(RELAY1,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY2,LOW);           // Turns ON Relays 2
   delay(1);                                      // Wait 2 seconds
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY3,LOW);           // Turns ON Relays 3
   delay(1);                                      // Wait 2 seconds
   digitalWrite(RELAY3,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY4,LOW);           // Turns ON Relays 4
   delay(1);                                      // Wait 2 seconds
   digitalWrite(RELAY4,HIGH);          // Turns Relay Off

    digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
   delay(3600000);                                      // Wait 2 seconds
   digitalWrite(RELAY1,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY2,LOW);           // Turns ON Relays 2
   delay(3600000);                                      // Wait 2 seconds
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY3,LOW);           // Turns ON Relays 3
   delay(3600000);                                      // Wait 2 seconds
   digitalWrite(RELAY3,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY4,LOW);           // Turns ON Relays 4
   delay(600000);                                      // Wait 2 seconds
   digitalWrite(RELAY4,HIGH);          // Turns Relay Off     
 }
