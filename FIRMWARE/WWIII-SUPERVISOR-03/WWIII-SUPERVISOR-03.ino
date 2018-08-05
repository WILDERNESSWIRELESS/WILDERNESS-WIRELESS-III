//==============================================================================
// Wilderness Wireless System Supervisor
// Brett Ian Balogh
// Summer 2018
//
// This program is designed to run on a ATMEGA328P microcontroller
// whose fuses are set to use the internal oscillator at 8Mhz, divided by 8 to
// yield a clock speed of 1MHz. This 1Mhz clock is available on AVR pin 14 for
// debugging purposes.
//
// The purpose of this code is to provide a low-level intelligence to the system.
// This intelligence takes measurements from the fuel gauge and decides on what
// the rest of the system should do. There are two threshold battery voltages,
// BAT_LO and BAT_HI. If the system starts when the battery is depleated,
// the battery voltage will be below the BAT_LO threshold, and the
// host's regulator will be inhibited. The host will then be inactive until the
// fuel gague reports a voltage above BAT_HI. Above the BAT_HI threshold, the
// regulator is enabled, and the host boots up and does its thing. The host will
// remain up as long as the battery voltage is above BAT_LO. When the battery
// voltage dips below BAT_LO, the regulator is inhibited, and the host shuts down.
//
// There are three status LEDs. If the battery voltage is below the BAT_LO
// threshold, only the bottom LED blinks. If the battery is charging and the
// battery voltage is greater than BAT_LO but lower than BAT_HI, two LEDs will
// blink. The bottom one first and then the one above. If the battery is charging
// and the battery voltage is above BAT_HI, three LEDs will blink in order of
// bottom to top. The order of the lights is reversed when the battery is
// discharging.
//
// BAT_LO is currently set to 25% and BAT_HI to 95%. The 'off' time at 25%
// has been on the order of days. Good for inclement weather. 
//
// This is pretty much alpha ware. You have been warned. Much more to come.
//
// It works for now, so let it be.
//
//==============================================================================

#include <EEPROM.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SparkFunBQ27441.h>

#define INTERRUPT_PIN   2
#define ENABLE_PIN      3
#define UP_PIN          4 // --> GPIO13
#define AVR_BUSY_PIN    6
#define HOST_BUSY_PIN   7
#define BAT_LO_LED      11
#define BAT_OK_LED      12
#define BAT_HI_LED      13

#define DEBUG

String cmdString = "";
String messageBuffer = "";
String data = "";

char inChar;

bool hostStatus = false; // false=off, true=on
bool shutdownStatus = false;
bool startupStatus = false;
bool BAT_OK = false;
bool recv = false;
bool cmdInProgress = false;
bool requestInProgress = false;
bool i2cInUse = false;
bool i2cMaster = true;



double temp = 0;

unsigned long currentTime = 0;

// Battery stats

unsigned int designCapacity = 0;
unsigned int soc = 0;
unsigned int volts = 0;
unsigned int fullCapacity = 0;
unsigned int remainingCapacity = 0;
int current = 0;
int power = 0;
int health = 0;

unsigned int BAT_LO = 0;
unsigned int BAT_HI = 0;
unsigned int DEFAULT_LO = 25;
unsigned int DEFAULT_HI = 95;
unsigned int BAT_CAPACITY = 1200;
unsigned int CAPACITY_NOW = 0;
unsigned int CAPACITY_THEN = 0;
unsigned int eeaddress = 0;
unsigned int blinkOnDelay = 50;
unsigned int blinkOffDelay = 100;
unsigned int CHARGING = 2;
int timeThen = 0;
int timeNow = 0;
int interval = 1000;
int payloadIndex = 0;
int index;

//==============================================================================
//========/FUNCTIONS/===========================================================
//==============================================================================

void setupBQ27441(void)
{
  if (!lipo.begin())
  {
    printNotification(F("Error: Unable to communicate with BQ27441."));
    while (1) ;
  }
  printNotification(F("Connected to BQ27441!"));
  lipo.setCapacity(BAT_CAPACITY);
}

//
// GET INFO FROM BQ27441
//

void getBatteryStats() {
  volts = lipo.voltage(); // Read battery voltage (mV)
  current = lipo.current(AVG); // Read average current (mA)
  fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  remainingCapacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  designCapacity = lipo.capacity(DESIGN);
  power = lipo.power(); // Read average power draw (mW)
  health = lipo.soh(); // Read state-of-health (%)
  temp = lipo.temperature();
  soc = lipo.soc();  // Read state-of-charge (%)
}

//
// Print status string
//

void printStatus()
{
  // Now print out those values:
  String toPrint = "";
  toPrint += String(volts) + "mV | ";
  toPrint += String(current) + "mA | ";
  toPrint += String(power) + "mW | ";
  toPrint += String(remainingCapacity) + "mAh | ";
  toPrint += String(BAT_LO) + "% | ";
  toPrint += String(soc) + "% | ";
  toPrint += String(BAT_HI) + "% | ";

  if (hostStatus) {
    toPrint += "HOST UP |";
  }
  else {
    toPrint += "HOST DN |";
  }
  if (BAT_OK) {
    toPrint += " BAT OK";
  }
  else {
    toPrint += " BAT LO";
  }
  Serial.println(toPrint);
}

//
// General Status Message Function
//

void printNotification(String msg) {
  Serial.println();
  Serial.print(" >>> ");
  Serial.print(msg);
  Serial.println();
}

//
// Simple status blink
//

void statusBlink(int d) {
  if (d == 1) {
    if (soc <= BAT_LO) {
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
    }
    if (soc > BAT_LO && soc < BAT_HI) {
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_OK_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
      digitalWrite(BAT_OK_LED, LOW);
      delay(blinkOffDelay);
    }
    if (soc >= BAT_HI) {
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_OK_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_HI_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
      digitalWrite(BAT_OK_LED, LOW);
      delay(blinkOffDelay);
      digitalWrite(BAT_HI_LED, LOW);
      delay(blinkOffDelay);
    }
  }
  if (d == 2) {
    if (soc <= BAT_LO) {
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
    }
    if (soc > BAT_LO && soc < BAT_HI) {
      digitalWrite(BAT_OK_LED, HIGH);
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      digitalWrite(BAT_OK_LED, LOW);
      delay(blinkOffDelay);
    }
    if (soc >= BAT_HI) {
      digitalWrite(BAT_HI_LED, HIGH);
      digitalWrite(BAT_OK_LED, HIGH);
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      digitalWrite(BAT_OK_LED, LOW);
      digitalWrite(BAT_HI_LED, LOW);
      delay(blinkOffDelay);
    }
  }
  if (d == 3) {
    if (soc <= BAT_LO) {
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
    }
    if (soc > BAT_LO && soc < BAT_HI) {
      digitalWrite(BAT_OK_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_OK_LED, LOW);
      delay(blinkOffDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
    }
    if (soc >= BAT_HI) {
      digitalWrite(BAT_HI_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_OK_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_LO_LED, HIGH);
      delay(blinkOnDelay);
      digitalWrite(BAT_HI_LED, LOW);
      delay(blinkOffDelay);
      digitalWrite(BAT_OK_LED, LOW);
      delay(blinkOffDelay);
      digitalWrite(BAT_LO_LED, LOW);
      delay(blinkOffDelay);
    }
  }
}

//
// Parses incoming JSON
//

void parseJsonCommand(String s) {
  StaticJsonBuffer<128> jsonInBuffer;
  JsonObject& recvMsg = jsonInBuffer.parseObject(s);

  if (!recvMsg.success()) {
    printNotification(F("JSON parsing failed"));
    return;
  }

  if (recvMsg.containsKey("batlo")) {
    BAT_LO = recvMsg["batlo"];
    EEPROM.put(0, BAT_LO);
    printNotification(F("Setting low battery threshold."));
    printNotification(F("Saving to EEPROM"));
  }

  if (recvMsg.containsKey("bathi")) {
    BAT_HI = recvMsg["bathi"];
    EEPROM.put(sizeof(int), BAT_HI);
    printNotification(F("Setting hi battery threshold to: "));
    printNotification(F("Saving to EEPROM"));
  }

  if (recvMsg.containsKey("design")) {
    printNotification(F("Setting design capacity to: "));
    int cap = recvMsg["design"];
    lipo.setCapacity(cap);
    Serial.println(cap);
  }

  if (recvMsg.containsKey("cmd")) {
    //printNotification(F("Received a command request: "));
    String cmd = recvMsg["cmd"];
    if (cmd.equals("time") || cmd.equals("TIME") || cmd.equals("Time")) {
      //stats["time"].printTo(data);
      //itoa(currentTime, data, 10);
      data = String(currentTime);
    }
    if (cmd.equals("soc") || cmd.equals("SOC") || cmd.equals("Soc")) {
      //itoa(soc, data, 10);
      data = String(soc);
    }
  }
}

//
// cmdBuilder builds commands crom individual characters
// accepts input from serial and i2c
//

void cmdBuilder(char c) {
  if (c == '{') {
    // SET IN PROGRESS FLAG
    cmdInProgress = true;
  }
  // IS THE CHARACTER A TERMINATION CHAR?
  if (c == '}') {
    messageBuffer += c;
    // COMMAND IS NO LONGER IN PROGRESS
    cmdInProgress = false;
    // COPY COMMAND TO MESSAGE
    cmdString = messageBuffer;
    // CLEAR COMMAND STRING
    messageBuffer = "";
    // PARSE MESSAGE
    parseJsonCommand(cmdString);
    return;
  }
  // IS THERE A COMMAND IN PROCESS?
  if (cmdInProgress) {
    // THROW ANOTHER CHAR ON THE HEAP
    messageBuffer += c;
  }
  else {
    //Serial.println("INVALID INPUT");
    messageBuffer = "";
  }
}

//
// HANDLE INCOMING I2C RECEIVE
//

void receiveEvent() {
  requestInProgress = true;
  while (Wire.available() > 0) { // loop through all but the last
    char inChar = Wire.read(); // receive byte as a character
    cmdBuilder(inChar);
  }
  requestInProgress = false;
}

//
// HANDER SERIAL EVENT
//

void serialEvent() {
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    cmdBuilder(inChar);
  }
}

//
// HANDLE I2C REQUEST
//

void requestEvent() {
  requestInProgress = true;
  Wire.write(data.c_str());
  requestInProgress = false;
}

//
// Switch between i2c slave and master
//

void switchMode(String mode) {
  if (mode.equals("MASTER")) {
    Wire.end();
    i2cMaster = true;
    digitalWrite(AVR_BUSY_PIN, HIGH);
    //delay(10);
    Wire.begin();
    //printNotification(F("AVR switching to MASTER MODE"));
  }
  if (mode.equals("SLAVE")) {
    Wire.end();
    i2cMaster = false;
    digitalWrite(AVR_BUSY_PIN, LOW);
    //delay(10);
    Wire.begin(0x77);
    //printNotification(F("AVR switching to SLAVE MODE @ 0x77"));
    // REGISTER I2C CALLBACK
    Wire.onReceive(receiveEvent);
    // REGISTER I2C CALLBACK
    Wire.onRequest(requestEvent);
  }
}

void loadParams() {
  printNotification(F("Retrieving params from EEPROM"));
  EEPROM.get(eeaddress, BAT_LO);
  eeaddress += sizeof(unsigned int);
  EEPROM.get(eeaddress, BAT_HI);
  eeaddress = 0;
  if (BAT_LO < 0 || BAT_LO > 100) {
    printNotification(F("Invalid BAT_LO read, using default"));
    BAT_LO = DEFAULT_LO;
  }
  if (BAT_HI < 0 || BAT_HI > 100) {
    printNotification(F("Invalid BAT_HI read, using default"));
    BAT_HI = DEFAULT_HI;
  }
}

void isr() {
  switchMode("SLAVE");
}



//==============================================================================
//========/SETUP/===============================================================
//==============================================================================


void setup() {

  // SET PIN MODES
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(UP_PIN, INPUT);
  pinMode(BAT_LO_LED, OUTPUT);
  pinMode(BAT_OK_LED, OUTPUT);
  pinMode(BAT_HI_LED, OUTPUT);
  pinMode(AVR_BUSY_PIN, OUTPUT);
  pinMode(HOST_BUSY_PIN, INPUT);

  // SET INITIAL PIN STATES
  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(AVR_BUSY_PIN, LOW);  // Flag pin is low when it is ok for PI to talk

  // ATTACH INTERRUPT
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), isr, RISING);

  // START HARDWARE SERIAL PORT
  Serial.begin(9600);

  // START I2C
  //switchMode("MASTER");
  Wire.begin();

  // START FUEL GAUGE
  setupBQ27441();  // Wire() is started in master mode

  // LOAD PARAMETERS FROM EEPROM
  loadParams();
  CAPACITY_THEN = lipo.capacity(REMAIN);
  timeThen = millis();

}

//==============================================================================
//========/LOOP/================================================================
//==============================================================================


void loop() {

  // WHAT'S THE LOCAL TIME?
  timeNow = millis();

  // IS THE PI RUNNING?
  hostStatus = digitalRead(UP_PIN);


  if (timeNow - timeThen > interval && !cmdInProgress && !shutdownStatus && !requestInProgress && !i2cInUse) {
    getBatteryStats();
    printStatus();

    CAPACITY_NOW = lipo.capacity(REMAIN);

    if (CAPACITY_NOW > CAPACITY_THEN) {
      CHARGING = 1;
    }
    //if (CAPACITY_NOW == CAPACITY_THEN) {
    //  CHARGING = 2;;
    //}
    if (CAPACITY_NOW < CAPACITY_THEN) {
      CHARGING = 3;
    }
    statusBlink(CHARGING);
    if (soc <= BAT_LO) {
      BAT_OK = false;
      digitalWrite(ENABLE_PIN, LOW);
    }
    if (soc >= BAT_HI) {
      BAT_OK = true;
      digitalWrite(ENABLE_PIN, HIGH);
    }
    CAPACITY_THEN = CAPACITY_NOW;
    timeThen = timeNow;
  }

  //
  // CHECK STATE AND DO STUFF
  //
  /*
    if (!BAT_OK & hostStatus && !shutdownStatus) {
      digitalWrite(SHUTDN_PIN, LOW);
      shutdownStatus = true;
    #ifdef DEBUG
      Serial.println(F("********"));
      Serial.println(F("LO BAT Detected, initiating SHUTDOWN..."));
      Serial.println(F("********"));
      Serial.print(F("SHUTDOWN In Progress. Waiting for system DOWN..."));
    #endif
    }

    //if (shutdownStatus && hostStatus) {
    //  Serial.print(".");
    //}

    if (shutdownStatus && !hostStatus) {
      digitalWrite(ENABLE_PIN, LOW);
      digitalWrite(SHUTDN_PIN, HIGH);
      shutdownStatus = false;
    #ifdef DEBUG
      Serial.println();
      Serial.println(F("********"));
      Serial.println(F("Shutdown complete, disabling boost..."));
      Serial.println(F("********"));
      delay(1000);
    #endif
    }

    if (BAT_OK && !hostStatus && !startupStatus) {
      digitalWrite(ENABLE_PIN, LOW);
      digitalWrite(ENABLE_PIN, HIGH);
      startupStatus = true;
      #ifdef DEBUG
      Serial.println(F("********"));
      Serial.println(F("BAT OK, initiating STARTUP..."));
      Serial.println(F("********"));
    #endif
    }

    if (hostStatus && startupStatus) {
      startupStatus = false;
      #ifdef DEBUG
      Serial.println(F("********"));
      Serial.println(F("Startup complete..."));
      Serial.println(F("********"));
      #endif
    }
  */
}
