#include "bmp390.h"
#include <math.h>

extern SPI_HandleTypeDef hspi1; 
extern const uint16_t CS; 

uint32_t get_raw_pressure(); 
uint32_t get_raw_temperature(); 

void BMP390_init(uint8_t osr_t, uint8_t osr_p, uint8_t iir) {
    const float par_t1 = 256.0f;            // 2^8
    const float par_t2 = 9.31322575e-10f;   // 2^-30
    const float par_t3 = 3.55271368e-15f;   // 2^-48
    const float par_p1 = 9.53674316e-07f;   // 2^-20
    const float par_p2 = 1.86264515e-09f;   // 2^-29
    const float par_p3 = 2.32830644e-10f;   // 2^-32
    const float par_p4 = 7.27595761e-12f;   // 2^-37
    const float par_p5 = 8.0f;              // 2^3
    const float par_p6 = 1.5625e-02f;       // 2^-6
    const float par_p7 = 3.90625e-03f;      // 2^-8
    const float par_p8 = 3.05175781e-05f;   // 2^-15
    const float par_p9 = 3.55271368e-15f;   // 2^-48
    const float par_p10 = 3.55271368e-15f;  // 2^-48
    const float par_p11 = 2.77555756e-20f;  // 2^-65


    BMP390_writedata(0x7E, 0xB6);  // The following while sentence will not break without this line
    while(!((BMP390_readdata(0x03)) == 0b00010000));  // p32

    // P28
    printf("0v0\n");  // I don't know why but we can't get correct pressure without this line
    // printf("inside_init: %d\n", (int)BMP390_readdata(0x3A)); 
    NVM_PAR_P11 = (short)(BMP390_readdata(0x45));
    NVM_PAR_P10 = (short)(BMP390_readdata(0x44));
    NVM_PAR_P9 = (short)(BMP390_readdata(0x43)<<8 | BMP390_readdata(0x42));
    NVM_PAR_P8 = (short)(BMP390_readdata(0x41));
    NVM_PAR_P7 = (short)(BMP390_readdata(0x40));
    NVM_PAR_P6 = (unsigned short)(BMP390_readdata(0x3F)<<8 | BMP390_readdata(0x3E));
    NVM_PAR_P5 = (unsigned short)(BMP390_readdata(0x3D)<<8 | BMP390_readdata(0x3C));
    NVM_PAR_P4 = (short)(BMP390_readdata(0x3B));
    NVM_PAR_P3 = (short)(BMP390_readdata(0x3A)); 
    NVM_PAR_P2 = (short)(BMP390_readdata(0x39)<<8 | BMP390_readdata(0x38));
    NVM_PAR_P1 = (short)(BMP390_readdata(0x37)<<8 | BMP390_readdata(0x36));
    NVM_PAR_T3 = (short)(BMP390_readdata(0x35));
    NVM_PAR_T2 = (unsigned short)(BMP390_readdata(0x34)<<8 | BMP390_readdata(0x33));
    NVM_PAR_T1 = (unsigned short)(BMP390_readdata(0x32)<<8 | BMP390_readdata(0x31));

    // p55
    PAR_T1 = NVM_PAR_T1 * par_t1;
    PAR_T2 = NVM_PAR_T2 * par_t2;
    PAR_T3 = NVM_PAR_T3 * par_t3;
    PAR_P1 = (NVM_PAR_P1 - pow(2.0, 14.0)) * par_p1;
    PAR_P2 = (NVM_PAR_P2 - pow(2.0, 14.0)) * par_p2;
    PAR_P3 = NVM_PAR_P3 * par_p3;
    PAR_P4 = NVM_PAR_P4 * par_p4;
    PAR_P5 = NVM_PAR_P5 * par_p5;
    PAR_P6 = NVM_PAR_P6 * par_p6;
    PAR_P7 = NVM_PAR_P7 * par_p7;
    PAR_P8 = NVM_PAR_P8 * par_p8;
    PAR_P9 = NVM_PAR_P9 * par_p9;
    PAR_P10 = NVM_PAR_P10 * par_p10;
    PAR_P11 = NVM_PAR_P11 * par_p11;
    
    OSR_T=osr_t;
    OSR_P=osr_p;
    IIR=iir;

    // p36 Enable pressure and temperature and select forced mode
    BMP390_writedata(0x1B, 0b00010011);  

    BMP390_writedata(0x1C, (OSR_T<<3)|(OSR_P));  // p37 
    BMP390_writedata(0x1F, (IIR<<1));  // p39

    printf("Init completed! \n"); 
}

uint8_t BMP390_readdata(uint8_t adr) {
    // Datasheet p43
    uint8_t reg = adr | 0x80; 
    uint8_t dummy = 0x00;
    uint8_t val = 0x00;

    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &dummy, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &val, 1, HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_SET);

    return val; 
}

void BMP390_writedata(uint8_t adr, uint8_t data) {
    // Datasheet p42
    uint8_t reg = adr & 0x7F; 
    
    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_SET);
}

float BMP390_compensate_temperature(uint32_t uncomp_temp)
{
    float partial_data1;
    float partial_data2;

    partial_data1 = (float)(uncomp_temp - PAR_T1);
    partial_data2 = (float)(partial_data1 * PAR_T2);

    /* Update the compensated temperature in calib structure since this is
     * needed for pressure calculation */
    t_lin = partial_data2 + (partial_data1 * partial_data1) * PAR_T3;

    /* Returns compensated temperature */
    return t_lin;
}

float BMP390_compensate_pressure(uint32_t uncomp_press){
    //Variable to store the compensated pressure 
    float comp_press;
    //Temporary variables used for compensation
    float partial_data1;
    float partial_data2;
    float partial_data3;
    float partial_data4;
    float partial_out1;
    float partial_out2;
    //Calibration data
    partial_data1 = PAR_P6 * t_lin;
    partial_data2 = PAR_P7 * (t_lin * t_lin);
    partial_data3 = PAR_P8 * (t_lin * t_lin * t_lin);
    partial_out1 = PAR_P5 + partial_data1 + partial_data2 + partial_data3;

    partial_data1 = PAR_P2 * t_lin;
    partial_data2 = PAR_P3 * (t_lin * t_lin);
    partial_data3 = PAR_P4 * (t_lin * t_lin * t_lin);
    partial_out2 = (float)uncomp_press * (PAR_P1 + partial_data1 + partial_data2 + partial_data3);

    partial_data1 = (float)uncomp_press * (float)uncomp_press;
    partial_data2 = PAR_P9 + PAR_P10 * t_lin;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + ((float)uncomp_press * (float)uncomp_press * (float)uncomp_press) * PAR_P11;
    comp_press = partial_out1 + partial_out2 + partial_data4;
    

    return comp_press;
    
}

uint32_t get_raw_pressure() {
    // p32
    uint8_t reg = 0x04 | 0x80;
    uint8_t dummy = 0x00;
    uint8_t data0 = 0x00, data1 = 0x00, data2 = 0x00;

    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &dummy, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &data0, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &data1, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &data2, 1, HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_SET);

    return (data2<<16) | (data1<<8) | (data0); 
}

uint32_t get_raw_temperature() {
    // p32
    uint8_t reg = 0x07 | 0x80;
    uint8_t dummy = 0x00;
    uint8_t data0 = 0x00, data1 = 0x00, data2 = 0x00;

    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &dummy, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &data0, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &data1, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, &data2, 1, HAL_MAX_DELAY);
    
    HAL_GPIO_WritePin(GPIOA, CS, GPIO_PIN_SET);

    return (data2<<16) | (data1<<8) | (data0); 
}

int BMP390_update() {
    if (BMP390_readdata(0x03) == 0b01110000) {  // P32
        uint32_t raw_press = get_raw_pressure(); 
        uint32_t raw_temp = get_raw_temperature(); 
        
        // The same as latter part of init
        // p36 Enable pressure and temperature and select forced mode
        BMP390_writedata(0x1B, 0b00010011);  

        BMP390_writedata(0x1C, (OSR_T<<3)|(OSR_P));  // p37 
        BMP390_writedata(0x1F, (IIR<<1));  // p39

        press = BMP390_compensate_pressure(raw_press); 
        temp = BMP390_compensate_temperature(raw_temp); 

        return 1; 
    }
    else {
        return 0;  
    }
}

float BMP390_get_pressure() {
    return press; 
}

float BMP390_get_temperature() {
    return temp; 
}