#include "uart_eth_gw.h"


int iTrdpToUartParse(void)
{
    int n = 0;

    #define X(TRDP_MSGID, ENABLE, COMID, SRCIP, SUB_HANDLE, MSG_BUFF, MSG_LEN, DATARECV ) \
        if ((ENABLE == TRUE) && (MSG_LEN != 0) && (DATARECV == TRUE))\
        {\
            n = write(uart_fd, MSG_BUFF, MSG_LEN);\
            if (n > 0)\
            {\
                printf("UART TX: %s\n", tx_buf);\
                tx_buf[0] = '\0';\
            }\
            DATARECV = false;\
            MSG_LEN = 0;\
            memset(MSG_BUFF, 0, MSG_LEN);\
        }

            TRDP_CONFIG_PD_RECV
    #undef X
    
    return 0;
}

int iUartToTrdpParse(void)
{
    return 0;
}