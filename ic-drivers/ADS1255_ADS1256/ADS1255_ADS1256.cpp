/**
 * Includes
 */
#include "ADS1255.h"

ADS1255::ADS1255(PinName _mosi, PinName _miso, PinName _sclk, PinName _datardy, PinName _csadc) : spi_(_mosi, _miso, _sclk), Datardy_(_datardy), Chip_Select_(_csadc)
{
	//set up Interrupt pins for the Dataready line
    Datardy_.fall(this, &ADS1255::Dataready_set);
    Datardy_.rise(this, &ADS1255::Dataready_unset);

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

    //Soft reset the ADS1255
    ADC_reset_values();
    //wait(100 * (float)MasterClockPeriod);
}

//select the ADS1255 chip on the SPI
void ADS1255::ADC_Select(bool selected)
{
    Chip_Select_ = selected;
}

//Fallig Edge of the !DATARDY Pin indicates ADS ready for Data Read
void ADS1255::Dataready_set()
{
    ADC_DRDY_interrupted = true;
    ADC_reseted = false;
    ADC_data_already_read = false;
}

//Rising Edge of the !DATARDY Pin indicates ADS not ready for Data Read
void ADS1255::Dataready_unset()
{
    ADS_DRDY_interrupted = false;
    ADC_data_already_read = true;
}

//Pulls down the !RESET pin on the ADS to trigger hardware reset
void ADS1255::ADC_hard_reset()
{
    spi_.write(ADS_RESET); // TBD: to be routed to hardware pin
    ADC_DRDY_interrupted = false;
    ADC_reseted = true;
    ADC_data_already_read = true;
}

//This command byte resets all ADS settings to default
void ADS1255::ADC_reset_values()
{
    spi_.write(ADS_RESET);
    ADC_DRDY_interrupted = false;
    ADC_reseted = true;
    ADC_data_already_read = true;
}

//Load the avaidable Datarates into an arry to be searchd later
bool ADS1255::ADC_Data_Rate_init()
{
	ADC_Datarates_avaidable[0] = 2;
    ADC_Datarates_avaidable[1] = 5;
    ADC_Datarates_avaidable[2] = 10;
    ADC_Datarates_avaidable[3] = 15;
    ADC_Datarates_avaidable[4] = 25;
    ADC_Datarates_avaidable[5] = 30;
    ADC_Datarates_avaidable[6] = 50;
    ADC_Datarates_avaidable[7] = 60;
    ADC_Datarates_avaidable[8] = 100;
    ADC_Datarates_avaidable[9] = 500;
    ADC_Datarates_avaidable[10] = 1000;
    ADC_Datarates_avaidable[11] = 2000;
    ADC_Datarates_avaidable[12] = 3750;
    ADC_Datarates_avaidable[13] = 7500;
    ADC_Datarates_avaidable[14] = 15000;
    ADC_Datarates_avaidable[15] = 30000;

    ADC_Number_of_Datarates_avaidable = (sizeof(ADC_Datarates_avaidable)/sizeof(*ADC_Datarates_avaidable));

    return 0;
}

int32_t ADS1255::ADC_Data_Rate_select(int32_t samplerate_requested)
{

    samplerate_commanded = ADC_Find_Data_Rate.find_closest(samplerate_requested);

    switch(samplerate_commanded) {

        case 2 :
            samplerate_set = 0b00000011; //2.5SPS

        case 5 :
            samplerate_set = 0b00010011; //5SPS

        case 10 :
            samplerate_set = 0b00100011; //10SPS

        case 15 :
            samplerate_set = 0b00110011; //15SPS

        case 25 :
            samplerate_set = 0b01000011; //25SPS

        case 30 :
            samplerate_set = 0b01010011; //30SPS

        case 50 :
            samplerate_set = 0b01100011; //50SPS

        case 60 :
            samplerate_set = 0b01110010; //60SPS

        case 100 :
            samplerate_set = 0b10000010; //100SPS

        case 500 :
            samplerate_set = 0b10010010; //500SPS

        case 1000 :
            samplerate_set = 0b10100001; //1000SPS

        case 2000 :
            samplerate_set = 0b10110000; //2000SPS

        case 3750 :
            samplerate_set = 0b11000000; //3750SPS

        case 7500 :
            samplerate_set = 0b11010000; //7500SPS

        case 15000 :
            samplerate_set = 0b11100000; //15000SPS

        case 30000 :
            samplerate_set = 0b11110000; //30000SPS

        default :
            samplerate_set = 0; // Error Message

    }

    set_settings(ADS_DRATE, samplerate_set);

    ADC_reseted = true;

    return samplerate_commanded;
}

//Select the appropriate Gain setting for maximising  ADC resolution for a voltage in mV given by the user.
uint32_t ADS1255::ADC_Meassurement_Range(uint32_t adc_input_range_requested)
{
    range_gain_requested = (2*ADC_ReferenceVoltage) / adc_input_range_requested  // Range of ADS1255 is +/- 2*Vref

    range_gain_set = ADC_Amplification_Gain_select(range_gain_requested);

    input_range_set = (2 * ADC_ReferenceVoltage) / range_gain_set ;

    return input_range_set;
}

//Load the avaidable Gains into an arry to be searchd later
bool ADS1255::ADC_Amplification_Gain_init()
{
	ADC_Amplification_Gains_avaidable[0] = 1
	ADC_Amplification_Gains_avaidable[1] = 2
	ADC_Amplification_Gains_avaidable[2] = 4
	ADC_Amplification_Gains_avaidable[3] = 8
	ADC_Amplification_Gains_avaidable[4] = 16
	ADC_Amplification_Gains_avaidable[5] = 32
	ADC_Amplification_Gains_avaidable[6] = 64

	ADC_Number_of_Gains_avaidable = (sizeof(ADC_Amplification_Gains_avaidable)/sizeof(*ADC_Amplification_Gains_avaidable));

	return 0;
}

//set up the avaidable Gain settings in an Array
//find the smaller gain fitting to stay on safe range side
//set the Gain setting byte in the ADS
uint8_t ADS1255::ADC_Amplification_Gain_select(uint8_t ADC_Gain_requested)
{
gain_commanded = ADC_Find_Gain.find_smaller(Gain_requested);

switch(gain_commanded) {

        case 1 :
            gain_set = 0b000; // PGA 1 (default)

        case 2 :
            gain_set = 0b001; // PGA 2

        case 4 :
            gain_set = 0b010; // PGA 4

        case 8 :
            gain_set = 0b011; // PGA 8

        case 16 :
            gain_set = 0b100; // PGA 16

        case 32 :
            gain_set = 0b101; // PGA 32

        case 64 :
            gain_set = 0b110;  // PGA 64

        default :
        gain_set = 0b000; // PGA 1 (default)

        //ADS_PGA_64 0b111 // PGA 64

        ADC_Control(ADS_DRATE, gain_set);

return gain_commanded
}

//Wait for ADC not reset or timout. Then write a setting register
void ADS1255::ADC_set_settings(uint8_t ADS_register, uint8_t ADS_value)
{
    uint8_t i=0;
    while((ADC_reseted == true) && (i<700)) {

        wait_us(1);
        i++;
    }
    if(ADC_reseted == false) {

        spi_.write(ADS_WREG | ADS_register);    // 0101 0011
        spi_.write(0x00);                       // 0 Byte
        spi_.write(ADS_value);                  // 1000 SPS

        ADC_reseted == true;
    }
}

//Wait for ADC not reset or timout.Then read back the setting of the specified byte
void ADS1255::ADC_read_settings(uint8_t ADS_register)
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
uint8_t ADS1255::ADC_direct_Control(uint8_t adc_control_register)
{

    set_settings(ADS_ADCON,adc_control_register);
    ADC_reseted = true;
    return 0;

}

//Compose the byte to be written into the Control register
uint8_t ADS1255::ADC_Control(uint8_t gain_setting, uint8_t sensor_detect, uint8_t clockout_setting)
{
    uint8_t adc_control_register = (clockout_setting << 7) | (sensor_detect << 5) | (gain_setting << 3);

    set_settings(ADS_ADCON,adc_control_register);

    ADC_reseted = true;

    return adc_control_register;
}

//set up the AS1255 to return data on every Data Read
void ADS1255::start_continuous_mode()
{
    uint8_t i=0;
    while((ADC_reseted == true) && (i<700)) {

        wait_us(1);
        i++;
    }
    if(ADC_reseted == false) {
        spi_.write(ADS_RDATAC);
        wait(50 * (float)MasterClockPeriod);         // Wait ADS_1255 x ClockCycles (t6)

        ADC_reseted == true;

    }

}

//Stop the ADS1255 from returning on every Data Read  
void ADS1255::stop_continuous_mode()
{
    uint8_t i=0;
    while((ADC_reseted == true) && (i<700)) {

        wait_us(1);
        i++;
    }
    if(ADC_reseted == false) {
        spi_.write(ADS_SDATAC);
        wait(50 * (float)MasterClockPeriod);        // Wait ADS_1255 x ClockCycles (t6)

        ADC_reseted == true;

    }
}


//Wait for the ADC to be ready for Data Read or Timeout
//Then read data by sending 3 CLK bytes
//Compose Bytes to correct int32_t format
int32_t ADS1255::ADS_read_data()
{
    ADC_data_already_read = true;

    int32_t analog_in = 0;
    uint16_t i=0;

    while((i<700) && ((ADS_DRDY_interrupted == false) || (ADC_data_already_read == true)) ) {

        wait_us(1);
        i++;
    }

    if((ADS_DRDY_interrupted == true) && (ADC_data_already_read == false)) {             // Wait ADS_1255 DataReady-Pin

        uint8_t data0 = spi_.write(0x00);                // MSB
        uint8_t data1 = spi_.write(0x00);                // MidByte
        uint8_t data2 = spi_.write(0x00);                // LSB

        analog_in = ((data0 << 24) + (data1 << 16) + (data2 << 8)) / 256;   // shift all and divide by 0xFF for right sign

        ADC_data_already_read = true;

    }

    return(analog_in);
}