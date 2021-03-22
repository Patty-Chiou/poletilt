//recieve:7D //drop:7E

#include "EasyLink.h"
EasyLink_RxPacket rxPacket;
EasyLink_TxPacket txPacket;
EasyLink myLink;

uint16_t PanID = 1;
uint16_t NodeID = 0; //gateway

int tmp_packet[20];
unsigned long bigMillis = 0;
unsigned long smallMillis = 0;
bool flagcall = false;
int caseswitch = 0;
int casecount = 2;

void setup() {
  Serial.begin(115200);
  // begin defaults to EasyLink_Phy_50kbps2gfsk
  myLink.begin();
  Serial.println(myLink.version());
  txPacket.dstAddr[0] = 0xaa;
}

void loop() {   
//Ask node 1 by 1 : every 10 minute or get order from TX
  if (millis() - bigMillis > 600000||Serial.available() > 0) {
    flagcall = true;
    caseswitch = 1;
    casecount = 2;
    bigMillis = millis();
  }

  if (flagcall) {
    switch (caseswitch)
    {
      default:
        if (millis() - smallMillis > 15000) {
          caseswitch = casecount;
          casecount++;
        }
        break;
        
      case 1:
        asknode(1);
        caseswitch = 0;
        smallMillis = millis();
        break;
        
      case 2:
        asknode(2);
        caseswitch = 0;
        smallMillis = millis();
        break;

      case 3:
        asknode(3);
        caseswitch = 0;
        flagcall = false;
        smallMillis = millis();
        break;
    }
  }
//listen to node
  recieve();

}
void recieve() {
  
  rxPacket.rxTimeout = EasyLink_ms_To_RadioTime(1000);
  rxPacket.absTime = EasyLink_ms_To_RadioTime(0);
  EasyLink_Status status = myLink.receive(&rxPacket);
  if (status == EasyLink_Status_Success) {
    //memcpy(&value, &rxPacket.payload, sizeof(uint16_t));
    if (rxPacket.payload[0] == 0x7D && rxPacket.payload[1] == PanID) {
      //Serial.print("Receive: ");
      for (int i = 0; i < 14; i++) {
        Serial.print(rxPacket.payload[i], HEX);
        Serial.print(" ");
      }
      /*
      for (int i = 3; i < 14; i++) {
        tmp_packet[i - 3] = rxPacket.payload[i];
      }
      int c = int(tmp_packet [0] + tmp_packet [1] + tmp_packet [2] + tmp_packet [3] + tmp_packet [4] + tmp_packet [5] + tmp_packet [6] + tmp_packet [7] + tmp_packet [8] + tmp_packet [9]);
      int sum = c & 0xFF;
      if (tmp_packet[10] == sum)
      { Serial.println(",Data Correct!");
      } else {
        Serial.println(",Bad Data QAQ");
      }
      Serial.println(millis());
      */
    }
  }
  
}

void asknode(uint16_t n) {
  txPacket.payload[0] = 0x7E;
  txPacket.payload[1] = PanID;
  txPacket.payload[2] = n;
  txPacket.len = 3;
  txPacket.absTime = EasyLink_ms_To_RadioTime(0);
  EasyLink_Status status = myLink.transmit(&txPacket);
 /*
  Serial.print("Sent");
  Serial.print(n);
  for (int i = 0; i < 3; i++) {
    Serial.print(txPacket.payload[i],HEX);
  }
  Serial.println(millis());*/
}
