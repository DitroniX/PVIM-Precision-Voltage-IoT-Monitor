/*
  Dave Williams, DitroniX 2019-2023 (ditronix.net)
  PVIM-1 AD7606 Precision Voltage IoT Monitor v1.0
  Features include ESP32 PVIM AD7606 16bit 200 kHz 8 Channel Isolated Differential ADC DAS EEPROM Precision Voltage GPIO I2C OLED SMPS D1 USB
  PCA 1.2304.101 - Test Code Firmware v1 - 25th April 2023

  Simplified Board Bring Up Test - I2C Bus Scanner and EEPROM Basic Test (Output sent to the Serial Print - ONLY ON BOOT! Press RESET to REFRESH)

  Remember!
  Set the BOARD to ESP32, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
  You may set the BAUD rate to 921600 to speed up flashing.
  The SDK does NOT need external power to flash.  It will take Power from the USB 5V.
  
  Note: In the default state, upon first power up and during reset, the Green LED may be partially lit. Once programmed and the GPIO defined, the Green LED will go off after power up.

  This test code is OPEN SOURCE and formatted for easier viewing.  Although is is not intended for real world use, it may be freely used, or modified as needed.
  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

  Further information, details and examples can be found on our website wiki pages ditronix.net/wiki and github.com/DitroniX.  Other project information https://www.hackster.io/ditronix
*/

// Libraries
#include <Wire.h>
#include <EEPROM.h>

// ****************  VARIABLES / DEFINES / STATIC ****************

// Constants
const int LoopDelay = 1;  // Loop Delay in Seconds

// Variables
unsigned short ReadValue;

// **************** OUTPUTS ****************
#define LED_Red 2    // Red LED
#define LED_Green 4  // Green LED

// Define I2C (Expansion Port)
#define I2C_SDA 21
#define I2C_SCL 22

// EEPROM
#define EEPROM_SIZE 1

// **************** FUNCTIONS AND ROUTINES ****************

// I2C Bus Scanner
void ScanI2CBus() {  

  byte error, address;
  int nDevices;

  Serial.println("Scanning I2C Bus for Devices ...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address Decimal ");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.print(address, HEX);
      if (address == 80) Serial.print(" EEPROM");
      Serial.println();

      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address Decimal ");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address);
      Serial.print(" = Hex 0x");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found. Possible Hardware Issue?");
  } else {
    Serial.println("I2C Bus Scan Complete");
  }
}

// Read EEPROM Test
void ReadEEPROM() {  
  Serial.print("EEPROM Test Read Value = 0x");
  ReadValue = EEPROM.read(0);
  Serial.print(ReadValue, HEX);
  if (ReadValue == 0x00) Serial.print(" Possible EEPROM Issue?");  // Blank New EEPROM should normally return 0xFF
  if (ReadValue == 0xFF) Serial.print(" Read OK.  Possible Blank EEPROM");
  Serial.println();
}

// **************** SETUP ****************
void setup() {

  // Stabalise
  delay(250);

  // Initialise UART
  Serial.begin(115200, SERIAL_8N1);  //115200
  while (!Serial)
    ;
  Serial.println("");

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // LEDs
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Green, OUTPUT);

  // LEDs Default Off State
  digitalWrite(LED_Red, HIGH);
  digitalWrite(LED_Green, HIGH);

    // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);

  // Hardware Tests
  ScanI2CBus();
  ReadEEPROM();
}

// **************** LOOP ****************
void loop() {

  // Heatbeat LED
  digitalWrite(LED_Red, LOW);
  delay(50);
  digitalWrite(LED_Red, HIGH);

  // Loop Delay
  delay(LoopDelay * 1000);
}