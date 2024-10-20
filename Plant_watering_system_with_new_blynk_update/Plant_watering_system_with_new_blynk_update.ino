#define BLYNK_TEMPLATE_ID "TMPL3P-L9MaHF"
#define BLYNK_TEMPLATE_NAME "Automatic water irrigation system"
#define BLYNK_AUTH_TOKEN "MDSV9Qr5LVV_1qsqwP46rxAcBIdLCw2M"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>



// Blynk Auth Token, Wi-Fi SSID, and password
char ssid[] = "sidd phone";  // Enter your Wi-Fi name (SSID)
char pass[] = "123456789";                          // Enter your Wi-Fi password

BlynkTimer timer;
bool Relay = 0;

// Define component pins
#define sensor A0       // Moisture sensor pin
#define waterPump D3    // Water pump control pin

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);  // Ensure water pump is off initially

  // Connect to Blynk Cloud
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

  // Set a timer to check soil moisture every 1 second (1000 milliseconds)
  timer.setInterval(1000L, soilMoistureSensor);

  // Indicate system setup is complete
  Serial.println("System Loading...");

  delay(5000); // Simulate system loading time
  Serial.println("System Loaded");
}

// Handle Blynk virtual button for pump control (on V1)
BLYNK_WRITE(V1) {
  Relay = param.asInt();
 if (Relay == 1) {
    digitalWrite(waterPump, LOW);  // Turn the motor on
  } else {
    digitalWrite(waterPump, HIGH);  // Turn the motor off
  }
 
}

// Read soil moisture sensor and send data to Blynk
void soilMoistureSensor() {
  int value = analogRead(sensor);  // Read analog value from moisture sensor
  value = map(value, 0, 1024, 0, 100);  // Map the sensor value to a percentage
  value = (value - 100) * -1;  // Invert the value to show correct percentage
 if (value <= 40) {
    digitalWrite(waterPump, LOW);  // Turn ON the water pump
    Serial.println("Motor is ON");
  } else {
    digitalWrite(waterPump, HIGH); // Turn OFF the water pump
    Serial.println("Motor is OFF");
  }
  Blynk.virtualWrite(V0, value);  // Send moisture level to Blynk on V0
  Serial.print("Moisture Level: ");
  Serial.print(value);
  Serial.println("%");
}

void loop() {
  Blynk.run();  // Run Blynk connection
  timer.run();  // Run the timer for moisture sensor
}
