#ifndef ADS1255_H
#define ADS1255_H

/**
 * Includes
 */
#include "mbed.h"

/**
 * Defines
 */



class find_closest_number
{
public:

	    /**
     * Constructor.
     */
	find_closest_number(int64_t selectable_values);

	int32_t find_closest(int32_t target_value);

	int32_t find_higher(int32_t target_value);

	int32_t find_lower(int32_t target_value);

private:

	int32_t searched_array_size;

	int64_t selectable_values_array_;

};

#endif