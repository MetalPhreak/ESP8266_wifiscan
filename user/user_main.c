/******************************************************************************
 * 
 *
 * FileName: user_main.c
 *
 * Description: basic wifi scanner
 *
 * Modification history:
 *     2016/3/8, v1.0 
*******************************************************************************/
#include "ets_sys.h"
#include "osapi.h"

#include "user_interface.h"

#define TopTaskPrio        2
#define TopTaskQueueLen    1

#define ActionTaskPrio        1
#define ActionTaskQueueLen    8

#define IdleTaskPrio        0
#define IdleTaskQueueLen    1

static void TopTask(os_event_t *events);
static void ActionTask(os_event_t *events);
static void IdleTask(os_event_t *events);

os_event_t    TopTaskQueue[TopTaskQueueLen];
os_event_t    ActionTaskQueue[ActionTaskQueueLen];
os_event_t    IdleTaskQueue[IdleTaskQueueLen];



static void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status) 
{
    if (status == OK) {
        struct bss_info *bss_link = (struct bss_info *)arg;
        //bss_link = bss_link->next.stqe_next; //ignore first (not needed since SDK v1.4 fixes this)

        while(bss_link != NULL) {
            os_printf("%32s (%02x:%02x:%02x:%02x:%02x:%02x) ch: %02d rssi: %02d auth: %02d hidden: %d freq_offset: %d\n", 
                bss_link->ssid,
                MAC2STR(bss_link->bssid),
                bss_link->channel, 
                bss_link->rssi, 
                bss_link->authmode,
                bss_link->is_hidden,
                bss_link->freq_offset
            );
            
            bss_link = bss_link->next.stqe_next;
        }
    } else {
        os_printf("Scan Failed?\n");
    }

    //add idle task back to queue once scan is complete
    system_os_post(IdleTaskPrio, 0, 0);


}

static void ICACHE_FLASH_ATTR TopTask(os_event_t *events){


}

static void ICACHE_FLASH_ATTR ActionTask(os_event_t *events){


}

static void ICACHE_FLASH_ATTR IdleTask(os_event_t *events){


    os_printf("Starting Wifi Scan:\n");
    wifi_station_scan(NULL,scan_done);

}

void ICACHE_FLASH_ATTR init_tasks(){
    
    os_printf("Initialisation Complete. Starting Tasks\n");
    //add tasks
    system_os_task(TopTask, TopTaskPrio, TopTaskQueue, TopTaskQueueLen);
    system_os_task(ActionTask, ActionTaskPrio, ActionTaskQueue, ActionTaskQueueLen);
    system_os_task(IdleTask, IdleTaskPrio, IdleTaskQueue, IdleTaskQueueLen);
    //after init done, start idle task loop
    system_os_post(IdleTaskPrio, 0, 0);
}


void user_rf_pre_init(void)
{
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    os_printf("SDK version:%s\n", system_get_sdk_version());

    wifi_set_opmode(STATION_MODE);
    system_init_done_cb(init_tasks); 

}

