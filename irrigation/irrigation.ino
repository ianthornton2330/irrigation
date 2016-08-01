// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

//SYSTEM_MODE(SEMI_AUTOMATIC);

//set APN to Hologram.io rather than Particle
#include "cellular_hal.h" //required to set APN
STARTUP(cellular_credentials_set("apn.konekt.io", "", "", NULL));




//PIN DEFINITIONS
    //inputs (Analog A0-A5)
    const int hygro1Pin = A5;
    const int hygro2Pin = A4;
    const int anemoPin = A1;
    
    char Str[9] = "Success!";

    //leds
    const int idleLED = D7;

    //outputs (Digital D0-D4, D7)
    const int valveRow1 = D0;
    const int valveRow2 = D1;
    const int valveRow3 = D2;
    const int valveSprinkler = D3;
    const int valvePump = D4;
    
    //related globals
    bool pumpRunning = false;
    bool debug = false; //kills initial connection and does no publishing of data. enables relaytest.

    // create FuelGauge
    FuelGauge fuel;

    /* Buttons Deprecated for virtual ones
    const int btnValve1 = B0;
    const int btnValve2 = B1;
    const int btnValve3 = B2;
    const int btnSprinkler = B3;
    const int btnPump = B4;
    */


//GLOBALS

    // time loop globals
    const int init = 1;
    const int waterRow1 = 2;
    const int waterRow2 = 3;
    const int waterRow3 = 4;
    const int waterSprinklers = 5;
    const int idle = 100;
    const char cycleEnded = '1';

    int currentPhase = init;
    int newPhase = 0;
    int nextPhase = 0;
    int lastPhase = 0;
    int phaseLength = 0;
    int phaseStartedAt = 0;
    int currentTime = 0;  

    //hygro values
    double hygro1 = 0; //Variable stores the value direct from the analog pin
    double hygro2 = 0; //Variable stores the value direct from the analog pin
    float hygro1Percent = 0.0;
    float hygro2Percent = 0.0;

    //anemo globals - Anemometer requires 7-24v

    int anemoValue = 0; //Variable stores the value direct from the analog pin
    float anemoVoltage = 0.0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
    float windSpeed = 0.0; // Wind speed in meters per second (m/s)
    float voltageConversionConstant = 0.0008; //This constant maps the value provided from the analog read function, which ranges from 0 to 4095, to actual voltage, which ranges from 0V to 2V
    int anemoDelay = 3000; //Delay between sensor readings, measured in milliseconds (ms)

    float anemoVoltMin = 0.4; // Mininum output voltage from anemometer in mV.
    float windSpeedMin = 0.0; // Wind speed in meters/sec corresponding to minimum voltage
    
    float anemoVoltMax = 2.0; // Maximum output voltage from anemometer in mV.
    float windSpeedMax = 72.47; // Wind speed in meters/sec corresponding to maximum voltage, tolerance is 0.3m/s tolerant

    //loop and logic globals
    double loopCounter = 0;



//SUBSCRIPTIONS and GLOBAL FUNCTIONS

    //--- Begin IFTTT Subscription ---
    
    void IFTTTsentEvent(const char *event, const char *data){
        Particle.publish("Received event from IFTTT", "Event: " + (String)event + ", Data: " + (String)data, 60, PRIVATE); 
              
        if ((String)event == "IFTTT-relay"){
            relayTest("test");
        }
        if (data)
        Serial.println(data);
        else
        Serial.println("NULL");
        
    }
      

    bool relayTest(String command){
        // Create a nice string with commas between x,y,z
        //String pubAccel = String::format("%d,%d,%d",t.readX(),t.readY(),t.readZ());
            Serial.print("Starting relay test.");
            
            digitalWrite(valveRow1, LOW);
            delay(200);
            digitalWrite(valveRow1, HIGH);
            digitalWrite(valveRow2, LOW);
            delay(200);
            digitalWrite(valveRow2, HIGH);
            digitalWrite(valveRow3, LOW);
            delay(200);
            digitalWrite(valveRow3, HIGH);
            digitalWrite(valveSprinkler, LOW);
            delay(200);
            digitalWrite(valveSprinkler, HIGH);
            digitalWrite(valvePump, LOW);
            delay(200);
            digitalWrite(valvePump, HIGH);
            Serial.print("done.");
            return true;
            
    }
    // --- End IFTTT Subscription ---
    
    
    //make setting LED color, brightness, and times to flash easy.
    void setLED(int red, int green, int blue, int bright, int timesToBlink = 0){
        RGB.color(red,green,blue); //set color
    
        if (timesToBlink > 0){
            for (int count = 0; count < timesToBlink; count++) {
                RGB.brightness(255); //full
                delay(40);
                RGB.brightness(0); //off
                delay(40);
            }
        }
        
        RGB.brightness(bright); //set bright
    }
            
    //control functions
    void row1Start(){
        digitalWrite(valveRow1, LOW); //start row 1
    }

    void row1Stop(){
        digitalWrite(valveRow1, HIGH); //stop row 1  
    }

    void row2Start(){
        digitalWrite(valveRow2, LOW); //start row 2  
    }

    void row2Stop(){
        digitalWrite(valveRow2, HIGH); //stop row 1
    }

    void sprinklerStart(){
        digitalWrite(valveSprinkler, LOW); //start sprinkler
    }

    void sprinklerStop(){
        digitalWrite(valveSprinkler, HIGH); //start sprinkler  
    }

    void allIrrigationStop(){
        digitalWrite(valveRow1, HIGH); //stop row 1
        digitalWrite(valveRow2, HIGH); //stop row 1
        digitalWrite(valveSprinkler, HIGH); //start sprinkler
        digitalWrite(valvePump, HIGH); //stop pump
    }

    void pumpStart(){
        digitalWrite(valvePump, LOW); //stop pump
    }

    void pumpStop(){
        digitalWrite(valvePump, HIGH); //stop pump
    }

    float getHygroAvg(){
        float hygro1 = analogRead(hygro1Pin);
        hygro1 = constrain(hygro1, 1192, 4095);
            //hygro1 = 2642;  //testing integer
        float hygro2 = analogRead(hygro2Pin);
        hygro2 = constrain(hygro2, 1192, 4095);
            //hygro2 = 2642;  //testing integer

        // Convert to percentages
        // Value read minus 1192 (low value), then divided into 2093 (range between 1192 and 4095) = percentage dry (p). Then 100 - (p) will be percent wet (moisture percentage).
        hygro1Percent = 100-(((hygro1-1192)/2903)*100);
        hygro2Percent = 100-(((hygro2-1192)/2903)*100);

        float hygroAvg = (hygro1Percent + hygro2Percent) / 2;
        return hygroAvg;
    }

    float getWindspeed(){
        anemoValue = analogRead(anemoPin); //Get a value between 0 and 4095 from the analog pin connected to the anemometer
        
        anemoValue = constrain(anemoValue, 496, 2482);

        anemoVoltage = anemoValue * voltageConversionConstant; //Convert sensor value to actual voltage
        
        //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
        if (anemoVoltage <= anemoVoltMin){
            windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
        }
        else {
            windSpeed = (anemoVoltage - anemoVoltMin) * (windSpeedMax / (anemoVoltMax - anemoVoltMin)); //For voltages above minimum value, use the linear relationship to calculate wind speed.
        }

        /* The math here works in that: minVoltage .4V == minWind 0 MPH == pinRead 496, and maxVoltage 2V == maxWind 72.47 MPH == pinRead 2482
            given that (pinRead * 3.3V)/4095 = pinVoltage, and (pinVoltage * 4095) / 3.3V = pinRead*/
        /*anemoValue = constrain(anemoValue, 496, 2482);
        windSpeed = map(anemoValue, 496, 2482, 0, 72.47);*/
        return windSpeed;
    }

//SETUP

void setup() {
    //take control of RGB
    RGB.control(true);

    //show we're in control
    setLED(255,0,255,255,100); //r,g,b,%,x - flashing magenta

    //init up indicator LED, not on though.
    pinMode(idleLED, OUTPUT);
    
    //set up sensor pins
    pinMode(hygro1Pin, INPUT_PULLDOWN);
    pinMode(hygro2Pin, INPUT_PULLDOWN);
    //pinMode(hygro3Pin, INPUT_PULLDOWN);
    //pinMode(hygro4Pin, INPUT_PULLDOWN);
    //pinMode(hygro5Pin, INPUT_PULLDOWN);
    pinMode(anemoPin, INPUT_PULLDOWN);

    //set up relays
    pinMode(valveRow1, OUTPUT);
    pinMode(valveRow2, OUTPUT);
    pinMode(valveRow3, OUTPUT);
    pinMode(valveSprinkler, OUTPUT);
    pinMode(valvePump, OUTPUT);

    //init relays to the open state
    digitalWrite(valveRow1, HIGH);
    digitalWrite(valveRow2, HIGH);
    digitalWrite(valveRow3, HIGH);
    digitalWrite(valveSprinkler, HIGH);
    digitalWrite(valvePump, HIGH);

    Serial.begin(9600);
    
    /*if (!(debug)){
        //Particle.connect();
    }*/

    if (Particle.connected() == true){
        Particle.variable("hygro1", hygro1);
        Particle.variable("hygro2", hygro2);
        //Particle.subscribe("IFTTT", IFTTTsentEvent);
        
        //DroneHome has booted. GPS will update every 20 minutes. Battery|
        //String strBootup = String::format("%d min updates. Power: %.2fv, %.2f\%.",delayMinutes,fuel.getVCell(),fuel.getSoC());
        String strBootup = String::format("Power: %.2fv, %.2f\%.",fuel.getVCell(),fuel.getSoC());
        Particle.publish("Booted", strBootup, 60, PRIVATE);
    }

    
    //init complete!
    setLED(0,255,0,255,0); //r,g,b,%,x - full green


}




// blah blah code
//rtc.now();
//rtc.begin(&UDPClient, "au.pool.ntp.org");
//rtc.setTimeZone(+11); // gmt offset
//Time.setTime(rtc.nowEpoch());
//System.reset()
//Time.now() //syncs on startup and on any call of Time.syncTime();
//Spark.connected()

/*// reset the system after 60 seconds if the application is unresponsive
ApplicationWatchdog wd(60000, System.reset);

void loop() {
  while (some_long_process_within_loop) {
    wd.checkin(); // resets the AWDT count
  }
}
// AWDT count reset automatically after loop() ends*/



// LOOP

void loop() {

    if(debug){
        relayTest("test");
    }
    
    /*if((!(debug)) && ((int)Time.now() >= phaseStartedAt + phaseLength)){ //if not debugging, and if this phase is over,
        phaseStartedAt = Time.now();
        //it's time to switch phase
        switch (currentPhase){
            // case = old state
            case init:
                lastPhase = init;
                newPhase = waterRow1;
                //actions
                pumpStart();
                row1Start();
                Particle.publish("irrigationRow1", (String)Str[9]);
                phaseLength = 60*6;     //run for 6 minutes
                nextPhase = waterRow2;
                currentPhase = newPhase;
                setLED(255,255,0,255,10); //r,g,b,%,x - flash yellow 10x
                break;
            case waterRow1:
                lastPhase = waterRow1;
                newPhase = waterRow2;
                //actions
                pumpStart(); //not necessary
                row1Stop();
                row2Start();
                phaseLength = 60*6;     //run for 6 minutes
                nextPhase = waterSprinklers;
                currentPhase = newPhase;
                setLED(255,150,0,255,10); //r,g,b,%,x - flash orange? 10x
                break;
            case waterRow2:
                lastPhase = waterRow2;
                newPhase = waterSprinklers;
                //actions
                pumpStart(); //not necessary
                row2Stop();
                sprinklerStart();
                phaseLength =  60*5;     //run for 5 minutes
                nextPhase = idle;
                currentPhase = newPhase;
                setLED(255,75,0,255,10); //r,g,b,%,x - flash red-orange? 10x
                break;
            //case "waterRow3":
                //do stuff
                //break;
            case waterSprinklers:
                lastPhase = waterSprinklers;
                newPhase = init;
                //actions
                sprinklerStop();
                pumpStop();
                Particle.publish("irrigationRunLog", (String)Str[9]);
                phaseLength = 60*60*11.7;
                nextPhase = init; //not being used but should be
                currentPhase = newPhase;
                setLED(0,255,0,255,10); //r,g,b,%,x - flash green 10x
                break;
            default:
                //this should never happen UNLESS phase == "" or on error
                setLED(0,255,255,255,0); //r,g,b,%,x - solid cyan
                allIrrigationStop(); 
        }
    } */

    //Publish Results
    
    /*if (!(debug)){

        if(loopCounter == 30*60*3){ //minutes, hours, number of hours. every 3 hours, publish soil info
            
            //Every 3 hours, publish hygroAverage
            // Serial.print(hygro1);
            // Serial.println("%");
            //Serial.print(hygro2);
            //Serial.println("%");
            Particle.publish("SoilLog", (String)getHygroAvg() + "%");
            Particle.syncTime();
            loopCounter = 1;    //reset loop counter
        }
        else if (loopCounter == 0){
            //first time only
            Particle.publish("SoilLog", (String)getHygroAvg() + "%");           
        }
    }   
    */
    if (!(debug)){
        
        if((int)loopCounter % 30*30 == 0){ //minutes, * 30.    //old = 5. every 5 loops, so 10 seconds
            Particle.publish("windSpeed", (String)getWindspeed() + " MPH", 60, PRIVATE);
            //Particle.publish("AnemoVoltage", (String)anemoVoltage);
            //Particle.publish("AnemoPinReading", (String)anemoValue);
        }
        else if (loopCounter == 0){
            //first time only
            Particle.publish("windSpeed", (String)getWindspeed() + " MPH", 60, PRIVATE);
        }    
    }    

   /* Particle.publish("Time", 
    "Time.Now is currently " + Time.now(),  //+ String::format("%.2f",soil) + "\%" + 
   // ", soil2:" + String::format("%.2f",soil2) + "\%" + 
    //" - soilAvg:" + String::format("%.2f",soilAvg) + "\%",
    60, PRIVATE
    ); 
    */
  
    // Serial.print("Voltage: ");
    // Serial.print(anemoVoltage);
    // Serial.print("Wind speed MPH: ");
    // Serial.println(windSpeed);

    //every 5 loops, publish anemo readings



    
    loopCounter++;

    setLED(0,255,255,255,5); //r,g,b,%,x - flash cyan 5x
    delay(1000*2);   //wait 2 seconds between loops
}











// extra crap for Particle functions


// Lets you remotely check the battery status by calling the function "batt"
// Triggers a publish with the info (so subscribe or watch the dashboard)
// and also returns a '1' if there's >10% battery left and a '0' if below
int batteryStatus(String command){
    // Publish the battery voltage and percentage of battery remaining
    // if you want to be really efficient, just report one of these
    // the String::format("%f.2") part gives us a string to publish,
    // but with only 2 decimal points to save space
    Particle.publish("Battery Status", 
          "voltage:" + String::format("%.2f",fuel.getVCell()) + 
          ",charge:" + String::format("%.2f",fuel.getSoC()) + "\%",
          60, PRIVATE
    );

    if(fuel.getSoC()>10){
        return 1;
    } 
    // if you're running out of battery, return 0
    else {
        return 0;
    }
}

/*
void loop()
{
  // blink the LED as long as the button is pressed
  while(digitalRead(button) == HIGH) {
    digitalWrite(LED, HIGH);          // sets the LED on
    delay(200);                       // waits for 200mS
    digitalWrite(LED, LOW);           // sets the LED off
    delay(200);                       // waits for 200mS
  }
}*/



