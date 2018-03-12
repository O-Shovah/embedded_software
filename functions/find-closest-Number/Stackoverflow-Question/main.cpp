#include "MyClass.h"

int main()
{
    int32_t samplerate_return = 0;
    int32_t samplerate_request = 7;

    MyClass ADC_1 ();

    while(1) {
      samplerate_return = ADC_1.select(samplerate_request);
    }
}