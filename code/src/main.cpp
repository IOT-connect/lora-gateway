#include <Arduino.h>
#include <SPI.h>
#include <Ethernet3.h>
#include <EthernetUdp3.h>

#include <SoftwareSerial.h>
SoftwareSerial Serial5(A5,A6);

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 1883;    //10002;  // local port to listen on

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";       // a string to send back

EthernetUDP Udp;

void setup() {
  Serial5.begin(9600);
  delay(5000);
  Ethernet.setCsPin(PIN_PB0);
  Ethernet.begin(mac);

  Udp.begin(2000);
  Serial5.println(Ethernet.localIP());
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial5.print("Received packet of size ");
    Serial5.println(packetSize);
    Serial5.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial5.print(remote[i], DEC);
      if (i < 3)
      {
        Serial5.print(".");
      }
    }
    Serial5.print(", port ");
    Serial5.println(Udp.remotePort());

    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial5.println("Contents:");
    Serial5.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  delay(10);
}