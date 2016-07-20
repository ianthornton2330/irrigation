//set APN to Hologram.io rather than Particle
#include "cellular_hal.h" //required to set APN
STARTUP(cellular_credentials_set("apn.konekt.io", "", "", NULL));


//PIN DEFINITIONS
    //inputs (Analog A0-A5)
    const int hygro1Pin = A5;
    const int hygro2Pin = A4;
    const int anemoPin = A0;

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

    String currentPhase = "init";
    
    //hygro values
    double hygro1 = 0; //Variable stores the value direct from the analog pin
    double hygro2 = 0; //Variable stores the value direct from the analog pin
    float hygro1Percent = 0.0;
    float hygro2Percent = 0.0;

    //anemo globals - Anemometer requires 7-24v

    int anemoValue = 0; //Variable stores the value direct from the analog pin
    float anemoVoltage = 0.0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
    float windSpeed = 0.0; // Wind speed in meters per second (m/s)
    float voltageConversionConstant = .00322265625; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
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
        /*if (data)
        Serial.println(data);
        else
        Serial.println("NULL");
        */
    }
      

    int relayTest(String command){
        // Create a nice string with commas between x,y,z
        //String pubAccel = String::format("%d,%d,%d",t.readX(),t.readY(),t.readZ());
        
        if(Particle.publish("Performing Relay Test", "", 60, PRIVATE)){
            
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
            return true;
        }
        else{
            return false;
        }
            
    }
    // --- End IFTTT Subscription ---
    
    
    //make setting LED color, brightness, and times to flash easy.
    void setLED(int red, int green, int blue, int bright, int timesToBlink = 0){
        RGB.color(red,green,blue); //set color
    
        if (timesToBlink > 0){
            for (int count = 0; count < timesToBlink; count++) {
                RGB.brightness(255); //full
                delay(20);
                RGB.brightness(0); //off
                delay(20);
            }
        }
        
        RGB.brightness(bright); //set bright
    }
            


//SETUP

void setup() {
    //take control of RGB
    RGB.control(true);

    //show we're in control
    setLED(255,255,0,255,100); //r,g,b,%,x - flashing red

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
    
    Particle.variable("hygro1", hygro1);
    Particle.variable("hygro2", hygro2);
    Particle.subscribe("IFTTT", IFTTTsentEvent);
    
    //DroneHome has booted. GPS will update every 20 minutes. Battery|
    //String strBootup = String::format("%d min updates. Power: %.2fv, %.2f\%.",delayMinutes,fuel.getVCell(),fuel.getSoC());
    String strBootup = String::format("Power: %.2fv, %.2f\%.",fuel.getVCell(),fuel.getSoC());
    Particle.publish("Booted", strBootup, 60, PRIVATE);
    
    //init complete!
    setLED(0,0,255,255,0); //r,g,b,%,x - full green
}

// LOOP

void loop() {

    loopCounter++;

    float hygro1 = analogRead(hygro1Pin);
    hygro1 = constrain(hygro1, 1192, 4095);
        //hygro1 = 2642;
    float hygro2 = analogRead(hygro2Pin);
    hygro2 = constrain(hygro2, 1192, 4095);
        //hygro2 = 2642;

    // Convert to percentages
    // Value read minus 1192 (low value), then divided into 2093 (range between 1192 and 4095) = percentage dry (p). Then 100 - (p) will be percent wet (moisture percentage).
    hygro1Percent = 100-(((hygro1-1192)/2903)*100);
    hygro2Percent = 100-(((hygro2-1192)/2903)*100);

    float hygroAvg = (hygro1Percent + hygro2Percent) / 2;

    anemoValue = analogRead(anemoPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer
    anemoVoltage = anemoValue * voltageConversionConstant; //Convert sensor value to actual voltage

    //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
    if (anemoVoltage <= anemoVoltMin){
        windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
    }
    else {
        windSpeed = (anemoVoltage - anemoVoltMin) * windSpeedMax / (anemoVoltMax - anemoVoltMin); //For voltages above minimum value, use the linear relationship to calculate wind speed.
    }

    //Publish Results

    if(loopCounter == 6/**60*3*/){ //get to minutes, get to hours, number of hours.
        
        //Every 3 hours, publish hygroAverage
        Serial.print(hygro1);
        Serial.println("%");
        Serial.print(hygro2);
        Serial.println("%");
        Particle.publish("SoilLog", (String)hygroAvg + "%");

        loopCounter = 1;    //reset loop counter
    }
    else if (loopCounter == 0){
        Particle.publish("SoilLog", (String)hygroAvg + "%");           
    }
    
    /*
    Particle.publish("Soil Log", 
    "soil:" + String::format("%.2f",soil) + "\%" + 
    ", soil2:" + String::format("%.2f",soil2) + "\%" + 
    " - soilAvg:" + String::format("%.2f",soilAvg) + "\%",
    60, PRIVATE
    );
    */
  
    Serial.print("Voltage: ");
    Serial.print(anemoVoltage);
    Serial.print("Wind speed MPH: ");
    Serial.println(windSpeed);
    Particle.publish("WindSpeed", (String)windSpeed);


    //ACT UPON CALCULATIONS
    
    digitalWrite(valvePump, LOW); //start pump. pump will stop automatically if/when pressure is achieved.
    
        digitalWrite(valveRow1, LOW); //start row 1
        delay(1000*60*8); //8 minutes
        digitalWrite(valveRow1, HIGH); //stop row 1
        
        digitalWrite(valveRow2, LOW); //start row 2
        delay(1000*60*8); //8 minutes
        digitalWrite(valveRow2, HIGH); //stop row 2
        
        /*
        digitalWrite(valveRow3, LOW); //start row 3 - DISABLED
        delay(10); //future addition
        digitalWrite(valveRow3, HIGH); //stop row 3
        */
        
        //if (windSpeed > 10){  //in the future, this will be used for water conservation
        
            digitalWrite(valveSprinkler, LOW); //start sprinkler
            delay(1000*60*5); //5 minutes
            digitalWrite(valveSprinkler, HIGH); //stop sprinkler
        //}
    
    digitalWrite(valvePump, HIGH); //stop pump

    delay(1000*10);   //loop every 10 seconds
    
    //60*60*11.45
    // loop every 11.45 hours, resuliting in a cycle every 12 hours, ideally.  
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
