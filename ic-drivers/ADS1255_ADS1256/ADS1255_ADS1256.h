#ifndef ADS1255_H
#define ADS1255_H

/**
 * Includes
 */
#include "mbed.h"
#include "find_number.h"

/**
 * Defines
 */

// ********* ADS1255/ADS1256 Registers ********************
#define ADS_STATUS   0x00    // ADS1256 Status register 
#define ADS_MUX      0x01    // ADS1256 Multiplexer register 
#define ADS_ADCON    0x02    // ADS1256 ADCON register 
#define ADS_DRATE    0x03    // ADS1256 Data rate register 
#define ADS_IO       0x04    // ADS1256 IO register 
#define ADS_OFC0     0x05    // ADS1256 Offset register 
#define ADS_OFC1     0x06    // ADS1256 Offset register 
#define ADS_OFC2     0x07    // ADS1256 Offset register 
#define ADS_FSC0     0x08    // ADS1256 Scale register 
#define ADS_FSC1     0x09    // ADS1256 Scale register 
#define ADS_FSC2     0x0A    // ADS1256 Scale register 

// ********* ADS1255/ADS1256 Commands ********************
#define ADS_WAKEUP   0x00    // ADS1256 command 
#define ADS_RDATA    0x01    // ADS1256 command 
#define ADS_RDATAC   0x03    // ADS1256 command 
#define ADS_SDATAC   0x0f    // ADS1256 command 
#define ADS_RREG     0x10    // ADS1256 command 
#define ADS_WREG     0x50    // ADS1256 command 
#define ADS_SELFCAL  0xf0    // ADS1256 command 
#define ADS_SELFOCAL 0xf1    // ADS1256 command 
#define ADS_SELFGCAL 0xf2    // ADS1256 command 
#define ADS_SYSOCAL  0xf3    // ADS1256 command 
#define ADS_SYSGCAL  0xf4    // ADS1256 command 
#define ADS_SYNC     0xfc    // ADS1256 command 
#define ADS_STANDBY  0xfd    // ADS1256 command 
#define ADS_RESET    0xfe    // ADS1256 command 
#define ADS_WAKEUP2  0xff    // ADS1256 command

// ********* ADS1255/ADS1256 DataRate ********************
#define ADS_30k_SPS  0b11110000 // 30,000SPS (default)
#define ADS_15k_SPS  0b11100000 // 15,000SPS
#define ADS_7500_SPS 0b11010000 // 7,500SPS
#define ADS_3750_SPS 0b11000000 // 3,750SPS
#define ADS_2000_SPS 0b10110000 // 2,000SPS
#define ADS_1000_SPS 0b10100001 // 1,000SPS
#define ADS_500_SPS  0b10010010 // 500SPS
#define ADS_100_SPS  0b10000010 // 100SPS
#define ADS_60_SPS   0b01110010 // 60SPS
#define ADS_50_SPS   0b01100011 // 50SPS
#define ADS_30_SPS   0b01010011 // 30SPS
#define ADS_25_SPS   0b01000011 // 25SPS
#define ADS_15_SPS   0b00110011 // 15SPS
#define ADS_10_SPS   0b00100011 // 10SPS
#define ADS_5_SPS    0b00010011 // 5SPS
#define ADS_2_5_SPS  0b00000011 // 2.5SPS

// ********* ADS1255/ADS1256 Amplifaction Gains *********
#define ADS_1_PGA  0b000 // PGA 1 (default)
#define ADS_2_PGA  0b001 // PGA 2
#define ADS_4_PGA  0b010 // PGA 4
#define ADS_8_PGA  0b011 // PGA 8
#define ADS_16_PGA 0b100 // PGA 16
#define ADS_32_PGA 0b101 // PGA 32
#define ADS_64_PGA 0b110 // PGA 64
//#define ADS_64_SPS 0b111 // PGA 64

// ********* Define external ADC IC inputs ********* 
#define spi_clkin 1800000                           // mbed SPI Frequency 1.8 Mhz
#define f_clkin_ADC_nominal 7680000                 // 7.68 MHz The intended default frequency source for the ADS1255 chip
#define f_clkin_ADC_applied 8000000                 // 8.00 MHz The acutally used frequency source for the ADS1255 chip

#define MasterClockPeriod 1/f_clkin_ADC_applied     // Time period for one clock cylce
#define f_clkin_ADC_function_scaling_factor f_clkin_ADC_applied/f_clkin_ADC_nominal     // Factor for scaling all chip functions wich are linear tied to f_clin frequency source

#define ADC_ReferenceVoltage 1024                   // The voltage of the reference source in mV

class ADS1255
{

public:

    /**
     * Constructor.
     */
    ADS1255(PinName _mosi, PinName _miso, PinName _sclk, PinName _datardy, PinName _csadc);


    ADS1255();

     
    

    int32_t ADC_Data_Rate_select(int32_t samplerate_requested);
    

    void stop_continuous_mode();
    void start_continuous_mode();

    uint8_t ADC_Control(uint8_t gain_setting);    
    int32_t ADS_read_data();
    //uint8_t A+D_Control(uint8_t gain_setting, uint8_t sensor_detect, uint8_t clock-out_setting);

    int32_t samplerate_set;


    void ADC_Select(bool selected);
    void ADC_hard_reset();
    void ADC_reset_values();

    uint8_t ADC_Amplification_Gain_select(uint8_t ADC_Gain_requested)
    uint32_t ADC_Meassurement_Range(uint32_t adc_input_range_requested)

private:

    //Declare IO Pins
    SPI spi_;
    InterruptIn Datardy_;
    DigitalOut Chip_Select_;


    //Declare ADC state functions
    void Dataready_set();
    void Dataready_unset();

    //Declare ADC state variables
    bool volatile ADS_DRDY_interrupted;
    bool volatile ADC_reseted;
    bool volatile ADC_data_already_read;

    //Initialize Data Rate selection
    bool ADC_Data_Rate_init();
    int32_t ADC_Datarates_avaidable[16];
    int32_t ADC_Number_of_Datarates_avaidable;
    find_number ADC_Find_Data_Rate;

    //Initialize Gain selection
    bool ADC_Amplification_Gain_init();
    int32_t ADC_Amplification_Gains_avaidable[7];
    int32_t ADC_Number_of_Gains_avaidable;
    find_number ADC_Find_Gain;


    //Variables
    int32_t samplerate_commanded;
    
    

    //Internal Functions
    void set_settings(uint8_t ADS_register, uint8_t ADS_value);

    


};


#endif
