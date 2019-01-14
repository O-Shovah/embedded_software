#ifndef find_number_h
#define find_number_h

/**
 * Includes
 */
#include "mbed.h"

/**
 * Defines
 */



class find_number
{
public:

        
    //Constructor:
    
    find_number(int32_t  *selectable_values,int32_t cells_in_array);
    
    find_number(); // = default;
    
    //Class Objects:
    
    int32_t find_closest(int32_t target_value);

    int32_t find_larger(int32_t target_value);

    int32_t find_smaller(int32_t target_value);

private:

    int32_t searched_array_size;
    
    int32_t *selectable_values_array;
    
};

#endif