#include <Arduino.h>

#include <FlexCAN_T4.h>

unsigned long lastTimeSerialSent = 0;
unsigned long timeBetweenSerialSends = 10000;

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FCAN;

CAN_message_t canMsg;


struct frame {
  uint32_t frameId = 0;
  uint8_t timesReceivedThisSecond = 0;
  uint8_t averagedHz = 0;
  unsigned long lastMilliReceived = 0;
  unsigned int totalMessagesReceivedSinceStart = 0;
  unsigned long lastSecondReceived = 0;
};

frame mainArray[70];
uint8_t nextArraySpot = 0;

uint8_t findFrameIdInArray(uint32_t a){
  for(uint8_t i =0; i < nextArraySpot ; i++){
    if(mainArray[i].frameId == a) return i;
  }
  return 99;
}

void setup() {
	FCAN.begin();
	FCAN.setBaudRate(500000);
  // put your setup code here, to run once:
}

void loop() {
  if( ( millis() - lastTimeSerialSent ) > timeBetweenSerialSends){
    for(uint8_t i =0 ; i < nextArraySpot ; i++){
      Serial.print("FrameID: ");
      Serial.println(mainArray[i].frameId, DEC);

      Serial.print("Averaged Hz: ");
      Serial.println(mainArray[i].averagedHz, DEC);

      Serial.print("Total Msg Rcvd: ");
      Serial.println(mainArray[i].totalMessagesReceivedSinceStart, DEC);

      Serial.print("Last Whole Second (since Start) received a message");
      Serial.println(mainArray[i].lastSecondReceived, DEC);

      Serial.println();
    }

    Serial.print(" -------------------------------------------------------------- ");
    Serial.println();
    Serial.println();
  }


  if ( FCAN.read(canMsg) ) {
    uint8_t frameArrayLocation = findFrameIdInArray(canMsg.id);
    unsigned long currentSecond = millis() * .001;
    
    if(frameArrayLocation != 99){ // it was found
      if(mainArray[frameArrayLocation].lastSecondReceived == currentSecond){
        mainArray[frameArrayLocation].timesReceivedThisSecond++;
      } else{ //new second
        uint16_t lclHz =  mainArray[frameArrayLocation].averagedHz + mainArray[frameArrayLocation].timesReceivedThisSecond;
        if(lclHz > 0)         mainArray[frameArrayLocation].averagedHz = lclHz / 2;

        mainArray[frameArrayLocation].lastSecondReceived = currentSecond;
      }
      mainArray[frameArrayLocation].lastSecondReceived = currentSecond;
      mainArray[frameArrayLocation].lastMilliReceived = millis();
      mainArray[frameArrayLocation].totalMessagesReceivedSinceStart++;
    } else {// not found. need to add to array,
      mainArray[nextArraySpot].frameId = canMsg.id;
      mainArray[nextArraySpot].lastMilliReceived = millis();
      mainArray[nextArraySpot].totalMessagesReceivedSinceStart++;
      mainArray[nextArraySpot].lastSecondReceived = currentSecond;
      nextArraySpot++;
    }
  }
  // put your main code here, to run repeatedly:
}