#ifndef UART_TRDP_H
#define UART_TRDP_H

#include "main.h"

#define DATA_UART    "/dev/ttySTM2"
#define BUF_SIZE     1432
#define BAUDRATE     B115200

extern pthread_mutex_t rx_lock;
extern pthread_mutex_t tx_lock;
extern char rx_buf[BUF_SIZE];
extern char tx_buf[BUF_SIZE];
extern int uart_fd;

/**
 * @brief 
 * 
 * @param dev 
 * @param uibaudRate 
 * @return int 
 */
int uartInit(const char *dev , speed_t uibaudRate);

/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void *vpUartRxThread(void *arg);

/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void *vpUartTxThread(void *arg);



#endif /* UART_TRDP_H*/