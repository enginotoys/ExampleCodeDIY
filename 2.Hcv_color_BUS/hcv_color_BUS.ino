#include <Arduino.h>
#include "Wire.h"
#include "veml6040.h"

int Speed = 0; // Variable to store the speed value based on detected color

const int ledPinD10 = 13;  // Digital pin 10
const int ledPinA2 =  A0;  // Analog pin A2

const int M2_D_pin = 11;  // Motor2 D pin
const int M2_A_pin = 12;  // Motor2 A pin

const int M1_D_pin = 5;   // Motor1 D pin
const int M1_A_pin = 6;   // Motor1 A pin

bool speed1_flag = true;
bool speed2_flag = true;
bool speed3_flag = true;

VEML6040 RGBWSensor; // Global instance of VEML6040

void setup() {
    Serial.begin(9600); // Initialize serial communication
    pinMode(ledPinD10, OUTPUT);
    pinMode(ledPinA2, OUTPUT);
    Wire.begin(); // Initialize I2C communication
    if (!RGBWSensor.begin()) {
        Serial.println("ERROR: couldn't detect the sensor");
        while (1) {} // Hang here if sensor initialization fails
    }
    // Configure the sensor
    RGBWSensor.setConfiguration(VEML6040_IT_320MS + VEML6040_AF_AUTO + VEML6040_SD_ENABLE);
  
    delay(1000); // Delay for sensor initialization
}

void loop() {
    // Read HSV values from the sensor
    float hueValue = RGBWSensor.rgb2hsv(0);
    float saturationValue = RGBWSensor.rgb2hsv(1);
    float valueValue = RGBWSensor.rgb2hsv(2);

    Serial.print("Hue: ");
    Serial.println(hueValue);
    Serial.print("Saturation: ");
    Serial.println(saturationValue);
    Serial.print("Value: ");
    Serial.println(valueValue);

    // Check conditions and set speed based on detected color
    if ((hueValue >= 23 && hueValue <= 55) && (saturationValue >= 0.6 && valueValue > 0.2)) {
        Serial.println("Red");
        Speed = 3;
        digitalWrite(ledPinD10, LOW); // ON headlight
        analogWrite(ledPinA2, 0);
    } else if ((hueValue >= 150 && hueValue <= 195) && (saturationValue >= 0.2 && valueValue > 0.1)) {
        Serial.println("Blue");
        Speed = 1;
        digitalWrite(ledPinD10, HIGH); // ON headlight
        analogWrite(ledPinA2, 0);
    } else if ((hueValue >= 70 && hueValue <= 100) && (saturationValue >= 0.5 && valueValue > 0.2)) {
        Serial.println("Green");
        Speed = 2;
        digitalWrite(ledPinD10, LOW); // ON headlight
        analogWrite(ledPinA2, 254);
    }

    // Control motors based on the speed value
    if (Speed == 1 && speed1_flag) {
        // Motor1
        analogWrite(M1_D_pin, 180); // Set Motor1 speed
        digitalWrite(M1_A_pin, HIGH); // Set Motor1 direction
        // Motor2
        analogWrite(M2_D_pin, 180); // Set Motor2 speed
        digitalWrite(M2_A_pin, HIGH); // Set Motor2 direction
        
        Serial.println("Speed1");

        speed1_flag = false; 
        speed2_flag = true;
        speed3_flag = true;
    }

    if (Speed == 2 && speed2_flag) {
        // Motor1
        analogWrite(M1_D_pin, 120); // Set Motor1 speed
        digitalWrite(M1_A_pin, HIGH); // Set Motor1 direction
        // Motor2
        analogWrite(M2_D_pin, 120); // Set Motor2 speed
        digitalWrite(M2_A_pin, HIGH); // Set Motor2 direction
        
        Serial.println("Speed2");

        speed2_flag = false;
        speed1_flag = true;
        speed3_flag = true;
    }

    if (Speed == 3 && speed3_flag) {
        // Stop Motor1
        digitalWrite(M1_A_pin, HIGH);
        digitalWrite(M1_D_pin, HIGH);
        // Stop Motor2
        digitalWrite(M2_D_pin, HIGH);
        digitalWrite(M2_A_pin, HIGH);
        
        Serial.println("Speed3");

        speed3_flag = false;
        speed1_flag = true;
        speed2_flag = true;
    }

    delay(100); // Delay before reading again
}
