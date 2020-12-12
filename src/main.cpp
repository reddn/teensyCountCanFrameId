#include <Arduino.h>

#include <FlexCAN_T4.h>


FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FCAN;

CAN_message_t canMsg;


struct frame {
  uint16_t frameId = 0;
  uint8_t timesReceivedThisSecond = 0;
  uint8_t averagedHz = 0;
  unsigned long lastMilliReceived = 0;
  unsigned int totalMessagesReceivedSinceStart = 0;
};

frame mainArray[40];
uint8_t nextArraySpot = 0;

uint8_t findFrameIdInArray(uint32_t a){
  for(uint8_t i =0; i < nextArraySpot ; i++){
    if(mainArray[i].frameId == a.frameId) return i;
  }
  return 99;
}

void setup() {
	FCAN.begin();
	FCAN.setBaudRate(500000);
  // put your setup code here, to run once:
}

void loop() {
  if ( FCAN.read(canMsg) ) {
    uint8_t frameArrayLocation = findFrameIdInArray(canMsg.id);
    
    
    if(frameArrayLocation != 99){ // it was found

    }else {// not found. need to add to array,
      
      
      nextArraySpot++;
    }
  }
  // put your main code here, to run repeatedly:
}