#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

#if defined (POSIX)
#include <unistd.h>
#elif (defined (WIN32) || defined (WIN64))
#include "getopt.h"
#endif

#include "trdp_if_light.h"
#include "vos_thread.h"
#include "vos_sock.h"
#include "vos_utils.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <malloc.h>


#define NUM_FRAMES 15
#define FRAME_SIZE 1432

#define  PACKED __attribute__((packed));

// extern VOS_QUEUE_T eth_to_uart_queue;

/**
 * @brief 
 * 
 */
void print_heap_usage(void);

#endif /* MAIN_H */  