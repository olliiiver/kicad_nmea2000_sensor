#include <Arduino.h>

#include <N2kMsg.h>
#include <NMEA2000.h>
#include <avr_can.h>            // https://github.com/thomasonw/avr_can
#include <NMEA2000_avr.h>       // https://github.com/thomasonw/NMEA2000_avr


#include <N2kMessages.h>

tNMEA2000_avr NMEA2000;

Stream *OutputStream;

// List here messages your device will transmit.
const unsigned long TransmitMessages[] PROGMEM={127505L,0};

int count = 0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  // Set Product information
  //NMEA2000.SetN2kCANSendFrameBufSize(50);
  //NMEA2000.SetN2kCANReceiveFrameBufSize(2);
  NMEA2000.SetN2kCANMsgBufSize(1);

  NMEA2000.SetProductInformation("00001337", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "Message sender example",  // Manufacturer's Model ID
                                 "1.0.0.15 (2017-01-01)",  // Manufacturer's Software version code
                                 "1.0.0.0 (2017-01-01)" // Manufacturer's Model version
                                 );
  // Set device information
  NMEA2000.SetDeviceInformation(1337, // Unique number. Use e.g. Serial number.
                                132, // Device function=Analog to NMEA 2000 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                25, // Device class=Inter/Intranetwork Device. See codes on  http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );

  Serial.begin(115200);
  OutputStream=&Serial;

  // logging
  // NMEA2000.SetForwardStream(&Serial);  // PC output on due programming port
  // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.
  // NMEA2000.SetForwardOwnMessages();

  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,22);

  // NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.ExtendTransmitMessages(TransmitMessages);
  NMEA2000.Open();

  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);
}

#define SlowDataUpdatePeriod 1000

void SendN2kSlowData() {
  static unsigned long SlowDataUpdated=millis();
  tN2kMsg N2kMsg;

  int raw = 0;
  int percent = 0;
  float R1 = 1000; // known 1k
  float max = 160; // max sensor range (0 - 190 ohm)

  if (SlowDataUpdated+SlowDataUpdatePeriod<millis()) {
    SlowDataUpdated=millis();
    raw = analogRead(0);
    // percent = 100 - (raw / R1 * 100);
    //    percent = (raw / max * 100);
    if (raw < 15) {
      percent = 0;
    } else if (raw >= 15 && raw < 32) {
      percent = 10;
    } else if (raw >= 32 && raw < 50) {
      percent = 15;
    } else if (raw >= 50 && raw < 60) {
      percent = 20;
    } else if (raw >= 60 && raw < 70) {
      percent = 25;
    } else if (raw >= 70 && raw < 80) {
      percent = 30;
    } else if (raw >= 80 && raw < 105) {
      percent = 40;
    } else if (raw >= 105 && raw < 130) {
      percent = 50;
    } else if (raw >= 130 && raw < 155) {
      percent = 75;
    } else if (raw >= 155) {
      percent = 100;
    }


    percent = (percent > 100) ? 100 : percent;
    percent = (percent < 1) ? 0 : percent;
    OutputStream->print("===> raw: ");
    OutputStream->print(raw);
    OutputStream->print(" percent: ");
    OutputStream->println(percent);

    // Send fuel level in percent for the 2nd fuel tank (60 liters max vol.)
    SetN2kFluidLevel(N2kMsg, 1, N2kft_Fuel, percent, 60);
    NMEA2000.SendMsg(N2kMsg);

    if (count == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      count = 1;
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      count = 0;
    }
  }


}

void loop() {
  SendN2kSlowData();
  NMEA2000.ParseMessages();
}

