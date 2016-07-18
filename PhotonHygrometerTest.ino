double soil=0;
double soil2=0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Particle.variable("soil", soil);
}

// the loop routine runs over and over again forever:
void loop() {
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
    
  // print out the soil water percentage you calculated:
     Serial.print(soil);
     Serial.print(soil2);
     Serial.println("%");
     String pubData = String(avg) + "%";
     Particle.publish("SoilLog", pubData);


  delay(10000);        // delay in between reads for stability
}


