/**
 * Includes
 */
#include "find_closest_number.h"


int findKey(int[], int, int);
//function main
int main()
{
    int RandNumbs[] = { 85, 250, 750, 1250, 2000, 2500, 3750, 7500};
    const int Blob = (sizeof(RandNumbs)/sizeof(*RandNumbs));
    int SearchKey;
    int NearestNumbs;
    pc.printf("Enter number to search: \n\r");
    pc.scanf("%i",&SearchKey);
    pc.printf("Entered number %i : \n\r",SearchKey);
    NearestNumbs = findKey(RandNumbs, SearchKey, Blob);
    pc.printf("The closest number to the entered number in the array is: \n\r");
    pc.printf("%i \n\r",NearestNumbs);
    return 0;
}

int findKey(int Array1[], int key, int Blub)
{
    int diff = abs( key - Array1[0]);
    int Num1 = Array1[0];
    for (int a = 0; a < Blub; a++) {
        if (diff > abs( key - Array1[a] )) {
            diff = abs( key - Array1[a]);
            Num1 = Array1[a];
        }
    }
    return Num1;
}