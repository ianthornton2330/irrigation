// Automatic Irrigation Setup

#define RELAY1  2                        
#define RELAY2  3                        
#define RELAY3  4                        
#define RELAY4  5
#define RELAY5  6
#define RELAY0  7

// Momentary Button for Pump power setup

int inPin = 8;         // the number of the input pin
int outPin = 6;       // the number of the output pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{
// Initialise the Arduino data pins for OUTPUT for RELAYS
  pinMode(RELAY1, OUTPUT);       
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY0, OUTPUT);

// Setup for the Momentary Push Buttons
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);


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
   
   digitalWrite(RELAY5,LOW);           // Turns ON Relays 4
   delay(1);                                      // Wait 1 milliseconds
   digitalWrite(RELAY5,HIGH);          // Turns Relay Off

/* Here we are turing the relays on for their allotted times */

   digitalWrite(RELAY1,LOW);           // Turns ON Relay 1
   digitalWrite(RELAY5,LOW);           // Turns ON Relay 5 (this is the pump)
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
   digitalWrite(RELAY5,HIGH);           // Turns OFF Relay 5 (this is the pump)

   digitalWrite(RELAY0,LOW);           // Turns ON Relay 0 - a non existent relay to wait for 23 hours 50 Minutes
   delay(85800000);
   digitalWrite(RELAY0,HIGH);          // Turns OFF Relay 0, the non existent relay

// Momentary Pump Power Setup

 reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;

    time = millis();    
  }

  digitalWrite(outPin, state);

  previous = reading;

 }
