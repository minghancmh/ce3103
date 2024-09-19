#include "playSong.c"
#include "system.h"
#include "stm32f4xx_rcc.h"

static u8 *song =
    (uint8_t *)"e2,d2,e2,d2,e2,B2,d2,c2,A2_C2,E2,A2,B2_E2,G2,B2,c4_E2,e2,d2,e2,"
               "d2,e2,B2,d2,c2,A2_C2,E2,A2,B2_E2,c2,B2,A4";

void BUTTON_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // Enable APB Clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);    //Enable APB Clock


  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN; // button Pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //??? TODO
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50M
  GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);      // Initialize BUTTON GPIO
  GPIO_SetBits(BUTTON_PORT, BUTTON_PIN);
}

void button_task(void *pvParameters) {
  BUZ = 0;
  while (1) {
    if (BUTTON == 0) {
      playSong(song);
    }
		else {
			delay_ms(100);
		}}
}