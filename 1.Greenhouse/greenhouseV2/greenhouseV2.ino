#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//Lcd init
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo myservo;

const int dry = 202; // value for dry sensor
const int wet = 515; // value for wet sensor

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
 
  Wire.begin();
  myservo.attach(7);  // Attach the servo to pin 9
  myservo.write(0);
  Serial.begin(9600);


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  display.clearDisplay();
  dht.begin(); // Initialize DHT sensor
}

void loop() {
  


  int sensorVal = analogRead(A7);// read data Molsture sensor
  int percentageHumidity = map(sensorVal, wet, dry, 0, 100);
  Serial.print("Molsture RAW : ");
  Serial.println(sensorVal);
  
  Serial.print("Molsture HUM : ");
  Serial.println(percentageHumidity);

  delay(100);  // Delay for DHT sensor reading

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT sensor data!");
    return;
  }

  // Display on the OLED screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("DATA: ");
  display.println(" ");

  display.print("Moisture: ");
  display.print(percentageHumidity);
  display.println("%");

  display.print("Temp:");
  display.print(temperature);
  display.println(" C, ");
  display.print("Hum:");
  display.print(humidity);
  display.println(" %");


  display.display();
  // Control servo based on temperature
 if (temperature > 25) {
    myservo.write(180); // Rotate servo to 180 degrees
  } else if (temperature < 24) {
    myservo.write(0); // Rotate servo to 0 degrees
  }

  delay(50);
}
