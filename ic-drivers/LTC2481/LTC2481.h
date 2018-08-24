#ifndef LTC2481_H
#define LTC2481_H

/**
 * Includes
 */

#include "find_number.h"

/**
 * Defines
 */

// ********* LTC2481 Commands ********************
// OR bitwise with address for read or write
#define READ      0x01
#define WRITE     0x00

// ********* LTC2481 Addresses ********************
//  TC2481 configurable Address Assignment 
//    CA1         CA0/f0      Address 
//    LOW         HIGH        001 01 00
//    LOW         Floating    001 01 01
//    Floating    Floating    010 01 00
//    HIGH        HIGH        010 01 10
//    HIGH        Floating    010 01 11
//
//    Global Address          111 01 11



// ********* LTC2481 Configuration *********
//These settings are OR together to assemble the 8 bit config byte.

// ********* LTC2481 Amplifaction Gains *********
// avaidable Gains depend on Speed setting
#define GAIN_1_1        0b00000000   // G = 1   (SPD = 0), G = 1   (SPD = 1)
#define GAIN_4_2        0b00100000   // G = 4   (SPD = 0), G = 2   (SPD = 1)
#define GAIN_8_4        0b01000000   // G = 8   (SPD = 0), G = 4   (SPD = 1)
#define GAIN_16_8       0b01100000   // G = 16  (SPD = 0), G = 8   (SPD = 1)
#define GAIN_32_16      0b10000000   // G = 32  (SPD = 0), G = 16  (SPD = 1)
#define GAIN_64_32      0b10100000   // G = 64  (SPD = 0), G = 32  (SPD = 1)
#define GAIN_128_64     0b11000000   // G = 128 (SPD = 0), G = 64  (SPD = 1)
#define GAIN_256_128    0b11100000   // G = 256 (SPD = 0), G = 128 (SPD = 1)

// ********* LTC2481 ADC source *********
// differential input or PTAT (Temperature) circuit

#define VIN    0b00000000
#define PTAT   0b00001000

// ********* LTC2481 Rejection frequency *********//
#define Reject_50Hz    0b00000010
#define Reject_55Hz    0b00000000
#define Reject_60Hz    0b00000100

// ********* LTC2481 DataRate and Calibration********************
// Speed setting is set by bit 7 in the configuration byte
#define Low_Speed_7_5_SPS   0b00000000 // Low output rate with autozero
#define High_Speed_15_SPS   0b00000001 // High output rate without autozero


// ********* Define external ADC IC inputs ********* 
#define I2C_clkin 100000                            // 100 khz (0 to 400khz) I2C Bus Frequency
#define f_clkin_ADC_internal 125000                 // 125 kHz The internal default frequency source for the LTC2481 chip
#define f_clkin_ADC_external_applied 800000         // 800 khz (10 to 1000 kHz) The external frequency source for the LTC2481 chip if applied

#define MasterClockPeriod 1/f_clkin_ADC_applied     // Time period for one clock cylce
#define f_clkin_ADC_function_scaling_factor f_clkin_ADC_applied/f_clkin_ADC_nominal     // Factor for scaling all chip functions wich are linear tied to f_clin frequency source

#define ADC_ReferenceVoltage 4096                   // The voltage of the reference source in mV

class LTC2481
{

public:

    /**
     * Constructor.
     */
    LTC2481(PinName _mosi, PinName _miso, PinName _sclk, PinName _datardy, PinName _csadc);


    LTC2481();

     
    

    int32_t ADC_Data_Rate_select(int32_t samplerate_requested);
    

    void stop_continuous_mode();
    void start_continuous_mode();

    uint8_t ADC_Control(uint8_t gain_setting);    
    int32_t ADS_read_data();
    //uint8_t A+D_Control(uint8_t gain_setting, uint8_t sensor_detect, uint8_t clock-out_setting);

    int32_t samplerate_set; // returns the samplerate currently configured in the ADC


    void ADC_Select(bool selected);
    void ADC_hard_reset();
    void ADC_reset_values();

    uint8_t ADC_Amplification_Gain_select(uint8_t ADC_Gain_requested) // user requests a specific Gain setting
    uint32_t ADC_Meassurement_Range(uint32_t adc_input_range_requested) //user requests an input range in mV

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
