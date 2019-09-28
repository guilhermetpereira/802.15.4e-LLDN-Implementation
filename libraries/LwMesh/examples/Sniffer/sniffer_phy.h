#ifndef _SNIFFER_PHY_H_
#define _SNIFFER_PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ascii_table_t
{
	ASCII_NUL	= 0x00,					// Null - Nulo												// Reservado
	ASCII_SOH,							// Start of Header - In�cio do cabe�alho					// Bloco Inicio
	ASCII_STX,							// Start of Text - In�cio do texto							// Frame Inicio
	ASCII_ETX,							// End of Text - Fim do texto								// Frame Fim
	ASCII_EOT,							// End of Tape - Fim de fita								//
	ASCII_ENQ,							// Enquire - Interroga identidade do terminal				//
	ASCII_ACK,							// Acknowledge - Reconhecimento								// Reservado
	ASCII_BEL,							// Bell - Campainha
	ASCII_BS,							// Back-space - Espa�o atr�s
	ASCII_HT,							// Horizontal Tabulation - Tabula��o horizontal
	ASCII_LF,							// Line-Feed - Alimenta linha
	ASCII_VT,							// Vertical Tabulation - Tabula��o vertical
	ASCII_FF,							// Form-Feed - Alimenta formul�rio
	ASCII_CR,							// Carriage-Return - Retorno do carro (enter)
	ASCII_SO,							// Shift-Out - Sa�da do shift (passa a usar
	//	caracteres caixa baixa - min�sculas, etc.)
	ASCII_SI,							// Shift-In - Entrada no shift (passa a usar				// Bloco
	//	caracteres caixa alta: mai�sculas, caracteres especiais, etc.)
	ASCII_DLE,							// Data-Link Escape											// Escape
	ASCII_DC1,							// Device-Control 1
	ASCII_DC2,							// Device-Control 2
	ASCII_DC3,							// Device-Control 3
	ASCII_DC4,							// Device-Control 4
	ASCII_NAK,							// Neg-Acknowledge - N�o-reconhecimento						// Erro
	ASCII_SYN,							// Synchronous Idle
	ASCII_ETB,							// End-of-Transmission Block								// Bloco Fim
	ASCII_CAN,							// Cancel
	ASCII_EM,							// End-Of-Medium											// Carrousel - End of Parameter
	ASCII_SUB,							// Substitute
	ASCII_ESC,							// Escape
	ASCII_FS,							// File Separator											// Carrousel - File Parameter
	ASCII_GS,							// Group Separator
	ASCII_RS,							// Record Separator											// Carrousel - Record peace Parameter
	ASCII_US,							// Unit Separator
} ascii_table_t;

typedef enum PHYCommandState_tag
{
	CMDS_IDLE,
	CMDS_BUSY,
} PHYCommandState_t;

#define MAX_BUFFER_SIZE 256

void phy_init(void);
void phy_recv_task(void);
void phy_send(const uint8_t* buffer, int16_t lenght);

#ifdef __cplusplus
}
#endif

#endif // _SNIFFER_PHY_H_
