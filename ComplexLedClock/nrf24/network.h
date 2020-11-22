#ifndef _NETWORK_H_
#define _NETWORK_H_

enum cmdTypes{
  NTP_TIME = 200,
  MIN_BRIGHTNESS = 201,
  MAX_LUX_RANGE = 202
};

struct netPacket{
  unsigned char cmd;
  unsigned char data1;
  unsigned char data2;
  unsigned char data3;
};

void initNetwork();
void processNetwork();


#endif //_NETWORK_H_
