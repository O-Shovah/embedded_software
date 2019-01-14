/**
 * Includes
 */
#include "find_number.h"

Serial pc2(USBTX, USBRX); // tx, rx

find_number::find_number(int32_t *selectable_values, int32_t cells_in_array)
{
    selectable_values_array = selectable_values;

    searched_array_size = cells_in_array;


    pc2.printf("****************************\n\r");
    pc2.printf("Values inside find_numbers : \n\r"); 
    pc2.printf("****************************\n\r\n\r");
       
    pc2.printf("cells_in_array: %i \n\r",cells_in_array);
    pc2.printf("selectable_values_array:");
    for(int i = 0; i <searched_array_size;i++)
    {
        pc2.printf(" %i, ",selectable_values_array[i]);
        }
    pc2.printf("\n\r\n\r");
}

find_number::find_number() {};



int32_t find_number::find_closest(int32_t target_value)
{
    int32_t difference = abs( target_value - selectable_values_array[0]);
    int32_t closest_integer = selectable_values_array[0];
    
    pc2.printf("******************************\n\r");
    pc2.printf("Values inside 'find_closest' : \n\r");    
    pc2.printf("******************************\n\r\n\r");
    pc2.printf("searched_array_size: %i \n\r",searched_array_size);
    pc2.printf("difference: %i \n\r",difference);
    pc2.printf("target_value: %i \n\r",target_value);
    pc2.printf("selectable_values_array: ");
    for(int i = 0; i <searched_array_size;i++)
    {
        pc2.printf("%i, ",selectable_values_array[i]);
        }
    pc2.printf("\n\r\n\r");

    for (int32_t cell = 0; cell < searched_array_size; cell++) {
        if (difference > abs( target_value - selectable_values_array[cell] )) {
            difference = abs( target_value - selectable_values_array[cell]);
            closest_integer = selectable_values_array[cell];
        }
    }
    return closest_integer;
}

int32_t find_number::find_larger(int32_t target_value)
{
    int32_t larger_integer = 0;

    for (int32_t cell=0; cell<searched_array_size; cell++) {

        for (int32_t next_cell = cell+1; next_cell < searched_array_size; next_cell++) {

            if (selectable_values_array[cell] <= target_value && target_value <= selectable_values_array[next_cell]) {

                larger_integer = selectable_values_array[next_cell];
                
                if(selectable_values_array[next_cell] > larger_integer) larger_integer = selectable_values_array[next_cell];
                
                if(selectable_values_array[cell] > larger_integer) larger_integer = selectable_values_array[cell];
                
                if(selectable_values_array[cell] == target_value) larger_integer = selectable_values_array[cell];

                if(selectable_values_array[next_cell] == target_value) larger_integer = selectable_values_array[next_cell];

                break;
            }
            
            if(selectable_values_array[0] > target_value) larger_integer = selectable_values_array[0];
        }

    }
    return larger_integer;
}


int32_t find_number::find_smaller(int32_t target_value)
{
    int32_t smaller_integer = 0;

    for (int32_t cell=0; cell<searched_array_size; cell++) {

        for (int32_t next_cell = cell+1; next_cell < searched_array_size; next_cell++) {

            if (selectable_values_array[cell] <= target_value && target_value <= selectable_values_array[next_cell]) {

                smaller_integer = selectable_values_array[cell];
                
                if(selectable_values_array[cell] < smaller_integer ) smaller_integer = selectable_values_array[cell];
                
                if(selectable_values_array[next_cell] < smaller_integer) smaller_integer = selectable_values_array[next_cell];
                
                if(selectable_values_array[cell] == target_value) smaller_integer = selectable_values_array[cell];

                if(selectable_values_array[next_cell] == target_value) smaller_integer = selectable_values_array[next_cell];
                
                break;
            }
        }

    }
    return smaller_integer;
}