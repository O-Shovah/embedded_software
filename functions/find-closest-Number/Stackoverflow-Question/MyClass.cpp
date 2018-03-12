#include "MyClass.h"

MyClass::MyClass()
{
    init();
    find_number MyNumber;
}

bool MyClass::init()
{
    int32_t Integer_array[] = {2,5,10};

    Numbers_in_Integer_array = (sizeof(Integer_array)/sizeof(*Integer_array));

    find_number MyNumber (Integer_array,Numbers_in_Integer_array);

    return 0;
}

int32_t MyClass::select(int32_t the_searched_number)
{
    the_selected_number = MyNumber.find_closest(the_searched_number);

    return the_selected_number;
}