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



void setup() 
{              
  Serial.begin(9600);  //Start the serial connection
}


//Anemometer Variables
const int sensorPin = A5; //Defines the pin that the anemometer output is connected to
int sensorValue = 0; //Variable stores the value direct from the analog pin
float sensorVoltage = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float windSpeed = 0; // Wind speed in meters per second (m/s)

float voltageConversionConstant = .004882814; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 3000; //Delay between sensor readings, measured in milliseconds (ms)

float voltageMin = .4; // Mininum output voltage from anemometer in mV.
float windSpeedMin = 0; // Wind speed in meters/sec corresponding to minimum voltage

float voltageMax = 2.0; // Maximum output voltage from anemometer in mV.
float windSpeedMax = 71.58; // Wind speed in meters/sec corresponding to maximum voltage

double soil=0;
double soil2=0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Particle.variable("soil", soil);
}

void loop() {
    
//Begin Hygrometer
  // read the input on analog pin 0:
  float sensorValue = analogRead(A0);
    sensorValue = constrain(sensorValue, 1192, 4095);
  float sensorValue2 = analogRead(A1);
    sensorValue2 = constrain(sensorValue2, 1192, 4095);
  // print out the value you read:
  //Serial.println(sensorValue);

  //map the value to a percentage
  soil = map(sensorValue, 1192, 4095, 100, 0);
  soil2 = map(sensorValue2, 1192, 4095, 100, 0);

//average the values
    float avg = (soil + soil2)/2;
    
//Begin Anemometer 

sensorValue = analogRead(sensorPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer

sensorVoltage = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage

//Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
 if (sensorVoltage <= voltageMin){
 windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
}
else {
  windSpeed = (sensorVoltage - voltageMin)*windSpeedMax/(voltageMax - voltageMin); //For voltages above minimum value, use the linear relationship to calculate wind speed.
}
 
    
  // print out the soil water percentage you calculated:
     Serial.print(soil);
     Serial.print(soil2);
     Serial.println("%");
     String pubData = String(avg) + "%";
     Particle.publish("SoilLog", pubData);

  delay(10800000);        // delay reads for every 3 hours


 //Print voltage and windspeed to serial
  Serial.print("Voltage: ");
  Serial.print(sensorVoltage);
  Serial.print("\t"); 
  Serial.print("Wind speed MPH: ");
  Serial.println(windSpeed);
  Particle.publish("WindSpeed", windSpeed)

 
 delay(sensorDelay);
}
