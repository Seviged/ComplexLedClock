#include "nrf24.h"
#include "network.h"
#include "../Settings/settings.h"
#include "../ds3231/DS3231.h"

extern RealTimeClock rtc;
extern uint8_t minBrightness;
extern uint8_t maxLuxValue;

uint8_t data_array[4];
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};

void initNetwork()
{
    /* init hardware pins */
    nrf24_init();
    
    /* Channel #2 , payload length: 4 */
    nrf24_config(150,4);
    
    /* Set the device addresses */
    nrf24_tx_address(tx_address);
    nrf24_rx_address(rx_address);
}


void processNetwork()
{
    if(nrf24_dataReady())
    {
        nrf24_getData(data_array);
        
        netPacket payload;
        payload.cmd = data_array[0];
        payload.data1 = data_array[1];
        payload.data2 = data_array[2];
        payload.data3 = data_array[3];
        
        switch((enum cmdTypes)payload.cmd)
        {
            case NTP_TIME:
            rtc.setTimeToRtc(payload.data1, payload.data2, payload.data3);
            break;
            case MIN_BRIGHTNESS:
            minBrightness = payload.data3;
            writeSettings(EE_MIN_BRIGHTNESS, payload.data3);
            break;
            case MAX_LUX_RANGE:
            maxLuxValue = payload.data3;
            writeSettings(EE_MAX_LUX_RANGE, payload.data3);
            break;
        }
    }
}
