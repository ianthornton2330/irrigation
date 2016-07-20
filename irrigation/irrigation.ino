//begin anemometer
//Anemometer Variables - Anemometer requires 7-24v
const int sensorPin = A0; //Defines the pin that the anemometer output is connected to
int sensorValue = 0; //Variable stores the value direct from the analog pin
float sensorVoltage = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float windSpeed = 0; // Wind speed in meters per second (m/s)

float voltageConversionConstant = .00322265625; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 3000; //Delay between sensor readings, measured in milliseconds (ms)

float voltageMin = .4; // Mininum output voltage from anemometer in mV.
float windSpeedMin = 0; // Wind speed in meters/sec corresponding to minimum voltage

float voltageMax = 2.0; // Maximum output voltage from anemometer in mV.
float windSpeedMax = 72.47; // Wind speed in meters/sec corresponding to maximum voltage, tolerance is 0.3m/s tolerant


//begin soil
double soil=0;
double soil2=0;


void setup() {
    Serial.begin(9600);
    Particle.variable("soil", soil);
}

int loopCounter = 0;

void loop() {
    
    loopCounter++;
    /*Begin Hygrometer */

    // read the input on analog pin 0:
    float sensorValue = analogRead(A0);
        sensorValue = constrain(sensorValue, 1192, 4095);
    //sensorValue = 2642;
    float sensorValue2 = analogRead(A1);
        sensorValue2 = constrain(sensorValue2, 1192, 4095);
    //sensorValue2 = 2642;
    // print out the value you read:
    //Serial.println(sensorValue);

    //map the value to a percentage
    // Value read minus 1192 (low value), then divided into 2093 (range between 1192 and 4095) = percentage dry (p). Then 100 - (p) will be percent wet.
    soil = 100-(((sensorValue-1192)/2903)*100);
    soil2 = 100-(((sensorValue2-1192)/2903)*100);

    float result = (soil + soil2) / 2;

    /*Begin Anemometer */

    sensorValue = analogRead(sensorPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer

    sensorVoltage = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage

    //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
    if (sensorVoltage <= voltageMin){
        windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
    }
    else {
        windSpeed = (sensorVoltage - voltageMin)*windSpeedMax/(voltageMax - voltageMin); //For voltages above minimum value, use the linear relationship to calculate wind speed.
    }

    //Publish Results

    if(loopCounter == 6*60*3){
    
    Serial.print(soil);
    Serial.println("%");
    Serial.print(soil2);
    Serial.println("%");
    Particle.publish("SoilLog", (String)result + "%");
    loopCounter = 0;
    }
    
    Serial.print("Voltage: ");
    Serial.print(sensorVoltage);
    Serial.print("Wind speed MPH: ");
    Serial.println(windSpeed);
    Particle.publish("WindSpeed", (String)windSpeed);

    delay(10000);        // delay reads for every 3 hours
}
