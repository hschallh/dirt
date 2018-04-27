/*
 * Quick example of how to read multiple sensors with a single analog pin.
 * 
 * To build a soil sensor you will need:
 *  - 2 3" galvanized nails or screws
 *  - 3 wires, one red, one yellow, one black
 *  - 10Kohm resistor
 *  - A block of wood, firm plastic, or 3D printed holder for the nails
 * 1. Solder a red wire to a nail.
 * 2. Solder a black wire and a yellow wire to your other nail.
 * 3. Connect a 10Kohm resistor to the black wire.
 * 4. Red is +, black is -, yellow is read
 * 5. Screw in the screws or nails to the holder so they are parallel and 3cm apart
 * 

 * 
 * To build your board you will need:
 *  - Your completed sensor circuits
 *  - A diode for each sensor
 *  - A NodeMCU ESP8266
 * 1. Connect the "read" pin of each sensor to AD via a diode           read --|>|-- AD
 * 2. Connect the + pin of each sensor to a different digital out pin   + ---------- D1
 *    Your first sensor should connect to D1, second to d2, and so on
 * 3. Connect the - pin of all the sensors to GND                       - ---------- GND
 * 4. Change NUM_SENSORS to the number of sensors you have used (MAX 8)
 * 5. Change READ_RATE to how often you'd like to read sensor values
 * 
 */

#define NUM_SENSORS   8 // MAX 8
#define READ_RATE  2000 // In milliseconds

#define SENSOR_PIN A0
#define LOOP_OVER_PINS(x) for (int x = 0; x < NUM_SENSORS; x++)

int readings[NUM_SENSORS] = {};
long last_read_time = 0;

// LOCAL FUNCTIONS
void setup_pins();
void read_sensors();
void write_readings();

/** 
 * Run once when the device boots up. Sets up Serial for writing
 * and sets up the digital pins to enable reading of sensor vals
 */
void setup() {
  Serial.begin(9600);

  setup_pins();
}

/**
 * Run continuously after setup. Checks the time to determine
 * if it is time to read the sensors
 */
void loop() {
  // Check the time
  long now = millis();

  // Only read the sensors every READ_RATE ms
  if (now - last_read_time > READ_RATE) {
    last_read_time = now;
    read_sensors();
    write_readings();
  }
}

/**
 * Setup the enable pins to supply voltage to the sensors to
 * read. Start the LOW to ensure no voltage is supplied to
 * The sensors to start
 */
void setup_pins() {
  LOOP_OVER_PINS(pin_num) {
    pinMode(pin_num, OUTPUT);
    digitalWrite(pin_num, LOW);
  }
}

/**
 * Read values from all the sensors. To do this, we supply voltage
 * to each of the sensors in turn and read the value.
 */
void read_sensors() {
  LOOP_OVER_PINS(pin_num) {
    digitalWrite(pin_num, HIGH);
    readings[pin_num] = analogRead(SENSOR_PIN);
    digitalWrite(pin_num, LOW);
    delay(100);
  }
}

/**
 * Write all the sensor values to Serial
 */
void write_readings() {
  Serial.print("Read "); Serial.print(NUM_SENSORS); Serial.println(" sensors");
  LOOP_OVER_PINS(pin_num) {
    Serial.print("Sensor "); Serial.print(pin_num); Serial.print(" reading:\t"); Serial.println(readings[pin_num]);
  }
}

