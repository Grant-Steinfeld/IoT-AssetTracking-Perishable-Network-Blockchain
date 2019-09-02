// Library dependencies to be added to the Particle Simple project.properties
// dependencies.Adafruit_LIS3DH=1.0.3
// dependencies.Adafruit_GPS=1.0.3
// dependencies.AssetTracker=0.0.10
// dependencies.CellularHelper=0.0.4
// dependencies.Adafruit_Sensor=1.0.2
// dependencies.Adafruit_DHT_Particle=0.0.2
// dependencies.google-maps-device-locator=0.0.4
#include <google-maps-device-locator.h>
#include <AssetTracker.h>

//Temparature and humidity Sensor 
#include <Adafruit_DHT_Particle.h>
// DHT Sensor parameters
// sensor type: [DHT11, DHT22, DHT21, AM2301]
#define DHTTYPE DHT11
// which digital pin for the DHT
// digtial pin 3
#define DHTPIN D3
// DHT Sensor global
DHT dht(DHTPIN, DHTTYPE);



// Adafruit LIS3DH Triple Axis Accelerometer on the Particle Asset Tracker v2 PCB
// Accelerometer Threshold to trigger a publish
// 9000 is very sensitive, 12000 will detect small bumps
int AccelThreshold = 12000;
String MaxAccelJSON;
int MaxAccelThisInterval = 0;
// Creating an AssetTracker named 't'
GoogleMapsDeviceLocator locator;
AssetTracker t = AssetTracker();
int ledonboard = D7;  // Light the onboard Particle LED when in Tracking mode
void setup() {
   // Sets up all the necessary AssetTracker bits
   t.begin();
   Serial.begin(9600);
   // Declare a Particle.function so that we can adjust the Asset Tracking on and off reporting interval from the cloud.
   Particle.function("SetInterval",AssetTrackerSetReportInterval);
   // Declare a Particle.function so that we can query the current temperature from the cloud.
   Particle.function("GetCurrTemp",AssetTrackerGetCurrentTemp);
   // Declare a Particle.function so that we can adjust the accelerometer threshold from the cloud.
   Particle.function("SetXYZThresh",AssetTrackerSetAccelThresh);
   // Declare a Particle.function so that we can query recent accelerometer data from the cloud.
   Particle.function("GetRecentXYZ",AssetTrackerGetRecentAccel);
   // Set the returned location handler function by the locationCallback() method
   // Initialize the Asset Tracker to check in once a Day 60*60*24
   locator.withSubscribe(AssetTrackerLocationCallback).withLocatePeriodic(86400);
   // Give this Electron a name so we can identify it
   //  locator.withEventName("blockchain-iot-asset-tracker1");
   // Start the DHT sensor
   dht.begin();
   // LED pin configuration
   pinMode(ledonboard, OUTPUT);
   // Turn off LED on the Particle board when the application starts
   digitalWrite(ledonboard, LOW);
}
// Remotely change the accelerometer trigger threshold
int AssetTrackerSetAccelThresh( String command ) {
    // Try to convert Srting to an integer
    int NewAccelThreshold = command.toInt();
    // Disgard any non-integer command strings sent from the cloud
    if ( NewAccelThreshold > 0) {
        AccelThreshold = NewAccelThreshold;
        Serial.print("Accelerometer Threshold now set to : ");
        Serial.println(command);
        return 1;
    }
    // Keep the predefined Threshold if function received garbage
    return 0;
}
int AssetTrackerSetReportInterval( String command ) {
    /* Particle.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, sending the function a string that contains a Number will set the AssetTracker delay
    and telling it "off", 0 or some bogus command will turn the AssetTracker off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return :
        New Delay value - when the Cellular Asset Tracker is turning on
        0 - when the Cellular Asset Tracker is turning off
    */
    int Delay = command.toInt();
    if( Delay > 0 ) {
        locator.withSubscribe(AssetTrackerLocationCallback).withLocatePeriodic(Delay);
        digitalWrite(ledonboard,HIGH);
        Serial.print("Enabling Asset Location reporting interval:");
        Serial.println(command);
        return Delay;
    } else {
      // Any invalid string that doesn't convert to a number or 0 should be considered "Off"
      // "once a day" to be sufficiently large to be OFF
        locator.withSubscribe(AssetTrackerLocationCallback).withLocatePeriodic(86400);
        digitalWrite(ledonboard,LOW);
        Serial.println("Disabling Asset Location reporting");
        return 0;
    }
}
int AssetTrackerGetCurrentTemp(String Coordinates ) {
    float celsius, fahrenheit;
    // Read Celsius and Fahrenheit values from DHT sensor
    celsius    = dht.getTempCelcius();
    fahrenheit = dht.getTempFarenheit();
    // now that we have the readings, we can publish them to the cloud
    // store celsius and fahrenheit temp readings in "temperature" stringified JSON
    String temperature = String::format("{\"Celsius\":%f,\"Fahrenheit\":%f}", celsius, fahrenheit );
    String TempPlusLocation;
    if( Coordinates.length() != 0) {
        TempPlusLocation = String("{\"Temp\":"+temperature+",\"gps\":"+Coordinates+"}");
    } else {
        TempPlusLocation = String("{\"Temp\":"+temperature+"}"); // This was a web query, location unknown
    }
    Particle.publish("AssetTrackerTemperatureEvent", TempPlusLocation, PRIVATE); // publish to cloud
    Serial.print( "Sending AssetTrackerTemperatureEvent : " );
    Serial.println( TempPlusLocation );
    return (int)fahrenheit ;
}
int AssetTrackerGetRecentAccel( String Coordinates ) {
    // Check if there's been a big acceleration
    // Report the largest acceleration detected during the prior time interval
    if ( MaxAccelThisInterval ) {
        String AccelPlusLocation;
        if( Coordinates.length() != 0) {
            AccelPlusLocation = String("{\"Accel\":"+MaxAccelJSON+",\"gps\":"+Coordinates+"}");
        } else {
            AccelPlusLocation = String("{\"Accel\":"+MaxAccelJSON+"}"); // This was a web query, location unknown
        }
        Particle.publish("AssetTrackerAccelerationEvent", AccelPlusLocation, 60, PRIVATE);
        Serial.print( "Sending AssetTrackerAccelerationEvent : " );
        Serial.println( AccelPlusLocation );
    }
    // Report the Max Acceleration back to Particle Function return code
    // Reset the Max Acceleration for the next time interval to zero
    int reportMaxAccel = MaxAccelThisInterval;
    MaxAccelThisInterval = 0;
    return reportMaxAccel;
}
void AssetTrackerLocationCallback(float lat, float lon, float accuracy) {
    // Handle the returned location data for the device. This method is passed three arguments:
    // - Latitude
    // - Longitude
    // - Accuracy of estimated location (in meters)
    String Coordinates;
    Coordinates = String::format("{\"lat\":%f,\"lon\":%f,\"accuracy\":%d}", lat, lon, accuracy);
    // Check here if the Particle Asset Tracker v2 built-in GPS can get a more accurate geolocation
    // than the Cellular Tower Triangulation. Consumes more power but improves accuracy.
    // Report the temperture at this location.
    AssetTrackerGetCurrentTemp( Coordinates );
    // Determine if there has been a big acceleration event in the past interval
    AssetTrackerGetRecentAccel( Coordinates );
}
void loop() {
    locator.loop();
    // Check if there's been a big acceleration
    // Save only the largest acceleration detected during the prior time period
    int CurrentAccelmagnitude = t.readXYZmagnitude();
    if (CurrentAccelmagnitude > AccelThreshold && CurrentAccelmagnitude > MaxAccelThisInterval) {
        // Format a JSON Object to be parsed in the cloud {x:X,y:Y,z:Z}
          // Save a stringified JSON object
        MaxAccelJSON = String::format("{\"x\":%d,\"y\":%d,\"z\":%d}", t.readX(), t.readY(), t.readZ());
        Serial.print( "AccelerationEvent triggered: " );
        Serial.println( MaxAccelJSON );
        MaxAccelThisInterval = CurrentAccelmagnitude;
    }
}