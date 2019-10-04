#include <MQTT-TLS.h>
#include <ArduinoJson.h>
#include "math.h"
#include <Adafruit_DHT_Particle.h>

char *IOT_CLIENT = "d:b0wmtp:ParticleElectron:AssetTracker007";
char *IOT_HOST = "b0wmtp.messaging.internetofthings.ibmcloud.com";
char *IOT_PASSWORD = "IoTBlockchain";
char *IOT_PUBLISH = "iot-2/evt/temp/fmt/json";
char *IOT_USERNAME = "use-token-auth";

#define WATSON_IOT_PLATFORM_CA_PEM                                      \
"-----BEGIN CERTIFICATE----- \r\n"                                      \
"MIIElDCCA3ygAwIBAgIQAf2j627KdciIQ4tyS8+8kTANBgkqhkiG9w0BAQsFADBh\r\n"  \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n"  \
"QTAeFw0xMzAzMDgxMjAwMDBaFw0yMzAzMDgxMjAwMDBaME0xCzAJBgNVBAYTAlVT\r\n"  \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxJzAlBgNVBAMTHkRpZ2lDZXJ0IFNIQTIg\r\n"  \
"U2VjdXJlIFNlcnZlciBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\r\n"  \
"ANyuWJBNwcQwFZA1W248ghX1LFy949v/cUP6ZCWA1O4Yok3wZtAKc24RmDYXZK83\r\n"  \
"nf36QYSvx6+M/hpzTc8zl5CilodTgyu5pnVILR1WN3vaMTIa16yrBvSqXUu3R0bd\r\n"  \
"KpPDkC55gIDvEwRqFDu1m5K+wgdlTvza/P96rtxcflUxDOg5B6TXvi/TC2rSsd9f\r\n"  \
"/ld0Uzs1gN2ujkSYs58O09rg1/RrKatEp0tYhG2SS4HD2nOLEpdIkARFdRrdNzGX\r\n"  \
"kujNVA075ME/OV4uuPNcfhCOhkEAjUVmR7ChZc6gqikJTvOX6+guqw9ypzAO+sf0\r\n"  \
"/RR3w6RbKFfCs/mC/bdFWJsCAwEAAaOCAVowggFWMBIGA1UdEwEB/wQIMAYBAf8C\r\n"  \
"AQAwDgYDVR0PAQH/BAQDAgGGMDQGCCsGAQUFBwEBBCgwJjAkBggrBgEFBQcwAYYY\r\n"  \
"aHR0cDovL29jc3AuZGlnaWNlcnQuY29tMHsGA1UdHwR0MHIwN6A1oDOGMWh0dHA6\r\n"  \
"Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RDQS5jcmwwN6A1\r\n"  \
"oDOGMWh0dHA6Ly9jcmw0LmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RD\r\n"  \
"QS5jcmwwPQYDVR0gBDYwNDAyBgRVHSAAMCowKAYIKwYBBQUHAgEWHGh0dHBzOi8v\r\n"  \
"d3d3LmRpZ2ljZXJ0LmNvbS9DUFMwHQYDVR0OBBYEFA+AYRyCMWHVLyjnjUY4tCzh\r\n"  \
"xtniMB8GA1UdIwQYMBaAFAPeUDVW0Uy7ZvCj4hsbw5eyPdFVMA0GCSqGSIb3DQEB\r\n"  \
"CwUAA4IBAQAjPt9L0jFCpbZ+QlwaRMxp0Wi0XUvgBCFsS+JtzLHgl4+mUwnNqipl\r\n"  \
"5TlPHoOlblyYoiQm5vuh7ZPHLgLGTUq/sELfeNqzqPlt/yGFUzZgTHbO7Djc1lGA\r\n"  \
"8MXW5dRNJ2Srm8c+cftIl7gzbckTB+6WohsYFfZcTEDts8Ls/3HB40f/1LkAtDdC\r\n"  \
"2iDJ6m6K7hQGrn2iWZiIqBtvLfTyyRRfJs8sjX7tN8Cp1Tm5gr8ZDOo0rwAhaPit\r\n"  \
"c+LJMto4JQtV05od8GiG7S5BNO98pVAdvzr508EIDObtHopYJeS4d60tbvVS3bR0\r\n"  \
"j6tJLp07kzQoH3jOlOrHvdPJbRzeXDLz\r\n"                                  \
"-----END CERTIFICATE----- \r\n"                                        \
"-----BEGIN CERTIFICATE----- \r\n"                                      \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\r\n"  \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\r\n"  \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\r\n"  \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n"  \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\r\n"  \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\r\n"  \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\r\n"  \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\r\n"  \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\r\n"  \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\r\n"  \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\r\n"  \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\r\n"  \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\r\n"  \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\r\n"  \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\r\n"  \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\r\n"  \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\r\n"  \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\r\n"  \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\r\n"                  \
"-----END CERTIFICATE-----\r\n"

const char WIoTPCaPem[] = WATSON_IOT_PLATFORM_CA_PEM;

#define MQTT_TOPIC_INTERVAL "iot-2/cmd/interval/fmt/json"
int  ReportingInterval = 10000; // Default reporting interval
MQTT client( IOT_HOST, 8883, callback );




// DHT Sensor parameters
// sensor type: [DHT11, DHT22, DHT21, AM2301]
#define DHTTYPE DHT11
// which digital pin for the DHT
#define DHTPIN D3
// DHT Sensor global
DHT dht(DHTPIN, DHTTYPE);


#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
unsigned long lastSync = millis();

void setup() {
  if (millis() - lastSync > ONE_DAY_MILLIS) {
    Particle.syncTime();
    lastSync = millis();
  }

  Serial.begin( 9600 );

  while( !Serial.available() ) {
    Particle.process();
  }

  Serial.println("Welcome to the Particle Electron MQTT TLS Example");

  // Start the SHT31 sensor
  // if (! sht31.begin(0x45)) {   // Set to 0x45 for alternate i2c addr
  //   Serial.println("Couldn't find SHT31");
  // }

  // enable tls. set Root CA pem file.
  // if you don't use TLS, comment out this line.
  client.enableTls(WIoTPCaPem, sizeof(WIoTPCaPem));
  Serial.println("TLS secruity enabled");

 client.connect(
    IOT_CLIENT,
    IOT_USERNAME,
    IOT_PASSWORD
  );

  if( client.isConnected() ) {
    Serial.println( "Connected." );
    // Subscribe to "iot-2/cmd/interval/fmt/json" messages
    // from Watson IoT Platform
    client.subscribe( MQTT_TOPIC_INTERVAL );
  } else {
    Serial.println( "Connection failed");
  }

  dht.begin();
}

void EnsureConnected(){

  if( client.isConnected() ) {
    Serial.println( "Still Connected and subscribed ");
  } else {
    Serial.println( "Lost connection ... trying to reconnect!");
    client.disconnect();
    client.connect(
      IOT_CLIENT,
      IOT_USERNAME,
      IOT_PASSWORD );

    if( client.isConnected() ) {
      Serial.println( "Connected." );
    // Subscribe to "iot-2/cmd/interval/fmt/json" messages from Watson IoT Platform
      client.subscribe( MQTT_TOPIC_INTERVAL );
      Serial.println("Subscribed");
    }
  }

}


// String GetCurrentTempSHT() {
//     float celsius, fahrenheit;
//     String TempReading = String::format( "{\"d\":{\"Celsius\":0,\"Fahrenheit\":0}}" );

//     // Read Celsius and Fahrenheit values from SHT sensor
//     celsius = sht31.readTemperature();
//     // float h = sht31.readHumidity();
//     fahrenheit = (celsius* 9) /5 + 32;

//     if (! isnan(celsius)) {  // check if 'is not a number'
//         // now that we have the readings, we can publish them to the cloud
//         // store celsius and fahrenheit temp readings in "temperature" stringified JSON
//         TempReading = String::format("{\"d\":{\"Celsius\":%f,\"Fahrenheit\":%f}}", celsius, fahrenheit );

//         Serial.print( "Sending MQTT TemperatureEvent : " );
//         Serial.println( TempReading );
//     }
//     return( TempReading );
// }

String GetCurrentTemp() {
    float celsius, fahrenheit, humidity, dewpoint;

    // Read Celsius and Fahrenheit values from DHT sensor
    celsius    = dht.getTempCelcius();
    fahrenheit = dht.getTempFarenheit();
    humidity = dht.getHumidity();
    dewpoint = dht.getDewPoint();

    // now that we have the readings, we can publish them to the cloud
    // store celsius and fahrenheit temp readings in "temperature" stringified JSON
    String temperature = String::format("{\"Celsius\":%f,\"Fahrenheit\":%f,\"Humidity\":%f,\"DewPoint\":%f}", celsius, fahrenheit, humidity, dewpoint );
    String TempReading;
    TempReading = String("{\"Temp\":"+temperature+"}");
    Serial.print( "Sending MQTT TemperatureEvent : " );
    Serial.println( TempReading );
    return( TempReading );
}


void loop() {
  // String JSON;
  // count = AssetTrackerGetCurrentTemp( JSON );
  // JSON = String::format("{\"temp\":%d}", count);
  String JSON = GetCurrentTemp();
  Serial.println( JSON );
  client.publish( IOT_PUBLISH, JSON);

  client.loop();
  EnsureConnected();

  delay( ReportingInterval );
}


void callback( char* topic, byte* payload, unsigned int length ) {
  payload[length] = '\0'; // Null terminator used to terminate the char array
  Serial.printf( (char *)payload );
  int newInterval ;
  DynamicJsonDocument Interval(100);
  DeserializationError err = deserializeJson(Interval, payload);
  if( !err ) {
    newInterval = Interval["Interval"];
    // auto newInterval = Interval["Interval"].as<int>();
    Serial.printf("\nInterval is %d\n", newInterval );
    if( newInterval > 0 ) {
      // Set global - millisecond sleep
      ReportingInterval = newInterval  * 1000;
      Serial.printf("Set Reporting Interval to %d\n", ReportingInterval );
    }

  }
}
