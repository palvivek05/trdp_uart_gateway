#ifndef ETHERNET_TRDP_H
#define ETHERNET_TRDP_H

#include "trdp_if_light.h"
#include "vos_thread.h"
#include "vos_sock.h"
#include "vos_utils.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_PD_COMID          15
#define DATA_MAX            1432

typedef struct trdpSendConfig
{
    bool       bEnable;
    uint16_t   ui16TrdpSendComId;
    uint32_t   ui32TrdpTxIntervel;  /* time is in microseconds */
    uint32_t   ui32TrdpDestIp;
    TRDP_PUB_T pubHandle;
    uint8_t    ui8TrdpMsgBuff[DATA_MAX];
    uint16_t   ui16TrdpMsgLen;
} trdpSendConfig_t;

typedef struct trdpRecvConfig
{
    bool     bEnable;
    uint16_t ui16TrdpRecvComId;
    uint32_t    ui32TrdpSrcIp;
    TRDP_SUB_T  subHandle; 
    uint8_t     ui8TrdpRcvMsgBuff[DATA_MAX];
    uint16_t    ui16TrdpRcvMsgLen;
    bool        bDataReceived;
    TRDP_TIME_T lastRcvTime;
} trdpRecvConfig_t;


#define TRDP_CONFIG_PD_SEND \
    X(TRDP_SEND_MSG_1 , trdpSendConfig[TRDP_SEND_MSG_1 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_1].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_1].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_1].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_1].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_1].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_1].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_2 , trdpSendConfig[TRDP_SEND_MSG_2 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_2].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_2].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_2].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_2].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_2].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_2].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_3 , trdpSendConfig[TRDP_SEND_MSG_3 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_3].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_3].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_3].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_3].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_3].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_3].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_4 , trdpSendConfig[TRDP_SEND_MSG_4 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_4].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_4].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_4].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_4].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_4].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_4].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_5 , trdpSendConfig[TRDP_SEND_MSG_5 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_5].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_5].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_5].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_5].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_5].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_5].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_6 , trdpSendConfig[TRDP_SEND_MSG_6 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_6].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_6].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_6].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_6].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_6].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_6].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_7 , trdpSendConfig[TRDP_SEND_MSG_7 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_7].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_7].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_7].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_7].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_7].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_7].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_8 , trdpSendConfig[TRDP_SEND_MSG_8 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_8].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_8].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_8].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_8].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_8].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_8].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_9 , trdpSendConfig[TRDP_SEND_MSG_9 ].bEnable, trdpSendConfig[TRDP_SEND_MSG_9].ui16TrdpSendComId,  trdpSendConfig[TRDP_SEND_MSG_9].ui32TrdpTxIntervel,  trdpSendConfig[TRDP_SEND_MSG_9].ui32TrdpDestIp,  trdpSendConfig[TRDP_SEND_MSG_9].pubHandle,  trdpSendConfig[TRDP_SEND_MSG_9].ui8TrdpMsgBuff,  trdpSendConfig[TRDP_SEND_MSG_9].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_10, trdpSendConfig[TRDP_SEND_MSG_10].bEnable, trdpSendConfig[TRDP_SEND_MSG_10].ui16TrdpSendComId, trdpSendConfig[TRDP_SEND_MSG_10].ui32TrdpTxIntervel, trdpSendConfig[TRDP_SEND_MSG_10].ui32TrdpDestIp, trdpSendConfig[TRDP_SEND_MSG_10].pubHandle, trdpSendConfig[TRDP_SEND_MSG_10].ui8TrdpMsgBuff, trdpSendConfig[TRDP_SEND_MSG_10].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_11, trdpSendConfig[TRDP_SEND_MSG_11].bEnable, trdpSendConfig[TRDP_SEND_MSG_11].ui16TrdpSendComId, trdpSendConfig[TRDP_SEND_MSG_11].ui32TrdpTxIntervel, trdpSendConfig[TRDP_SEND_MSG_11].ui32TrdpDestIp, trdpSendConfig[TRDP_SEND_MSG_11].pubHandle, trdpSendConfig[TRDP_SEND_MSG_11].ui8TrdpMsgBuff, trdpSendConfig[TRDP_SEND_MSG_11].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_12, trdpSendConfig[TRDP_SEND_MSG_12].bEnable, trdpSendConfig[TRDP_SEND_MSG_12].ui16TrdpSendComId, trdpSendConfig[TRDP_SEND_MSG_12].ui32TrdpTxIntervel, trdpSendConfig[TRDP_SEND_MSG_12].ui32TrdpDestIp, trdpSendConfig[TRDP_SEND_MSG_12].pubHandle, trdpSendConfig[TRDP_SEND_MSG_12].ui8TrdpMsgBuff, trdpSendConfig[TRDP_SEND_MSG_12].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_13, trdpSendConfig[TRDP_SEND_MSG_13].bEnable, trdpSendConfig[TRDP_SEND_MSG_13].ui16TrdpSendComId, trdpSendConfig[TRDP_SEND_MSG_13].ui32TrdpTxIntervel, trdpSendConfig[TRDP_SEND_MSG_13].ui32TrdpDestIp, trdpSendConfig[TRDP_SEND_MSG_13].pubHandle, trdpSendConfig[TRDP_SEND_MSG_13].ui8TrdpMsgBuff, trdpSendConfig[TRDP_SEND_MSG_13].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_14, trdpSendConfig[TRDP_SEND_MSG_14].bEnable, trdpSendConfig[TRDP_SEND_MSG_14].ui16TrdpSendComId, trdpSendConfig[TRDP_SEND_MSG_14].ui32TrdpTxIntervel, trdpSendConfig[TRDP_SEND_MSG_14].ui32TrdpDestIp, trdpSendConfig[TRDP_SEND_MSG_14].pubHandle, trdpSendConfig[TRDP_SEND_MSG_14].ui8TrdpMsgBuff, trdpSendConfig[TRDP_SEND_MSG_14].ui16TrdpMsgLen ) \
    X(TRDP_SEND_MSG_15, trdpSendConfig[TRDP_SEND_MSG_15].bEnable, trdpSendConfig[TRDP_SEND_MSG_15].ui16TrdpSendComId, trdpSendConfig[TRDP_SEND_MSG_15].ui32TrdpTxIntervel, trdpSendConfig[TRDP_SEND_MSG_15].ui32TrdpDestIp, trdpSendConfig[TRDP_SEND_MSG_15].pubHandle, trdpSendConfig[TRDP_SEND_MSG_15].ui8TrdpMsgBuff, trdpSendConfig[TRDP_SEND_MSG_15].ui16TrdpMsgLen ) \

#define TRDP_CONFIG_PD_RECV \
    X(TRDP_RECV_MSG_1,  trdpRecvConfig[TRDP_RECV_MSG_1 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_1 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_1 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_1 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_1 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_1 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_1 ].bDataReceived ) \
    X(TRDP_RECV_MSG_2,  trdpRecvConfig[TRDP_RECV_MSG_2 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_2 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_2 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_2 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_2 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_2 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_2 ].bDataReceived ) \
    X(TRDP_RECV_MSG_3,  trdpRecvConfig[TRDP_RECV_MSG_3 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_3 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_3 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_3 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_3 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_3 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_3 ].bDataReceived ) \
    X(TRDP_RECV_MSG_4,  trdpRecvConfig[TRDP_RECV_MSG_4 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_4 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_4 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_4 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_4 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_4 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_4 ].bDataReceived ) \
    X(TRDP_RECV_MSG_5,  trdpRecvConfig[TRDP_RECV_MSG_5 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_5 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_5 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_5 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_5 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_5 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_5 ].bDataReceived ) \
    X(TRDP_RECV_MSG_6,  trdpRecvConfig[TRDP_RECV_MSG_6 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_6 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_6 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_6 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_6 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_6 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_6 ].bDataReceived ) \
    X(TRDP_RECV_MSG_7,  trdpRecvConfig[TRDP_RECV_MSG_7 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_7 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_7 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_7 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_7 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_7 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_7 ].bDataReceived ) \
    X(TRDP_RECV_MSG_8,  trdpRecvConfig[TRDP_RECV_MSG_8 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_8 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_8 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_8 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_8 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_8 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_8 ].bDataReceived ) \
    X(TRDP_RECV_MSG_9,  trdpRecvConfig[TRDP_RECV_MSG_9 ].bEnable, trdpRecvConfig[TRDP_RECV_MSG_9 ].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_9 ].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_9 ].subHandle, trdpRecvConfig[TRDP_RECV_MSG_9 ].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_9 ].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_9 ].bDataReceived ) \
    X(TRDP_RECV_MSG_10, trdpRecvConfig[TRDP_RECV_MSG_10].bEnable, trdpRecvConfig[TRDP_RECV_MSG_10].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_10].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_10].subHandle, trdpRecvConfig[TRDP_RECV_MSG_10].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_10].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_10].bDataReceived ) \
    X(TRDP_RECV_MSG_11, trdpRecvConfig[TRDP_RECV_MSG_11].bEnable, trdpRecvConfig[TRDP_RECV_MSG_11].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_11].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_11].subHandle, trdpRecvConfig[TRDP_RECV_MSG_11].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_11].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_11].bDataReceived ) \
    X(TRDP_RECV_MSG_12, trdpRecvConfig[TRDP_RECV_MSG_12].bEnable, trdpRecvConfig[TRDP_RECV_MSG_12].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_12].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_12].subHandle, trdpRecvConfig[TRDP_RECV_MSG_12].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_12].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_12].bDataReceived ) \
    X(TRDP_RECV_MSG_13, trdpRecvConfig[TRDP_RECV_MSG_13].bEnable, trdpRecvConfig[TRDP_RECV_MSG_13].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_13].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_13].subHandle, trdpRecvConfig[TRDP_RECV_MSG_13].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_13].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_13].bDataReceived ) \
    X(TRDP_RECV_MSG_14, trdpRecvConfig[TRDP_RECV_MSG_14].bEnable, trdpRecvConfig[TRDP_RECV_MSG_14].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_14].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_14].subHandle, trdpRecvConfig[TRDP_RECV_MSG_14].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_14].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_14].bDataReceived ) \
    X(TRDP_RECV_MSG_15, trdpRecvConfig[TRDP_RECV_MSG_15].bEnable, trdpRecvConfig[TRDP_RECV_MSG_15].ui16TrdpRecvComId, trdpRecvConfig[TRDP_RECV_MSG_15].ui32TrdpSrcIp, trdpRecvConfig[TRDP_RECV_MSG_15].subHandle, trdpRecvConfig[TRDP_RECV_MSG_15].ui8TrdpRcvMsgBuff, trdpRecvConfig[TRDP_RECV_MSG_15].ui16TrdpRcvMsgLen, trdpRecvConfig[TRDP_RECV_MSG_15].bDataReceived ) \

enum {
    #define X(TRDP_MSGID, ENABLE, COMID, INTERVAL, DESTIP, PUB_HANDLE, MSG_BUFF, MSG_LEN) TRDP_MSGID,
        TRDP_CONFIG_PD_SEND
    #undef X
};

enum {
    #define X(TRDP_MSGID, ENABLE, COMID, SRCIP, SUB_HANDLE, MSG_BUFF, MSG_LEN, DATARECV) TRDP_MSGID,
        TRDP_CONFIG_PD_RECV
    #undef X
};



extern trdpSendConfig_t trdpSendConfig[MAX_PD_COMID];
extern trdpRecvConfig_t trdpRecvConfig[MAX_PD_COMID];

/**
 * @brief 
 * 
 */
void vSetDefTrdpSendConfig(void);

/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void* pvEthTrdp(void* arg);


/**
 * @brief 
 * 
 * @param pRefCon 
 * @param appHandle 
 * @param pMsg 
 * @param pData 
 * @param dataSize 
 */
void pdCallback(void *pRefCon, TRDP_APP_SESSION_T appHandle, const TRDP_PD_INFO_T *pMsg, 
                UINT8 *pData, UINT32 dataSize);

/**
 * @brief 
 * 
 */
void vSetDefTrdpRecvConfig(void);

#endif /* ETHERNET_TRDP */ 