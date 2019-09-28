#ifndef _SNIFFER_MAC_H_
#define _SNIFFER_MAC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Wireshark_Settings_tag
{
  // Settings
  WS_STATUS,              // 0
  WS_GET_VERSION,         // 1
  WS_GET_ISM_BAND,        // 2
  WS_SET_PAGE,            // 3
  WS_SET_CHANNEL,         // 4

  // Commands
  WS_CAPTURE_START,       // 5
  WS_CAPTURE_STOP,        // 6
  WS_CAPTURE_RESTART,     // 7
  WS_CAPTURE_FRAME,       // 8

  WS_SETTINGS_SIZE,       // 9
} Wireshark_Settings_t;
typedef enum Wireshark_Settings_Status_tag
{
  WS_STATUS_OK,
  WS_STATUS_ERROR,
} Wireshark_Settings_Status_t;

#define WS_FRAME_HEADER_SIZE  1
#define WS_FRAME_FCS_SIZE     2
#define WS_FRAME_MAX_SIZE     128

void mac_recv_process(uint8_t* buffer, int16_t length);
#define mac_send_data phy_send

void mac_init(void);
void mac_task(void);

#ifdef __cplusplus
}
#endif

#endif // _SNIFFER_MAC_H_
