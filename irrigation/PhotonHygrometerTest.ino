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
    //sensorValue = 2642;
  float sensorValue2 = analogRead(A1);
   sensorValue2 = constrain(sensorValue2, 1192, 4095);
    //sensorValue2 = 2642;
  // print out the value you read:
  //Serial.println(sensorValue);

  //map the value to a percentage
  //soil = map(sensorValue, 1192, 4095, 0, 100);
  // Value read minus 1192 (low value), then divided into 2093 (range between 1192 and 4095) = percentage dry (p). Then 100 - (p) will be percent wet.
  soil = 100-(((sensorValue-1192)/2903)*100);
  soil2 = 100-(((sensorValue2-1192)/2903)*100);
    

  float result = (soil + soil2) / 2;
 // float avg = result / 2;
 
    Particle.publish("SoilLog", (String)result + "%");

  delay(10800000);        // delay reads for every 3 hours


 

}
