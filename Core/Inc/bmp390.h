#ifndef BMP390_H
#define BMP390_H

#include "main.h"

// pressure //p37
#define OSR_P1 0b000  // *1
#define OSR_P2 0b001  // *2
#define OSR_P4 0b010  // *4
#define OSR_P8 0b011  // *8
#define OSR_P16 0b100  // *16
#define OSR_P32 0b101  // *32

// temperature //p37
#define OSR_T1 0b000  // *1
#define OSR_T2 0b001  // *2
#define OSR_T4 0b010  // *4
#define OSR_T8 0b011  // *8
#define OSR_T16 0b100  // *16
#define OSR_T32 0b101  // *32

// IIR //P39
#define IIR_0 0b000  //off 
#define IIR_4 0b100
#define IIR_16 0b111

//OSR_1(0b001),OSR_4(0b011),IIR_0(0b000)
#define OSR_1 0b001
#define OSR_4 0b100

#define ADD1 0x77 << 1//defalt in case of adafruit BMP390 //p41
#define ADD2 0x76 << 1

unsigned short NVM_PAR_P6, NVM_PAR_P5, NVM_PAR_T2, NVM_PAR_T1;
short NVM_PAR_P11, NVM_PAR_P10, NVM_PAR_P9, NVM_PAR_P8, NVM_PAR_P7, NVM_PAR_P4, NVM_PAR_P3, NVM_PAR_P2, NVM_PAR_P1, NVM_PAR_T3;
float PAR_T1, PAR_T2, PAR_T3;
float PAR_P1, PAR_P2, PAR_P3, PAR_P4, PAR_P5, PAR_P6, PAR_P7, PAR_P8, PAR_P9, PAR_P10, PAR_P11;
uint8_t OSR_T, OSR_P, IIR;
float t_lin;
float press, temp;

void BMP390_init(uint8_t osr_t, uint8_t osr_p, uint8_t iir); 
uint8_t BMP390_readdata(uint8_t adr); 
void BMP390_writedata(uint8_t adr, uint8_t data); 

float BMP390_compensate_temperature(uint32_t uncomp_temp); 
float BMP390_compensate_pressure(uint32_t uncomp_press); 
int BMP390_update(); 
float BMP390_get_pressure(); 
float BMP390_get_temperature(); 

#endif