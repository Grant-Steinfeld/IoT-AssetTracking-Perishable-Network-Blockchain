#include <Adafruit_DHT_Particle.h>
#include <AssetTracker.h>
#define DHTTYPE DHT11
#define DHTPIN D3
DHT dht(DHTPIN, DHTTYPE);
AssetTracker t = AssetTracker();

int ledonboard = D7;

          
SYSTEM_MODE(SEMI_AUTOMATIC);
void setup() {
     
   t.begin();  // Start the DHT sensor
   Serial.begin(9600); 
       
         
  // Subscribe to the integration response event
  Particle.subscribe("hook-response/temperature", myHandler, MY_DEVICES);
  Particle.function("GetCurrTemp",AssetTrackerGetCurrentTemp);
   
   dht.begin();
   // LED pin configuration
   pinMode(ledonboard, OUTPUT);
   // Turn off LED on the Particle board when the application starts
   digitalWrite(ledonboard, LOW);

   Particle.connect();
}   

void myHandler(const char *event, const char *data) {
   // Handle the integration response
     Serial.println(data);
}

float AssetTrackerGetCurrentTemp(String Coordinates) {
     
    float celsius, fahrenheit;
    // Read Celsius and Fahrenheit values from DHT sensor
    celsius    = dht.getTempCelcius();
    fahrenheit = dht.getTempFarenheit();
    // now that we have the readings, we can publish them to the cloud
    // store celsius and fahrenheit temp readings in "temperature" stringified JSON
    String temperature = String::format("{\"Celsius\":%f,\"Fahrenheit\":%f}", celsius, fahrenheit );
    String TempPlusLocation;

    TempPlusLocation = String("{\"Temp\":"+temperature+"}"); 


    Particle.publish("temperature", TempPlusLocation, PRIVATE); // publish to webhook
    Serial.print( "Sending AssetTrackerTemperatureEvent : " );
    Serial.print(TempPlusLocation);
    return fahrenheit;

}


void loop() {
  // Get some data
  digitalWrite(ledonboard, HIGH);
  float ret = AssetTrackerGetCurrentTemp("bahhumbug");
  // Trigger the integration
  //Particle.publish("temperature", data, PRIVATE);
  // Wait 60 seconds
  Serial.println(ret);
  delay(60000);
  digitalWrite(ledonboard, LOW);
}