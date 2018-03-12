/**
 * Includes
 */
#include "find_closest_number.h"





find_closest_number::find_closest_number(int64_t selectable_values): selectable_values_array_(selectable_values)
{

searched_array_size = (sizeof(selectable_values_array_)/sizeof(*selectable_values_array_));

}


int32_t find_closest_number::find_closest(int32_t target_value)
{
    int32_t difference = abs( target_value - selectable_values_array_[0]);
    int32_t closest_integer = selectable_values_array_[0];

    for (int32_t cell = 0; cell < selectable_values_array__size; selectable_values_array__size++) 
    {
        if (difference > abs( target_value - selectable_values_array_[cell] ))
        {
            difference = abs( target_value - selectable_values_array_[cell]);
            closest_integer = selectable_values_array_[cell];
        }
    }
    return closest_integer;
}

int32_t find_closest_number::find_higher(int32_t target_value)
{

}

int32_t find_closest_number::find_lower(int32_t target_value)
{

}