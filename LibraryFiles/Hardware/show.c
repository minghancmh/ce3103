#include "show.h"

void show_task(void *pvParameters)
{
   u32 lastWakeTime = getSysTickCnt();
   while(1)
    { 

   vTaskDelayUntil(&lastWakeTime, F2T(RATE_50_HZ));
   oled_show();

   //oled_show_new();    
    }
}  

void oled_show(void)
{  
     //To DO
  
  
  //call some function in oled to display something



   //OLED_Refresh_Gram(); 
 
 OLED_Display_On();
 uint32_t elapsed_time = 0;  // Track elapsed time in seconds

    while(1)  // Infinite loop to keep the timer running
    {
        uint32_t minutes = elapsed_time / 60;  
        uint32_t seconds = elapsed_time % 60;  

      
        OLED_Clear();
        OLED_ShowNumber(0, 0, minutes, 2, 12); 
        OLED_ShowChar(16, 0, ':', 12, 1);      
        OLED_ShowNumber(24, 0, seconds, 2, 12);
   
   char s[] = "Chan Ming Han";
  
   OLED_ShowString(0,10, s);

        
        OLED_Refresh_Gram();

        vTaskDelay(1000 / portTICK_PERIOD_MS); 
        
        elapsed_time++;
    }
 


     
 }