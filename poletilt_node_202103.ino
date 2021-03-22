//recieve:7E //drop:7D

#include "EasyLink.h"
EasyLink_RxPacket rxPacket;
EasyLink_TxPacket txPacket;
EasyLink myLink;

uint16_t PanID = 1;
uint16_t NodeID = 1;

int tmp_packet[20];
#define  BC847 RED_LED //reopen WT31S through BC847


void setup() {
  Serial.begin(115200);
  myLink.begin();
  Serial.println(myLink.version());
  txPacket.dstAddr[0] = 0xaa;
  pinMode(BC847, OUTPUT);
  digitalWrite(BC847, HIGH);
}

void loop() {
  //listen Rx (WT31s
  if (Serial.available() > 0) {
    if (pkgread() == true) {
      pkgsent();
    }
  }
//listen gateway
  recieve();
}

//function

void recieve() {
  //listen gateway
  rxPacket.rxTimeout = EasyLink_ms_To_RadioTime(1000);
  rxPacket.absTime = EasyLink_ms_To_RadioTime(0);
  EasyLink_Status status = myLink.receive(&rxPacket);

  if (status == EasyLink_Status_Success) {
    if ((rxPacket.payload[0] == 0x7E) && (rxPacket.payload[1] == PanID)) { //收到閘道器命令他
      if (rxPacket.payload[2] == NodeID) {
        //Serial.print("CallMe");
        Serial.print("Recieve!");
        /*Serial.write(77);
          Serial.write(77);
          Serial.println("wake");*/
        digitalWrite(BC847, LOW);
        delay(1000);
        digitalWrite(BC847, HIGH);
      }
    }
  }
}

bool pkgread() {
  int a = Serial.read();
  if (a == 0x55) { 
    while (!Serial.available()); // wait for a character
    int b = Serial.read();
    if (b == 0x51) {
      tmp_packet[0] = 0x55;
      tmp_packet[1] = 0x51;
      for (int i = 2; i < 11; i++) { 
        while (!Serial.available()); 
        //delay(2);
        tmp_packet[i] = Serial.read();
      }
      return true;
    } else return false;
  }
}

void pkgsent() {
  txPacket.payload[0] = 0x7D;
  txPacket.payload[1] = PanID;
  txPacket.payload[2] = NodeID;
  for (int i = 3; i < 14; i++) {
    txPacket.payload[i] = tmp_packet[i - 3];
  }
  txPacket.len = 14;
  txPacket.absTime = EasyLink_ms_To_RadioTime(0);
  EasyLink_Status status = myLink.transmit(&txPacket);

  /*Serial.print("WtSent~");
    for (int i = 0; i < 14; i++) {
    Serial.print(txPacket.payload[i]);
    }*/

  for (int i = 0; i < 14; i++) {
    txPacket.payload[i] = NULL;
  }
}
