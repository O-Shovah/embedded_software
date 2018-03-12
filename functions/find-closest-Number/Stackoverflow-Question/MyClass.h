#ifndef MyClass_H
#define MyClass_H

#include "find_number.h"

class MyClass
{
public:
	MyClass();
    int32_t select(int32_t the_searched_number);
    int32_t the_selected_number;

private:
    int32_t Integer_array[];
    int32_t Numbers_in_Integer_array;
	bool init();
    find_number MyNumber;
};