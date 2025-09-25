#include "ethernet_trdp.h"
#include "uart_trdp.h"


/***********************************************************************************************************************
 * DEFINITIONS
 */

#define APP_VERSION         "1.4"
// #define PD_DEF_COMID        1000
// #define PD_DEF_COMID_CYCLE  1000000             /* in us (1000000 = 1 sec) */
//#define PD_DEF_DATASIZE     DATA_MAX
// UINT8 gData[DATA_MAX];   // this data buffer we have to send data to ques fr om cli and 

/* We use dynamic memory    */
#define RESERVED_MEMORY     160000

/* Example COMIDs */
UINT32 ui32DefaultComIdList[MAX_PD_COMID] = {
    1000, 1001, 1002, 1003, 1004,
    1005, 1006, 1007, 1008, 1009,
    1010, 1011, 1012, 1013, 1014
};

TRDP_IP_ADDR_T gDestIpList[MAX_PD_COMID] = {
    0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E,
    0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E,
    0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E
};

UINT32 ui32RecvComIdList[MAX_PD_COMID] = {
    2000, 2001, 2002, 2003, 2004,
    2005, 2006, 2007, 2008, 2009,
    2010, 2011, 2012, 2013, 2014
};

/* Source IP addresses to receive from - modify as needed */
TRDP_IP_ADDR_T gSrcIpList[MAX_PD_COMID] = {
    0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E,
    0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E,
    0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E, 0x0A00006E
};


trdpSendConfig_t trdpSendConfig[MAX_PD_COMID];
trdpRecvConfig_t trdpRecvConfig[MAX_PD_COMID];

void vSetDefTrdpSendConfig(void)
{
    int n = 0 ;

    #define X(TRDP_MSGID, ENABLE, COMID, INTERVAL, DESTIP, PUB_HANDLE, MSG_BUFF, MSG_LEN)\
        COMID = ui32DefaultComIdList[TRDP_MSGID];\
        DESTIP = gDestIpList[TRDP_MSGID];\
        ENABLE = false;\
        INTERVAL = (TRDP_MSGID + 1) * 500000u;\
        memset(&PUB_HANDLE, 0, sizeof(PUB_HANDLE));\
        n = snprintf((char *)MSG_BUFF, sizeof(MSG_BUFF), "this is test msg for com id %u ", COMID);\
        MSG_LEN = (n > 0) ? (UINT16)n : 0u;\

        TRDP_CONFIG_PD_SEND
    #undef X
}

/**
 * @brief Set default TRDP receive configuration
 */
void vSetDefTrdpRecvConfig(void)
{
    #define X(TRDP_MSGID, ENABLE, COMID, SRCIP, SUB_HANDLE, MSG_BUFF, MSG_LEN,  DATARECV)\
        COMID = ui32RecvComIdList[TRDP_MSGID];\
        SRCIP = gSrcIpList[TRDP_MSGID];\
        ENABLE = true;\
        memset(&SUB_HANDLE, 0, sizeof(SUB_HANDLE));\
        memset(MSG_BUFF, 0, sizeof(MSG_BUFF));\
        MSG_LEN = 0;\
        trdpRecvConfig[TRDP_MSGID].bDataReceived = false;\
        memset(&trdpRecvConfig[TRDP_MSGID].lastRcvTime, 0, sizeof(trdpRecvConfig[TRDP_MSGID].lastRcvTime));\

        TRDP_CONFIG_PD_RECV
    #undef X
}

/* hardcode configuration for trdp */

/**********************************************************************************************************************/
/** callback routine for TRDP logging/error output
 *
 *  @param[in]      pRefCon         user supplied context pointer
 *  @param[in]      category        Log category (Error, Warning, Info etc.)
 *  @param[in]      pTime           pointer to NULL-terminated string of time stamp
 *  @param[in]      pFile           pointer to NULL-terminated string of source module
 *  @param[in]      LineNumber      line
 *  @param[in]      pMsgStr         pointer to NULL-terminated string
 *
 *  @retval         none
 */
void dbgOut (
    void        *pRefCon,
    TRDP_LOG_T  category,
    const CHAR8 *pTime,
    const CHAR8 *pFile,
    UINT16      LineNumber,
    const CHAR8 *pMsgStr)
{
    const char *catStr[] = {"**Error:", "Warning:", "   Info:", "  Debug:", "   User:"};

    if (category != VOS_LOG_DBG)
    {
        /* we filter some more info we're not interested in */
        if ((category != VOS_LOG_INFO) ||
            (strstr(pFile, "vos_sock.c") == NULL))
        {
            printf("%s %s %s",
                   strrchr(pTime, '-') + 1,
                   catStr[category],
                   pMsgStr);
        }
    }
}


/**
 * @brief Get the Own Ip Address object
 * 
 * @return uint32_t 
 */
uint32_t getOwnIpAddress()
{
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];  // For IPv4 (use INET6_ADDRSTRLEN for IPv6)
    unsigned int  ownIp[4];
    uint32_t uidOwnIpAdd = 0;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 1;
    }

    // Loop through all interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

            // IPv4 only
            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
                inet_ntop(AF_INET, &(sa->sin_addr), ip, sizeof(ip));

                if(strcmp(ifa->ifa_name,"eth0") == 0 )
                {
                    if (sscanf(ip,"%u.%u.%u.%u",&ownIp[3],&ownIp[2],&ownIp[1],&ownIp[0]) < 4)
                    {
                        return 0;
                    }
                    //printf("Interface: %s\tIP: %s\n", ifa->ifa_name, ip);
                    //printf("the eth0 ip is %d.%d.%d.%d \n",ownIp[3],ownIp[2],ownIp[1],ownIp[0]);
                    uidOwnIpAdd = (ownIp[3] << 24) | (ownIp[2] << 16) | (ownIp[1] << 8) | ownIp[0];
                    //printf("the own ip address is %u \n ",uidOwnIpAdd);
                    break;
                }
            }
    }

    freeifaddrs(ifaddr);
    return uidOwnIpAdd;

}

/**
 * @brief 
 * 
 * @param pRefCon 
 * @param appHandle 
 * @param pMsg 
 * @param pData 
 * @param dataSize 
 */
void pdCallback(void *pRefCon, 
                TRDP_APP_SESSION_T appHandle, 
                const TRDP_PD_INFO_T *pMsg, 
                UINT8 *pData, 
                UINT32 dataSize)
{
    uint32_t copySize = 0;

    #define X(TRDP_MSGID, ENABLE, COMID, SRCIP, SUB_HANDLE, MSG_BUFF, MSG_LEN, DATARECV ) \
         if (pMsg != NULL && pData != NULL)\
         {\
            if(COMID == pMsg->comId && ENABLE)\
            {\
                copySize = (dataSize < DATA_MAX) ? dataSize : DATA_MAX;\
                MSG_LEN = copySize;\
                memcpy(MSG_BUFF, pData, copySize);\
                MSG_LEN = copySize;\
                DATARECV = true;\
                /*vos_getTime(&trdpRecvConfig[i].lastRcvTime);*/\
                vos_printLog(VOS_LOG_INFO, "Received COM ID %u, size %u bytes from IP 0x%08X\n", pMsg->comId, dataSize, pMsg->srcIpAddr);\
            }\
         }
        

        TRDP_CONFIG_PD_RECV
    #undef X

}


/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void* pvEthTrdp(void* arg)
{

    TRDP_APP_SESSION_T      appHandle; /*    Our identifier to the library instance    */
    int                     verbose =     0;                      // this is hardcord we have to configure from cli
    TRDP_ERR_T              err;
    TRDP_PD_CONFIG_T        pdConfiguration =
                        {NULL, NULL, TRDP_PD_DEFAULT_SEND_PARAM, TRDP_FLAGS_NONE, 1000000u, TRDP_TO_SET_TO_ZERO, 0u};
    TRDP_MEM_CONFIG_T       dynamicConfig   = {NULL, RESERVED_MEMORY, {0}};
    TRDP_PROCESS_CONFIG_T   processConfig   = {"Me", "", "", TRDP_PROCESS_DEFAULT_CYCLE_TIME, 0u, TRDP_OPTION_BLOCK};
    UINT32                  ownIP   = 0u;
    int                     rv      = 0;


    //int                     ch, i;


    /*    Init the library  */
    if (tlc_init(&dbgOut,                              /* no logging    */
                 NULL,
                 &dynamicConfig) != TRDP_NO_ERR)    /* Use application supplied memory    */
    {
        printf("Initialization error\n");
    }

    // geting own ip address
    ownIP = getOwnIpAddress();
    printf("the own ip address of %u \n",ownIP);

    vSetDefTrdpSendConfig();
    vSetDefTrdpRecvConfig();

     /*    Open a session  */
    if (tlc_openSession(&appHandle,
                        ownIP, 0,               /* use default IP address           */
                        NULL,                   /* no Marshalling                   */
                        &pdConfiguration, NULL, /* system defaults for PD and MD    */
                        &processConfig) != TRDP_NO_ERR)
    {
        vos_printLogStr(VOS_LOG_USR, "Initialization error\n");
    }

    /*    Copy the packet into the internal send queue, prepare for sending.    */
    /*     If we change the data, just re-publish it    */
    #define X(TRDP_MSGID, ENABLE, COMID, INTERVAL, DESTIP, PUB_HANDLE, MSG_BUFF, MSG_LEN ) \
        if (ENABLE == TRUE)\
        {\
            err = tlp_publish(  appHandle,                  /*    our application identifier    */\
                                &PUB_HANDLE,                /*    our pulication identifier     */\
                                NULL, NULL, \
                                0u, \
                                COMID,                      /*    ComID to send                 */\
                                0,                          /*    local consist only            */\
                                0,\
                                ownIP,                      /*    default source IP             */\
                                DESTIP,                     /*    where to send to              */\
                                INTERVAL,                   /*    Cycle time in us              */\
                                0,                          /*    not redundant                 */\
                                TRDP_FLAGS_NONE,            /*    Use callback for errors       */\
                                NULL,                       /*    default qos and ttl           */\
                                MSG_BUFF,                   /*    initial data                  */\
                                MSG_LEN                     /*    data size                     */\
                                );\
            if (err != TRDP_NO_ERR)\
            {\
                vos_printLogStr(VOS_LOG_ERROR, "prep pd error for COM Id \n");\
                tlc_terminate();\
            }\
        }
    
        TRDP_CONFIG_PD_SEND
    #undef X
    
    
    #define X(TRDP_MSGID, ENABLE, COMID, SRCIP, SUB_HANDLE, MSG_BUFF, MSG_LEN, DATARECV ) \
        if (ENABLE) {\
            err = tlp_subscribe(appHandle,                  /*  1. our application identifier    */\
                              &SUB_HANDLE,                  /*  2. our subscription identifier   */\
                              NULL,                         /*  3. user reference                */\
                              pdCallback,                   /*  4. callback function             */\
                              0u,                           /*  5. serviceId                     */\
                              COMID,                        /*  6. ComID to receive              */\
                              0,                            /*  7. etbTopoCnt                    */\
                              0,                            /*  8. opTrnTopoCnt                  */\
                              SRCIP,                        /*  9. srcIpAddr1                    */\
                              0,                            /* 10. srcIpAddr2 (0 = any)          */\
                              ownIP,                        /* 11. destIpAddr (our IP)           */\
                              TRDP_FLAGS_CALLBACK,          /* 12. pktFlags - Use callback       */\
                              NULL,                         /* 13. pRecParams - default          */\
                              10000000u,                    /* 14. timeout in microseconds       */\
                              TRDP_TO_SET_TO_ZERO);         /* 15. toBehavior                    */\
            if (err != TRDP_NO_ERR)\
            {\
                vos_printLog(VOS_LOG_ERROR, "subscribe pd error for COM Id %u (%s)\n", COMID, vos_getErrorString((VOS_ERR_T)err));\
                tlc_terminate();\
            }\
        }

        TRDP_CONFIG_PD_RECV
    #undef X

    /*
     Finish the setup.
     On non-high-performance targets, this is a no-op.
     This call is necessary if HIGH_PERF_INDEXED is defined. It will create the internal index tables for faster access.
     It should be called after the last publisher and subscriber has been added.
     Maybe tlc_activateSession would be a better name.If HIGH_PERF_INDEXED is set, this call will create the internal index tables for fast telegram access
     */

    err = tlc_updateSession(appHandle);
    if (err != TRDP_NO_ERR)
    {
        vos_printLog(VOS_LOG_USR, "tlc_updateSession error (%s)\n", vos_getErrorString((VOS_ERR_T)err));
        tlc_terminate();
    }

    vos_printLogStr(VOS_LOG_USR, "running...\n");

    while (1)
    {
        TRDP_FDS_T rfds;
        INT32 noDesc;
        TRDP_TIME_T tv;
        const TRDP_TIME_T   max_tv  = {1, 0};
        const TRDP_TIME_T   min_tv  = {0, TRDP_PROCESS_DEFAULT_CYCLE_TIME};

        /*
           Prepare the file descriptor set for the select call.
           Additional descriptors can be added here.
         */
        FD_ZERO(&rfds);
        /* FD_SET(pd_fd, &rfds); */

        /*
           Compute the min. timeout value for select.
           This way we can guarantee that PDs are sent in time
           with minimum CPU load and minimum jitter.
         */
        tlc_getInterval(appHandle, &tv, &rfds, &noDesc);
       /**< Micro seconds (max. value 999999)                */

        /*
           The wait time for select must consider cycle times and timeouts of
           the PD packets received or sent.
           If we need to poll something faster than the lowest PD cycle,
           we need to set the maximum time out our self.
         */
        if (vos_cmpTime(&tv, &max_tv) > 0)
        {
            tv = max_tv;
        }
        else if (vos_cmpTime(&tv, &min_tv) < 0)
        {
            tv = min_tv;
        }

        /*
           Select() will wait for ready descriptors or time out,
           what ever comes first.
         */

        rv = vos_select(noDesc, &rfds, NULL, NULL, &tv);

        /*
           Check for overdue PDs (sending and receiving)
           Send any pending PDs if it's time...
           Detect missing PDs...
           'rv' will be updated to show the handled events, if there are
           more than one...
           The callback function will be called from within the tlc_process
           function (in it's context and thread)!
         */

        (void) tlc_process(appHandle, &rfds, &rv);

        if (verbose)
        {
            printf(".");
            fflush(stdout);
        }
        
        /* Send each COMID respecting its interval */
        #define X(TRDP_MSGID, ENABLE, COMID, INTERVAL, DESTIP, PUB_HANDLE, MSG_BUFF, MSG_LEN ) \
            err = tlp_put(appHandle, PUB_HANDLE, MSG_BUFF, MSG_LEN); \

            TRDP_CONFIG_PD_SEND
        #undef X
        // do TRDP / TCN related work here
        //sleep(1);
        //usleep(1000000);
    }


    #define X(TRDP_MSGID, ENABLE, COMID, SRCIP, SUB_HANDLE, MSG_BUFF, MSG_LEN, DATARECV ) \
    if (ENABLE)\
    {\
         tlp_unsubscribe(appHandle, SUB_HANDLE);\
     }

     TRDP_CONFIG_PD_RECV
    #undef X

    #define X(TRDP_MSGID, ENABLE, COMID, INTERVAL, DESTIP, PUB_HANDLE, MSG_BUFF, MSG_LEN ) \
        tlp_unpublish(appHandle, PUB_HANDLE);\

        TRDP_CONFIG_PD_SEND
    #undef X

    tlc_closeSession(appHandle);
    tlc_terminate();
    //return rv;

}


