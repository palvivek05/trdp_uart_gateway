#include "main.h"
#include "ethernet_trdp.h"
#include "uart_trdp.h"

/* macros  */


void print_heap_usage(void) 
{
    static struct mallinfo2 mi;
    mi = mallinfo2();
    printf("Heap total allocated: %d bytes\n", mi.uordblks);
    printf("Heap free space:      %d bytes\n", mi.fordblks);
}


/**
 * @brief Create a thread object
 * 
 * @param threadId 
 * @param threadFunc 
 * @param policy 
 * @param priority 
 * @param stackSize 
 * @return int 
 */
int create_thread(pthread_t *threadId,
                  void* (*threadFunc)(void*),
                  int policy,
                  int priority,
                  size_t stackSize)
{
    pthread_attr_t attr;
    struct sched_param param;
    int ret;

    // 1. Initialize attribute structure
    pthread_attr_init(&attr);

    // 2. Set stack size
    pthread_attr_setstacksize(&attr, stackSize);

    // 3. Set scheduling policy (SCHED_FIFO or SCHED_RR for realtime)
    pthread_attr_setschedpolicy(&attr, policy);

    // 4. Set priority
    param.sched_priority = priority;
    pthread_attr_setschedparam(&attr, &param);

    // 5. Explicit scheduling (not inherit from parent)
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // 6. Create thread with these attributes
    ret = pthread_create(threadId, &attr, threadFunc, NULL);

    // 7. Destroy attr (no longer needed after create)
    pthread_attr_destroy(&attr);

    return ret;
}



/**
 * @brief 
 * 
 * @return int 
 */
int main()
{

    pthread_t threadTrdpEthId, threadUartRxId, threadUartTxId;
    int ret = 0;

    /*
        int minPrio = sched_get_priority_min(SCHED_FIFO);
        int maxPrio = sched_get_priority_max(SCHED_FIFO);
    
        printf("****** the task proiity  min priority is  %u \n",minPrio);
        printf("****** the task proiity  Max priority is  %u \n",maxPrio);
    */
    printf("befor initiating anything\n");
    print_heap_usage();

    /**
     * @brief 
     * 
     */
    ret = pthread_create(&threadTrdpEthId, NULL, pvEthTrdp, NULL);
    if (ret != 0)
    {
        vos_printLog(VOS_LOG_ERROR," Ethernet Task creation failed!\n");
        return -1;
    }

    /**
     * @brief 
     * 
     */
    ret = pthread_create(&threadUartRxId, NULL, vpUartRxThread, NULL);
    if (ret != 0)
    {
        vos_printLog(VOS_LOG_ERROR," Uart Rx Task creation failed!\n");
        return -1;
    }

/**
 * @brief 
 * 
 */
    ret = pthread_create(&threadUartTxId, NULL, vpUartTxThread, NULL);
    if (ret != 0)
    {
        vos_printLog(VOS_LOG_ERROR," Uart Tx Task creation failed!\n");
        return -1;
    }


    while (1)
    {

        sleep(2);
        //print_heap_usage();
        // pthread_mutex_lock(&tx_lock);
        // strcpy(tx_buf, "Hello from TX\r\n");
        // vos_printLog(VOS_LOG_INFO,"sending data on uart \n");
        // pthread_mutex_unlock(&tx_lock);

    }


    return 0;
}
