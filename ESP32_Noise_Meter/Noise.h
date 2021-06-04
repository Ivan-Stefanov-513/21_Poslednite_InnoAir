#ifndef NOISE_H
#define NOISE_H

#define Mic_Pin 35
#define SAMPLES 10

uint16_t mic_sample[10] = {0};
uint8_t sample_pos = 0;

uint32_t zero = 0, db = 0, noise = 0, noise_avrg = 0;

#define MIC_OFFSET_DB 6
#define MIC_OVERLOAD_DB 120

#endif
