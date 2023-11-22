#define AREF 5.0
#define VoltageSensorPin A1
void setup()
{
  Serial.begin(115200);
  pinMode(VoltageSensorPin,INPUT);
}
void loop()
{
  float voltage;
  voltage = analogRead(VoltageSensorPin)/1024.0*(float)AREF*5.0;
  Serial.println(voltage,2);
  delay(500);
}
