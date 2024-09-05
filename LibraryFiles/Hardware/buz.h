#ifndef __BUZ_H
#define __BUZ_H
#include "sys.h"
#include "system.h"

#define BUZ_TASK_PRIO		3     //Task priority 
#define BUZ_STK_SIZE 128  //Task stack size 


/*--------BUZZER control pin--------*/
#define BUZ_PORT GPIOB
#define BUZ_PIN GPIO_Pin_10
#define BUZ PBout(10) 
/*----------------------------------*/

void BUZ_Init(void);  
void buz_task(void *pvParameters);
extern int Buz_Count;
#endif