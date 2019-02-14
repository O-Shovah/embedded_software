/**
 *  Includes
 */

#include "LTC2481.h"


/*
*   Class initialisation
*/

LTC2481::LTC2481(PinName _sda, PinName _scl, string _CA0, string _CA1) : I2C_(_sda, _scl), Addresspin1_ (_CA0), Addresspin2_ (_CA1)
{
    //Initialize ADC setting variables
    samplerate_set = 0b00000000;
    gain_set = 0b00000000;
    rejection_set = 0b00000100;
    temperatureread_set = 0b00000000;
    ADC_address_set = 0b0010100;
    input_range_set = 0;
    I2C_clk = 100000;

    //Initialize the available values for the Rate selection
    ADC_Samplerate_init();

    //Create Object to find the feasible Data Rate value
    ADC_Find_Samplerate = find_number (ADC_Samplerates_avaidable,ADC_Number_of_Samplerates_avaidable);

    //Initialize the available values for the Gain selection
    ADC_Amplification_Gain_init();

    //Create Object to find the feasible Gain value
    ADC_Find_Gain = find_number (ADC_Amplification_Gains_avaidable,ADC_Number_of_Gains_avaidable);


    //set up the I2C Bus
    I2C_.frequency(I2C_clk);

    //set up the I2C address
    Set_ADC_Address(Addresspin1_, Addresspin2_);
}


LTC2481::LTC2481(PinName _sda, PinName _scl) : I2C_(_sda, _scl)
{
    //Initialize ADC setting variables
    samplerate_set = 0b00000000;
    gain_set = 0b00000000;
    rejection_set = 0b00000100;
    temperatureread_set = 0b00000000;
    ADC_address_set = 0b0010100;
    input_range_set = 0;
    I2C_clk = 100000;

    //Initialize the available values for the Rate selection
    ADC_Samplerate_init();

    //Create Object to find the feasible Data Rate value
    ADC_Find_Samplerate = find_number (ADC_Samplerates_avaidable,ADC_Number_of_Samplerates_avaidable);

    //Initialize the available values for the Gain selection
    ADC_Amplification_Gain_init();

    //Create Object to find the feasible Gain value
    ADC_Find_Gain = find_number (ADC_Amplification_Gains_avaidable,ADC_Number_of_Gains_avaidable);

    //set up the I2C Bus
    I2C_.frequency(I2C_clk);
}

/*
*   Class Functions
*/



uint8_t LTC2481::Set_ADC_Address(string _CA0, string _CA1)
{

    if (_CA1 == "LOW" && _CA0 == "HIGH") {
        ADC_address_set = 0b00101000;
    }
    if (_CA1 == "LOW" && _CA0 == "FLOATING") {
        ADC_address_set = 0b00101010;
    }
    if (_CA1 == "FLOATING" && _CA0 == "HIGH") {
        ADC_address_set = 0b01001110;
    }
    if (_CA1 == "FLOATING" && _CA0 == "FLOATING") {
        ADC_address_set = 0b01001000;
    }
    if (_CA1 == "HIGH" && _CA0 == "HIGH") {
        ADC_address_set = 0b01001100;
    }
    if (_CA1 == "HIGH" && _CA0 == "FLOATING") {
        ADC_address_set = 0b01001110;
    }

    return(ADC_address_set);
}

void LTC2481::Command_synchronise_all_ADC()
{
    ADC_address_set = 0b11101110;
}

//Assemble the settings byte from private variables. Then write the settings to the passed address.
uint8_t LTC2481::ADC_set_settings()
{
    uint8_t result = 0;
    uint8_t ADC_settings = 0;

    ADC_settings = ( gain_set | temperatureread_set | rejection_set | samplerate_set );

    I2C_.start();
    result = I2C_.write(ADC_address_set | 0b00000000 );

    if (result == 0b00000001) {
        result = I2C_.write(ADC_settings);
    }

    I2C_.stop();

    return(result);
}

//Load the avaidable Samplerates into an array to be searched later
bool LTC2481::ADC_Samplerate_init()
{
    ADC_Samplerates_avaidable[0] = 7;
    ADC_Samplerates_avaidable[1] = 15;

    ADC_Number_of_Samplerates_avaidable = (sizeof(ADC_Samplerates_avaidable)/sizeof(*ADC_Samplerates_avaidable));

    return 0;
}

uint32_t LTC2481::Request_samplerate(uint32_t samplerate_requested)
{
    uint32_t samplerate_commanded = 0;

    samplerate_commanded = ADC_Find_Samplerate.find_closest(samplerate_requested);

    switch(samplerate_commanded) {

        case 7 :
            samplerate_set = 0b00000000; //7.5SPS
            break;

        case 15 :
            samplerate_set = 0b00000001; //15SPS
            break;

        default :
            samplerate_set = 0b00000000; // Error Message
            break;

    }

    return(samplerate_set);
}

//Select the appropriate Gain setting for maximising  ADC resolution for a voltage in mV given by the user.
uint32_t LTC2481::Request_Meassurement_Range(uint32_t adc_input_range_requested)
{
    range_gain_requested = (0.5*ADC_ReferenceVoltage) / adc_input_range_requested;  // Range of LTC2481 is +/- 0.5*Vref

    range_gain_set = Request_Amplification_Gain(range_gain_requested);

    input_range_set = (0.5 * ADC_ReferenceVoltage) / range_gain_set ;

    return (input_range_set);
}

//Load the avaidable Gains into an arry to be searched later
bool LTC2481::ADC_Amplification_Gain_init()
{
    ADC_Amplification_Gains_avaidable[0] = 1;
    ADC_Amplification_Gains_avaidable[1] = 4;
    ADC_Amplification_Gains_avaidable[2] = 8;
    ADC_Amplification_Gains_avaidable[3] = 16;
    ADC_Amplification_Gains_avaidable[4] = 32;
    ADC_Amplification_Gains_avaidable[5] = 64;
    ADC_Amplification_Gains_avaidable[6] = 128;
    ADC_Amplification_Gains_avaidable[7] = 256;

    ADC_Number_of_Gains_avaidable = (sizeof(ADC_Amplification_Gains_avaidable)/sizeof(*ADC_Amplification_Gains_avaidable));

    return 0;
}

//set up the avaidable Gain settings in an Array
//find the smaller gain fitting to stay on safe range side
//set the Gain setting byte in the ADC
uint8_t LTC2481::Request_Amplification_Gain(uint8_t ADC_Gain_requested)
{
    uint32_t gain_commanded = 0;

    gain_commanded = ADC_Find_Gain.find_smaller(gain_requested);

    if (samplerate_set == 0b00000001) {
        gain_commanded = gain_commanded << 1;
    }

    switch(gain_commanded) {

        case 1 :
            gain_set = 0b00000000; // G = 1   (SPD = 0), G = 1   (SPD = 1)

        case 4 :
            gain_set = 0b00100000; // G = 4   (SPD = 0), G = 2   (SPD = 1)

        case 8 :
            gain_set = 0b01000000; // G = 8   (SPD = 0), G = 4   (SPD = 1)

        case 16 :
            gain_set = 0b01100000; // G = 16  (SPD = 0), G = 8   (SPD = 1)

        case 32 :
            gain_set = 0b10000000; // G = 32  (SPD = 0), G = 16  (SPD = 1)

        case 64 :
            gain_set = 0b10100000; // G = 64  (SPD = 0), G = 32  (SPD = 1)

        case 128 :
            gain_set = 0b11000000; // G = 128 (SPD = 0), G = 64  (SPD = 1)

        case 256 :
            gain_set = 0b11100000; // G = 256 (SPD = 0), G = 128 (SPD = 1)

        default :
            gain_set = 0b00000000; // PGA 1 (default)
    }

    return (gain_set);
}


//Write back the setting of the specified parameter
uint32_t LTC2481::Read_samplerate_set()
{
    return(samplerate_set);
}

//Write back the setting of the specified parameter
uint8_t LTC2481::Read_Amplification_Gain()
{
    return(gain_set);
}

//Write back the setting of the specified parameter
uint32_t LTC2481::Read_Meassurement_Range()
{
    return(input_range_set);
}

//Write back the setting of the specified parameter
uint8_t LTC2481::Read_ADC_Address()
{
    return(ADC_address_set);
}

//Wait for the ADC to be ready for Data Read or Timeout
//Then read data by sending 3 CLK bytes
//Compose Bytes to correct int32_t format
int32_t LTC2481::Read_read_ADC()
{

    uint8_t result = 0;

    int32_t analog_in = 0;
    uint32_t analog_in_long = 0;
    uint16_t analog_in_unsigned;

    uint8_t sign = 0;

    uint8_t data0;
    uint8_t data1;
    uint8_t data2;


    I2C_.start();
    result = I2C_.write(ADC_address_set | 0b00000001 );

    if (result == 0b00000001) {
        data0 = I2C_.read(result);      // MSB
        data1 = I2C_.read(result);      // MidByte
        data2 = I2C_.read(result);      // LSB
        I2C_.stop();
    }

    sign = data0 & 0b10000000;

    //data2 = data2 & 0b11000000;

    analog_in_unsigned = ((data0 << 10) + (data1 << 2) + (data2 >> 6));

    analog_in_long = analog_in_unsigned;

    switch(sign) {

        case 0 : {
            analog_in = (analog_in + analog_in_unsigned);
        }

        case 1 : {
            analog_in = (analog_in - analog_in_unsigned);

        }

    }

    return(analog_in_long);
}