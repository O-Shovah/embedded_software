/**
 *  Includes
 */

#include "LTC2481.h"


/*
*   Class initialisation
*/

LTC2481::LTC2481(PinName _sda, PinName _scl, char _CA0, char _CA1) : I2C_(_sda, _scl), Addressconfig_ (_CA0, _CA1)
{

    //Initialize ADC state variable
    ADC_reseted = false;
    ADC_DRDY_interrupted = false;
    ADC_data_already_read = false;
    Chip_Select_ = true;

    //Initialize the available values for the Rate selection
    ADC_Data_Rate_init();

    //Create Object to find the feasible Data Rate value
    ADC_Find_Data_Rate = find_number (ADC_Datarates_avaidable,ADC_Number_of_Datarates_avaidable);

    //Initialize the available values for the Gain selection
    ADC_Amplification_Gain_init();

	//Create Object to find the feasible Gain value
    ADC_Find_Gain = find_number (ADC_Amplification_Gains_avaidable,ADC_Number_of_Gains_avaidable);


    //set up the SPI Bus
    spi_.format(8,1);
    spi_.frequency(spi_clkin);

    //Soft reset the LTC2481
    ADC_reset_values();
    //wait(100 * (float)MasterClockPeriod);
}

/*
*   Class Functions
*/

//select the LTC2481 chip on the SPI
void LTC2481::ADC_Select(bool selected)
{
    Chip_Select_ = selected;
}

//Fallig Edge of the !DATARDY Pin indicates ADS ready for Data Read
void LTC2481::Dataready_set()
{
    ADC_DRDY_interrupted = true;
    ADC_reseted = false;
    ADC_data_already_read = false;
}

//Rising Edge of the !DATARDY Pin indicates ADS not ready for Data Read
void LTC2481::Dataready_unset()
{
    ADS_DRDY_interrupted = false;
    ADC_data_already_read = true;
}

//Pulls down the !RESET pin on the ADS to trigger hardware reset
void LTC2481::ADC_hard_reset()
{
    spi_.write(ADS_RESET); // TBD: to be routed to hardware pin
    ADC_DRDY_interrupted = false;
    ADC_reseted = true;
    ADC_data_already_read = true;
}

//This command byte resets all ADS settings to default
void LTC2481::ADC_reset_values()
{
    spi_.write(ADS_RESET);
    ADC_DRDY_interrupted = false;
    ADC_reseted = true;
    ADC_data_already_read = true;
}

//Load the avaidable Samplerates into an array to be searched later
bool LTC2481::ADC_Samplerate_init()
{
	ADC_Samplerates_avaidable[0] = 7;
    ADC_Samplerates_avaidable[1] = 15;

    ADC_Number_of_Samplerates_avaidable = (sizeof(ADC_Samplerates_avaidable)/sizeof(*ADC_Samplerates_avaidable));

    return 0;
}

int32_t LTC2481::ADC_Samplerate_select(int32_t samplerate_requested)
{
    uint32_t samplerate_commanded = 0;

    samplerate_commanded = ADC_Find_Samplerate.find_closest(samplerate_requested);

    switch(samplerate_commanded) {

        case 7 :
            samplerate_set = 0b00000000; //2.5SPS

        case 15 :
            samplerate_set = 0b00000001; //5SPS

        default :
            samplerate_set = 0b00000000; // Error Message

    }

    ADC_set_settings(ADC_address);

    ADC_reseted = true;

    return samplerate_set;
}

//Select the appropriate Gain setting for maximising  ADC resolution for a voltage in mV given by the user.
uint32_t LTC2481::ADC_Meassurement_Range(uint32_t adc_input_range_requested)
{
    range_gain_requested = (0.5*ADC_ReferenceVoltage) / adc_input_range_requested  // Range of LTC2481 is +/- 0.5*Vref

    range_gain_set = ADC_Amplification_Gain_select(range_gain_requested);

    input_range_set = (0.5 * ADC_ReferenceVoltage) / range_gain_set ;

    return input_range_set;
}

//Load the avaidable Gains into an arry to be searched later
bool LTC2481::ADC_Amplification_Gain_init()
{
	ADC_Amplification_Gains_avaidable[0] = 1
	ADC_Amplification_Gains_avaidable[1] = 4
	ADC_Amplification_Gains_avaidable[2] = 8
	ADC_Amplification_Gains_avaidable[3] = 16
	ADC_Amplification_Gains_avaidable[4] = 32
	ADC_Amplification_Gains_avaidable[5] = 64
	ADC_Amplification_Gains_avaidable[6] = 128
    ADC_Amplification_Gains_avaidable[7] = 256

	ADC_Number_of_Gains_avaidable = (sizeof(ADC_Amplification_Gains_avaidable)/sizeof(*ADC_Amplification_Gains_avaidable));

	return 0;
}

//set up the avaidable Gain settings in an Array
//find the smaller gain fitting to stay on safe range side
//set the Gain setting byte in the ADC
uint8_t LTC2481::ADC_Amplification_Gain_select(uint8_t ADC_Gain_requested)
{
    uint32_t gain_commanded = 0;

    gain_commanded = ADC_Find_Gain.find_smaller(Gain_requested);

    if (samplerate_set == 0b00000001)
    {
    gain_commanded = gain_commanded << 1
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

        ADC_Control(ADS_DRATE, gain_set);

return gain_set
}

//Assemble the settings byte from private variables. Then write the settings to the passed address.
void LTC2481::ADC_set_settings(uint8_t ADC_address)
{
    uint8_t result = 0;
    uint8_t ADC_settings = 0;

    ADC_settings = ( samplerate_set | gain_set | rejection_set | temperatureread_set)

    I2C.start;
    result = I2C.write(ADC_address | 0 );

    if (result == 0)
    {
    result = I2C.write(ADC_settings);
    }

    I2C.stop;

}

//Wait for ADC not reset or timout.Then read back the setting of the specified byte
void LTC2481::ADC_read_settings(uint8_t ADC_address)
{
    uint8_t i=0;
    while((ADC_reseted == true) && (i<700)) {

        wait_us(1);
        i++;
    }
    if(ADC_reseted == false) {

        spi_.write(ADS_RREG | ADS_register);    // 0101 0011
        spi_.write(0x00);                       // 0 Byte
        uint8_t setting = spi_.write(0x00);

        ADC_reseted == true;
    }
    return(setting_set);
}

//directly issu the binary byte to be written into the Control register
uint8_t LTC2481::ADC_direct_Control(uint8_t adc_control_register)
{

    set_settings(ADS_ADCON,adc_control_register);
    ADC_reseted = true;
    return 0;

}

//Compose the byte to be written into the Control register
uint8_t LTC2481::ADC_Control(uint8_t gain_setting, uint8_t sensor_detect, uint8_t clockout_setting)
{
    uint8_t adc_control_register = (clockout_setting << 7) | (sensor_detect << 5) | (gain_setting << 3);

    set_settings(ADS_ADCON,adc_control_register);

    ADC_reseted = true;

    return adc_control_register;
}

//set up the AS1255 to return data on every Data Read
void LTC2481::Command_start_continuous_mode()
{
    uint8_t i=0;
    while((ADC_reseted == true) && (i<700)) {

        wait_us(1);
        i++;
    }
    if(ADC_reseted == false) {
        spi_.write(ADS_RDATAC);
        wait(50 * (float)MasterClockPeriod);         // Wait LTC_2481 x ClockCycles (t6)

        ADC_reseted == true;

    }

}

//Stop the LTC2481 from returning on every Data Read  
void LTC2481::Command_stop_continuous_mode()
{
    uint8_t i=0;
    while((ADC_reseted == true) && (i<700)) {

        wait_us(1);
        i++;
    }
    if(ADC_reseted == false) {
        spi_.write(ADS_SDATAC);
        wait(50 * (float)MasterClockPeriod);        // Wait LTC_2481 x ClockCycles (t6)

        ADC_reseted == true;

    }
}

//set up the LTC2481 returning just data from a single conversion  
void LTC2481::Command_single_conversion()
{

}


//Wait for the ADC to be ready for Data Read or Timeout
//Then read data by sending 3 CLK bytes
//Compose Bytes to correct int32_t format
int32_t LTC2481::ADS_read_data()
{
    ADC_data_already_read = true;

    int32_t analog_in = 0;
    uint16_t i=0;

    while((i<700) && ((ADS_DRDY_interrupted == false) || (ADC_data_already_read == true)) ) {

        wait_us(1);
        i++;
    }

    if((ADS_DRDY_interrupted == true) && (ADC_data_already_read == false)) {             // Wait LTC_2481 DataReady-Pin

        uint8_t data0 = spi_.write(0x00);                // MSB
        uint8_t data1 = spi_.write(0x00);                // MidByte
        uint8_t data2 = spi_.write(0x00);                // LSB

        analog_in = ((data0 << 24) + (data1 << 16) + (data2 << 8)) / 256;   // shift all and divide by 0xFF for right sign

        ADC_data_already_read = true;

    }

    return(analog_in);
}