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
/*This step initializes the relay pins. However, they start in the ON position, so we are using just 1 millisecond times to
initialize the pin into the off state */

   digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
   delay(1);                                      // Wait 1 milliseconds
   digitalWrite(RELAY1,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY2,LOW);           // Turns ON Relays 2
   delay(1);                                      // Wait 1 milliseconds
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY3,LOW);           // Turns ON Relays 3
   delay(1);                                      // Wait 1 milliseconds
   digitalWrite(RELAY3,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY4,LOW);           // Turns ON Relays 4
   delay(1);                                      // Wait 1 milliseconds
   digitalWrite(RELAY4,HIGH);          // Turns Relay Off

/* Here we are turing the relays on for their allotted times */

    digitalWrite(RELAY1,LOW);           // Turns ON Relays 1
   delay(900000);                                      // Wait 15 mins
   digitalWrite(RELAY1,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY2,LOW);           // Turns ON Relays 2
   delay(900000);                                      // Wait 15 mins
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY3,LOW);           // Turns ON Relays 3
   delay(900000);                                      // Wait 15 mins
   digitalWrite(RELAY3,HIGH);          // Turns Relay Off
 
   digitalWrite(RELAY4,LOW);           // Turns ON Relays 4
   delay(300000);                                      // Wait 5 Minutes
   digitalWrite(RELAY4,HIGH);          // Turns Relay Off 

   digitalWrite(RELAY0,LOW);           // Turns ON Relay 0 - a non existent relay to wait for 23 hours 50 Minutes
   delay(85800000);
   digitalWrite(RELAY0,HIGH);          // Turns OFF Relay 0, the non existent relay
 }
