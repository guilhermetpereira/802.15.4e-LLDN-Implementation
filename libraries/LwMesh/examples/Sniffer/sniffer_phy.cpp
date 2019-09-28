#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "sniffer_phy.h"
#include "sniffer_mac.h"

static bool phy_in_dle = false;
static PHYCommandState_t phy_in_state;
uint8_t phy_in_buffer[MAX_BUFFER_SIZE];
static int16_t phy_in_buffer_size = 0;

void phy_init(void)
{
  phy_in_dle= false;
  phy_in_state = CMDS_IDLE;

  Serial.begin(115200);
}
void phy_recv_task(void)
{
  static uint8_t data;

  while(Serial.available())
  {
    data = Serial.read();

    if(phy_in_dle)
    {
      phy_in_dle= false;

      if(data != ASCII_DLE)
      {
        if(data == ASCII_ETX)
        {
          if(phy_in_state != CMDS_BUSY || phy_in_buffer_size <= 0)
            continue;

          phy_in_state= CMDS_IDLE;

          mac_recv_process(phy_in_buffer, phy_in_buffer_size);
        }
        else
        {
          phy_in_buffer_size= 0;
          phy_in_state= (data == ASCII_STX) ? CMDS_BUSY : CMDS_IDLE ;
        }

        continue;
      }
    }
    else if(data == ASCII_DLE)
    {
      // Start decode Command
      phy_in_dle= true;
      continue;
    }

    // Only in payload state system store stream of bytes
    if(phy_in_state != CMDS_BUSY)
      continue;

    if(phy_in_buffer_size >= MAX_BUFFER_SIZE)
    {
      phy_in_state= CMDS_IDLE;
    }
    else
    {
      phy_in_buffer[phy_in_buffer_size++]= data;
    }
  }
}
void phy_send(const uint8_t* buffer, int16_t lenght)
{
  // Send RAW Header ESC STX
  Serial.write(ASCII_DLE);
  Serial.write(ASCII_STX);

  // Loop data escaping any ESC
  for(int16_t i = 0; i < lenght; ++i)
  {
    if(buffer[i] == ASCII_DLE)
    {
      Serial.write(ASCII_DLE);
    }

    Serial.write(buffer[i]);
  }

  // Send RAW Header ESC ETX
  Serial.write(ASCII_DLE);
  Serial.write(ASCII_ETX);
}

#ifdef __cplusplus
}
#endif

