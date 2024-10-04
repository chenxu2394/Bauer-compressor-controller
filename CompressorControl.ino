/*
  CompressorControl.ino

  This Arduino sketch is designed to control a compressor based on the input
  from two sensors indicating the state of a gas bag. The compressor is
  controlled to ensure that the gas bag does not overinflate or completely
  deflate.

  The system supports manual override via a USB input to simulate sensor
  activation.

  Pin Assignments:
  - Compressor control signal: Pin 11
  - Top sensor input: Pin 7
  - Bottom sensor input: Pin 8
  - USB relay input (for simulation/testing): Pin 9

  The logic includes edge-triggered signals to start/stop the compressor
  to conform with the compressor's PLC requirements.
*/

int compressor = 11;   // Pin connected to the compressor relay
int top_sensor = 7;    // Pin reading the top sensor state
int bottom_sensor = 8; // Pin reading the bottom sensor state
int flag = 0;          // State flag to remember the last state
int top_signal;        // Variable to store the state of the top sensor
int bottom_signal;     // Variable to store the state of the bottom sensor
int usb;               // Variable to read the USB override input
int usb_relay = 9;     // USB relay pin

void setup() {
  // Set the pin modes for input and output appropriately
  pinMode(top_sensor, INPUT);
  pinMode(bottom_sensor, INPUT);
  pinMode(compressor, OUTPUT);
  pinMode(usb_relay, INPUT);
}

void loop() {
  // Read the states of top and bottom sensors, and the USB relay
  top_signal = digitalRead(top_sensor);
  bottom_signal = digitalRead(bottom_sensor);
  usb = digitalRead(usb_relay);

  // Check if the USB is activated to simulate top sensor blocked
  if (usb == 1) {
    top_signal = 1; // Simulate top sensor blocked by USB trigger
    flag = 3;
  }

  // Evaluate the sensor states and control the compressor accordingly
  if (top_signal == 1 && bottom_signal == 1) {
    triggerCompressorStart(); // Both sensors are blocked
    flag = 1;
  } else if (top_signal == 0 && bottom_signal == 0) {
    triggerCompressorStop(); // Both sensors are unblocked
    flag = 2;
  } else if (top_signal == 0 && bottom_signal == 1) {
    // Bottom sensor blocked and top sensor unblocked
    flag = 3; // Continue current state, may need to handle previous state
  }
}

void triggerCompressorStart() {
  if (flag != 1) {
    // Ensure a rising edge to start the compressor
    digitalWrite(compressor, LOW);
    delay(1000); // Ensure the LOW state is long enough to be recognized
    digitalWrite(compressor, HIGH);
  }
}

void triggerCompressorStop() {
  if (flag != 2) {
    // Ensure a falling edge to stop the compressor
    digitalWrite(compressor, HIGH);
    delay(1000); // Ensure the HIGH state is long enough to be recognized
    digitalWrite(compressor, LOW);
  }
}
