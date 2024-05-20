#include <Servo.h>

#define s0 2        // Module pins wiring for Arduino Uno
#define s1 3
#define s2 4
#define s3 5
#define out 6       // Note: Choose a different pin if pin 6 is used for S2

int Red = 0, Blue = 0, Green = 0;  // RGB values

Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  pinMode(s0, OUTPUT);    // Pin modes
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  servo1.attach(8); // Attach servo 1 to pin 9
  servo2.attach(9); // Attach servo 2 to pin 10
  servo3.attach(11); // Attach servo 3 to pin 11

  Serial.begin(9600);   // Initialize the serial monitor baud rate

  digitalWrite(s0, HIGH); // Putting S0/S1 on HIGH/HIGH levels means the output frequency scaling is at 100% (recommended)
  digitalWrite(s1, HIGH); // LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is 2%
}

void loop() {
  GetColors();                                     // Execute the GetColors function to get the value of each RGB color

  Serial.print("Red: ");
  Serial.print(Red);
  Serial.print(", Blue: ");
  Serial.print(Blue);
  Serial.print(", Green: ");
  Serial.println(Green);

  if (Red <= 15 && Green <= 15 && Blue <= 15) { // If the values are low it's likely the white color (all the colors are present)
    Serial.println("White");
    // Turn off all servos if white is detected
    servo1.write(90); // Stop servo 1
    servo2.write(90); // Stop servo 2
    servo3.write(90); // Stop servo 3
  } else if (Red < Blue && Red <= Green && Red < 23) { // If Red value is the lowest one and smaller than 23 it's likely Red
    Serial.println("Red");
    // Activate servo 1
    servo1.write(0); // Rotate servo 1 to position 0 degrees
    delay(1000); // Wait for servo to reach position
    servo1.write(90); // Stop servo 1
  } else if (Blue < Green && Blue < Red && Blue < 20) { // If Blue value is the lowest one and smaller than 20 it's likely Blue
    Serial.println("Blue");
    // Activate servo 2
    servo2.write(0); // Rotate servo 2 to position 0 degrees
    delay(1000); // Wait for servo to reach position
    servo2.write(90); // Stop servo 2
  } else if (Green < Red && Green < Blue && Green - Blue <= 8) { // If Green value is the lowest one and difference between Green and Blue is acceptable, it's likely Green
    Serial.println("Green");
    // Activate servo 3
    servo3.write(0); // Rotate servo 3 to position 0 degrees
    delay(1000); // Wait for servo to reach position
    servo3.write(90); // Stop servo 3
  } else {
    Serial.println("Unknown"); // if the color is not recognized, you can add as many as you want
    // Turn off all servos for unknown color
    servo1.write(90); // Stop servo 1
    servo2.write(90); // Stop servo 2
    servo3.write(90); // Stop servo 3
  }

  delay(2000); // 2s delay you can modify if you want
}

void GetColors() {
  digitalWrite(s2, LOW); // S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
  digitalWrite(s3, LOW);
  Red = AveragePulseIn(out, LOW, 10); // Here we wait until "out" goes LOW, we start measuring the duration and stops when "out" is HIGH again
  delay(20);
  digitalWrite(s3, HIGH); // Here we select the other color (set of photodiodes) and measure the other colors value using the same technique
  Blue = AveragePulseIn(out, LOW, 10);
  delay(20);
  digitalWrite(s2, HIGH);
  Green = AveragePulseIn(out, LOW, 10);
  delay(20);
}

unsigned long AveragePulseIn(uint8_t pin, uint8_t state, int times) {
  unsigned long sum = 0;
  for (int i = 0; i < times; i++) {
    sum += pulseIn(pin, state);
  }
  return sum / times;
}