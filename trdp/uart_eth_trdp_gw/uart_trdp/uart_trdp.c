#include "uart_trdp.h"
#include "main.h"
#include "uart_trdp.h"
#include "uart_eth_gw.h"



char rx_buf[BUF_SIZE];
char tx_buf[BUF_SIZE];

pthread_mutex_t rx_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tx_lock = PTHREAD_MUTEX_INITIALIZER;
int uart_fd ;

void *vpUartRxThread(void *arg)
{
    //uartInit(DATA_UART,BAUDRATE);
    uart_fd = uartInit(DATA_UART, BAUDRATE);
    if (uart_fd < 0)
    {
        vos_printLog(VOS_LOG_ERROR, "UART Init failed!\n");
    }

    char buf[256];
    while (1)
    {
        int n = read(uart_fd, buf, sizeof(buf));
        if (n > 0)
        {
            pthread_mutex_lock(&rx_lock);
            // copy into rx_buf or queue
            memcpy(rx_buf, buf, n);
            rx_buf[n] = '\0';
            printf("UART RX: %s\r\n", buf);
            pthread_mutex_unlock(&rx_lock);
        }
    }

    close(uart_fd);
    return NULL;
}

void *vpUartTxThread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&tx_lock);
        // if (strlen(tx_buf) > 0)
        // {
        //     int n = write(uart_fd, tx_buf, strlen(tx_buf));
        //     if (n > 0)
        //     {
        //         printf("UART TX: %s\n", tx_buf);
        //         tx_buf[0] = '\0'; // clear after sending
        //     }
        // }
        iTrdpToUartParse();
        pthread_mutex_unlock(&tx_lock);
        usleep(1000); // avoid busy loop
    }
    return NULL;
}


int uartInit(const char *dev , speed_t uibaudRate)
{
    struct termios tty;
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    memset(&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0) 
    {
        perror("tcgetattr");
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, uibaudRate);
    cfsetispeed(&tty, uibaudRate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;   // no parity
    tty.c_cflag &= ~CSTOPB;   // 1 stop bit
    tty.c_cflag &= ~CRTSCTS;  // no hw flow ctrl
    tty.c_cflag |= CLOCAL | CREAD;

    tty.c_iflag = 0;  // Clear all input flags
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // shut off xon/xoff ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); 

    // Local flags (c_lflag) - RAW MODE
    tty.c_lflag = 0;  // Clear all local flags - RAW INPUT
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN);

    // Output flags (c_oflag) - RAW OUTPUT
    tty.c_oflag = 0;  // Clear all output flags - RAW OUTPUT
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 0;

    tcflush(fd, TCIOFLUSH);

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    return fd;
}


