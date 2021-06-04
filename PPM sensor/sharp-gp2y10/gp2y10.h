#ifndef __GP2Y10_H__
#define __GP2Y10_H__

// ========== HAL ========== //
#define PIN_LED   4
#define PIN_PPM   A0

// Pin position values
#define PPM_LED_ON  LOW
#define PPM_LED_OFF HIGH

// Time parameters in uS
#define TIME_HIGH     320
#define TIME_TOSAMPLE 280
#define TIME_PERIOD   10000

void Sharp_gp2y10_init();

#endif /*   __GP2Y10_H__   */
