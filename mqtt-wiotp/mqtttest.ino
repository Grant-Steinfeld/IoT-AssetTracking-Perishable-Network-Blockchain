#include <MQTT.h>
char *IOT_CLIENT = "d:b0wmtp:ParticleElectron:AssetTracker007";
char *IOT_HOST = "b0wmtp.messaging.internetofthings.ibmcloud.com";
char *IOT_PASSWORD = "IoTBlockchain";
char *IOT_PUBLISH = "iot-2/evt/temp/fmt/json";
char *IOT_USERNAME = "use-token-auth";

int count = 0;

MQTT client( IOT_HOST, 1883, callback );

void setup() {
  Serial.begin( 9600 );

  while( !Serial.available() ) {
    Particle.process();
    //Users/grantsteinfeld/Downloads/mqtt-tls-wiotp/mqtt-tls-wiotp.ino
  }

  client.connect(
    IOT_CLIENT,
    IOT_USERNAME,
    IOT_PASSWORD
  );

  if( client.isConnected() ) {
    Serial.println( "Connected." );
    // client.subscribe( IOT_SUBSCRIBE );
  } else {
    Serial.println( "Connection failed");
  }
}

// DHT
#include <Adafruit_DHT_Particle.h>

// DHT Sensor parameters
// sensor type: [DHT11, DHT22, DHT21, AM2301]
#define DHTTYPE DHT11
// which digital pin for the DHT
#define DHTPIN D3
// DHT Sensor global
DHT dht(DHTPIN, DHTTYPE);

String GetCurrentTemp() {
    float celsius, fahrenheit;

    // Read Celsius and Fahrenheit values from DHT sensor
    celsius    = dht.getTempCelcius();
    fahrenheit = dht.getTempFarenheit();

    // now that we have the readings, we can publish them to the cloud
    // store celsius and fahrenheit temp readings in "temperature" stringified JSON
    String temperature = String::format("{\"Celsius\":%f,\"Fahrenheit\":%f}", celsius, fahrenheit );
    String TempReading;
    TempReading = String("{\"Temp\":"+temperature+"}");
    Serial.print( "Sending MQTT TemperatureEvent : " );
    Serial.println( TempReading );
    return( TempReading );
}


void loop() {
  count = count + 1;

  // String JSON;
  // count = AssetTrackerGetCurrentTemp( JSON );
  // JSON = String::format("{\"temp\":%d}", count);
  String JSON = GetCurrentTemp();
  Serial.println( JSON );
  client.publish( IOT_PUBLISH, JSON);

  client.loop();
  if( client.isConnected() ) {
    Serial.println( "Connected." );
  }

  delay( 10000 );
}

void callback( char* topic, byte* payload, unsigned int length ) {

}
