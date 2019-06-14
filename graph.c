#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_RULES      67
#define MAX_CONF_SIZE  26
#define NUM_CONF      633

#define swap(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

bool ERROR = false;
int memory_in_use = 0;
int num_label = 1;
int rule_size[] = { 2,  2,  3,  3,  4,  4,  5,  5,  5,  5,  6,  6,  
    6,  6,  7,  7,  4,  4,  5,  5,  6,  6,  6,  7,  7,  8,  8,  8,  
	6,  6,  8,  8,  8,  8,  9,  9,  8,  8, 10, 10,  8,  8,  8,  8,  
	8,  8,  6,  6,  9,  9,  9,  9,  9,  9, 10, 10,  7,  7,  8,  8,  
	8,  8, 10, 10, 10, 10, 10};
int rule_vertices[][10] = {
/* 01 */ { 0,  1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 02 */ { 0,  1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 03 */ { 0,  1,  3, -1, -1, -1, -1, -1, -1, -1},
/* 04 */ { 0,  1,  2, -1, -1, -1, -1, -1, -1, -1},
/* 05 */ { 0,  1,  3,  5, -1, -1, -1, -1, -1, -1},
/* 06 */ { 0,  1,  2,  4, -1, -1, -1, -1, -1, -1},
/* 07 */ { 0,  1,  3,  5,  9, -1, -1, -1, -1, -1},
/* 08 */ { 0,  1,  2,  4,  8, -1, -1, -1, -1, -1},
/* 09 */ { 0,  1,  3,  5,  9, -1, -1, -1, -1, -1},
/* 10 */ { 0,  1,  2,  4,  8, -1, -1, -1, -1, -1},
/* 11 */ { 0,  1,  3,  5,  9, 11, -1, -1, -1, -1},
/* 12 */ { 0,  1,  2,  4,  8, 10, -1, -1, -1, -1},
/* 13 */ { 0,  1,  3,  5,  7,  9, -1, -1, -1, -1},
/* 14 */ { 0,  1,  2,  4,  6,  8, -1, -1, -1, -1},
/* 15 */ { 0,  1,  3,  5,  7,  9, 11, -1, -1, -1},
/* 16 */ { 0,  1,  2,  4,  6,  8, 10, -1, -1, -1},
/* 17 */ { 0,  1,  3,  5, -1, -1, -1, -1, -1, -1},
/* 18 */ { 0,  1,  2,  4, -1, -1, -1, -1, -1, -1},
/* 19 */ { 0,  1,  3,  5, 14, -1, -1, -1, -1, -1},
/* 20 */ { 0,  1,  2,  4, 12, -1, -1, -1, -1, -1},
/* 21 */ { 0,  1,  2,  3,  4,  5, -1, -1, -1, -1},
/* 22 */ { 0,  1,  2,  3,  4,  5, -1, -1, -1, -1},
/* 23 */ { 0,  1,  3,  2,  5,  4, -1, -1, -1, -1},
/* 24 */ { 0,  1,  2,  3,  4,  5,  8, -1, -1, -1},
/* 25 */ { 0,  1,  3,  2,  5,  4,  9, -1, -1, -1},
/* 26 */ { 0,  1,  2,  3,  4,  5,  8,  9, -1, -1},
/* 27 */ { 0,  1,  2,  3,  4,  5,  6,  8, -1, -1},
/* 28 */ { 0,  1,  3,  2,  5,  4,  7,  9, -1, -1},
/* 29 */ { 0,  1,  2,  3,  4,  8, -1, -1, -1, -1},
/* 30 */ { 0,  1,  3,  2,  5,  9, -1, -1, -1, -1},
/* 31 */ { 0,  1,  2,  3,  4,  5, 12, 13, -1, -1},
/* 32 */ { 0,  1,  3,  2,  5,  4, 14, 17, -1, -1},
/* 33 */ { 0,  1,  2,  3,  4,  5, 12, 13, -1, -1},
/* 34 */ { 0,  1,  3,  2,  5,  4, 14, 17, -1, -1},
/* 35 */ { 0,  1,  2,  3,  4,  5,  8, 12, 13, -1},
/* 36 */ { 0,  1,  3,  2,  5,  4,  9, 14, 17, -1},
/* 37 */ { 0,  1,  2,  3,  5,  7,  9, 14, -1, -1},
/* 38 */ { 0,  1,  3,  2,  4,  6,  8, 12, -1, -1},
/* 39 */ { 0,  1,  2,  3,  4,  5,  7,  9, 12, 13},
/* 40 */ { 0,  1,  3,  2,  5,  4,  6,  8, 14, 17},
/* 41 */ { 0,  1,  2,  3,  4,  5, 12, 13, -1, -1},
/* 42 */ { 0,  1,  3,  2,  5,  4, 14, 17, -1, -1},
/* 43 */ { 0,  1,  2,  3,  4,  6, 12, 13, -1, -1},
/* 44 */ { 0,  1,  3,  2,  5,  7, 14, 17, -1, -1},
/* 45 */ { 0,  1,  2,  3,  4,  6, 12, 13, -1, -1},
/* 46 */ { 0,  1,  3,  2,  5,  7, 14, 17, -1, -1},
/* 47 */ { 0,  1,  2,  3,  5,  9, -1, -1, -1, -1},
/* 48 */ { 0,  1,  3,  2,  4,  8, -1, -1, -1, -1},
/* 49 */ { 0,  1,  2,  3,  4,  5,  6, 12, 13, -1},
/* 50 */ { 0,  1,  3,  2,  5,  4,  7, 14, 17, -1},
/* 51 */ { 0,  1,  2,  3,  4,  5,  9, 11, 14, -1},
/* 52 */ { 0,  1,  3,  2,  5,  4,  8, 10, 12, -1},
/* 53 */ { 0,  1,  2,  3,  4,  5,  6, 12, 13, -1},
/* 54 */ { 0,  1,  3,  2,  5,  4,  7, 14, 17, -1},
/* 55 */ { 0,  1,  2,  3,  4,  5,  6,  9, 15, 16},
/* 56 */ { 0,  1,  3,  2,  5,  4,  7,  8, 18, 19},
/* 57 */ { 0,  1,  2,  3,  4,  5, 12, -1, -1, -1},
/* 58 */ { 0,  1,  3,  2,  5,  4, 14, -1, -1, -1},
/* 59 */ { 0,  1,  2,  3,  4,  8, 12, 13, -1, -1},
/* 60 */ { 0,  1,  3,  2,  5,  9, 14, 17, -1, -1},
/* 61 */ { 0,  1,  2,  3,  4,  5,  9, 14, -1, -1},
/* 62 */ { 0,  1,  3,  2,  5,  4,  8, 12, -1, -1},
/* 63 */ { 0,  1,  2,  3,  4,  5,  8,  9, 12, 14},
/* 64 */ { 0,  1,  2,  3,  4,  5,  8,  9, 12, 14},
/* 65 */ { 0,  1,  3,  2,  5,  4,  9,  8, 14, 12},
/* 66 */ { 0,  1,  2,  3,  4,  5,  6, 12, 13, 14},
/* 67 */ { 0,  1,  3,  2,  5,  4,  7, 14, 17, 12}
};
int min_deg[][20] = {
//         0   1   2   3   4  -1   6   7   8   9  10  11  12  13  14  15  16  17  18  19
/* 01 */ { 5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 02 */ { 5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 03 */ { 5,  7, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 04 */ { 5,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 05 */ { 5,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 06 */ { 5,  6,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 07 */ { 6,  6, -1,  5, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 08 */ { 6,  6,  5, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 09 */ { 6,  6, -1,  6, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 10 */ { 6,  6,  6, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 11 */ { 6,  6, -1,  6, -1,  6, -1, -1, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1},
/* 12 */ { 6,  6,  6, -1,  6, -1, -1, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 13 */ { 6,  7, -1,  5, -1,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 14 */ { 6,  7,  5, -1,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 15 */ { 6,  7, -1,  6, -1,  6, -1,  5, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1},
/* 16 */ { 6,  7,  6, -1,  6, -1,  5, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 17 */ { 7,  7, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 18 */ { 7,  7,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 19 */ { 7,  7, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 20 */ { 7,  7,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 21 */ { 7,  7,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 22 */ { 7,  7,  5,  5,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//         0   1   2   3   4  -1   6   7   8   9  10  11  12  13  14  15  16  17  18  19
/* 23 */ { 7,  7,  5,  5,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 24 */ { 7,  7,  5,  5,  6,  6, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 25 */ { 7,  7,  5,  5,  6,  6, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 26 */ { 7,  7,  5,  5,  5,  5, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 27 */ { 7,  8,  5,  5,  6,  5,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 28 */ { 7,  8,  5,  5,  5,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 29 */ { 7,  7,  6,  5,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 30 */ { 7,  7,  5,  6, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 31 */ { 7,  7,  6,  5,  5,  7, -1, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 32 */ { 7,  7,  5,  6,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 33 */ { 7,  7,  6,  5,  6,  5, -1, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 34 */ { 7,  7,  5,  6,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 35 */ { 7,  7,  6,  5,  5,  5, -1, -1,  6, -1, -1, -1,  5,  7, -1, -1, -1, -1, -1, -1},
/* 36 */ { 7,  7,  5,  6,  5,  5, -1, -1, -1,  6, -1, -1, -1, -1,  5, -1, -1,  7, -1, -1},
/* 37 */ { 7,  7,  6,  5, -1,  6, -1,  5, -1,  5, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 38 */ { 7,  7,  5,  6,  6, -1,  5, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 39 */ { 7,  8,  6,  5,  6,  6, -1,  5, -1,  5, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 40 */ { 7,  8,  5,  6,  6,  6,  5, -1,  5, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 41 */ { 7,  7,  6,  6,  5,  6, -1, -1, -1, -1, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1},
/* 42 */ { 7,  7,  6,  6,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1,  5, -1, -1},
/* 43 */ { 7,  7,  6,  6,  5, -1,  5, -1, -1, -1, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1},
/* 44 */ { 7,  7,  6,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1,  5, -1, -1,  5, -1, -1},
/* 45 */ { 7,  7,  6,  6,  5, -1,  5, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
//         0   1   2   3   4  -1   6   7   8   9  10  11  12  13  14  15  16  17  18  19
/* 46 */ { 7,  7,  6,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 47 */ { 7,  7,  5,  7, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 48 */ { 7,  7,  7,  5,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 49 */ { 7,  7,  5,  7,  5,  6,  5, -1, -1, -1, -1, -1,  5,  6, -1, -1, -1, -1, -1, -1},
/* 50 */ { 7,  7,  7,  5,  6,  5, -1,  5, -1, -1, -1, -1, -1, -1,  5, -1, -1,  6, -1, -1},
/* 51 */ { 7,  7,  5,  7,  5,  5, -1, -1, -1,  6, -1,  5, -1, -1,  5, -1, -1, -1, -1, -1},
/* 52 */ { 7,  7,  7,  5,  5,  5, -1, -1,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 53 */ { 7,  7,  6,  7,  6,  5,  5, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 54 */ { 7,  7,  7,  6,  5,  6, -1,  5, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 55 */ { 7,  7,  6,  7,  7,  5,  6, -1, -1,  5, -1, -1, -1, -1, -1,  5,  6, -1, -1, -1},
/* 56 */ { 7,  7,  7,  6,  5,  7, -1,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5,  6},
/* 57 */ { 8,  7,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 58 */ { 8,  7,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 59 */ { 8,  7,  5,  5,  5, -1, -1, -1,  5, -1, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1},
/* 60 */ { 8,  7,  5,  5, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1,  5, -1, -1,  5, -1, -1},
/* 61 */ { 8,  7,  5,  5,  5,  6, -1, -1, -1,  5, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 62 */ { 8,  7,  5,  5,  6,  5, -1, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 63 */ { 8,  7,  5,  5,  5,  5, -1, -1,  5,  5, -1, -1,  6, -1,  6, -1, -1, -1, -1, -1},
/* 64 */ { 8,  7,  5,  5,  5,  5, -1, -1,  5,  6, -1, -1,  6, -1,  6, -1, -1, -1, -1, -1},
/* 65 */ { 8,  7,  5,  5,  5,  5, -1, -1,  6,  5, -1, -1,  6, -1,  6, -1, -1, -1, -1, -1},
/* 66 */ { 8,  7,  6,  7,  5,  6,  5, -1, -1, -1, -1, -1,  5,  5,  5, -1, -1, -1, -1, -1},
/* 67 */ { 8,  7,  7,  6,  6,  5, -1,  5, -1, -1, -1, -1,  5, -1,  5, -1, -1,  5, -1, -1}
};
int max_deg[][20] = {
//         0   1   2   3   4  -1   6   7   8   9  10  11  12  13  14  15  16  17  18  19
/* 01 */ { 5, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 02 */ { 5, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 03 */ { 6, 12, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 04 */ { 6, 12,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 05 */ { 6, 12, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 06 */ { 6, 12,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 07 */ { 6, 12, -1,  5, -1,  6, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 08 */ { 6, 12,  5, -1,  6, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 09 */ { 6, 12, -1,  6, -1,  6, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 10 */ { 6, 12,  6, -1,  6, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 11 */ { 6, 12, -1,  6, -1,  6, -1, -1, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1},
/* 12 */ { 6, 12,  6, -1,  6, -1, -1, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 13 */ { 6, 12, -1,  5, -1,  6, -1,  5, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 14 */ { 6, 12,  5, -1,  6, -1,  5, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 15 */ { 6, 12, -1,  6, -1,  6, -1,  5, -1,  6, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1},
/* 16 */ { 6, 12,  6, -1,  6, -1,  5, -1,  6, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 17 */ { 7, 12, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 18 */ { 7, 12,  6, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 19 */ { 7, 12, -1,  6, -1,  6, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 20 */ { 7, 12,  6, -1,  6, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 21 */ { 7, 12,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 22 */ { 7, 12,  5,  5,  5, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
//         0   1   2   3   4  -1   6   7   8   9  10  11  12  13  14  15  16  17  18  19
/* 23 */ { 7, 12,  5,  5, 12,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 24 */ { 7, 12,  5,  5,  6, 12, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 25 */ { 7, 12,  5,  5, 12,  6, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 26 */ { 7, 12,  5,  5,  5,  5, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 27 */ { 7, 12,  5,  5,  6,  5,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 28 */ { 7, 12,  5,  5,  5,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 29 */ { 7, 12,  6,  5,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 30 */ { 7, 12,  5,  6, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 31 */ { 7, 12,  6,  5,  5, 12, -1, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 32 */ { 7, 12,  5,  6, 12,  5, -1, -1, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 33 */ { 7, 12,  6,  5,  6,  5, -1, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 34 */ { 7, 12,  5,  6,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 35 */ { 7, 12,  6,  5,  5,  5, -1, -1,  6, -1, -1, -1,  5, 12, -1, -1, -1, -1, -1, -1},
/* 36 */ { 7, 12,  5,  6,  5,  5, -1, -1, -1,  6, -1, -1, -1, -1,  5, -1, -1, 12, -1, -1},
/* 37 */ { 7,  7, 12,  5, -1,  6, -1,  5, -1,  5, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 38 */ { 7,  7,  5, 12,  6, -1,  5, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 39 */ { 7, 12, 12,  5,  6,  6, -1,  5, -1,  5, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 40 */ { 7, 12,  5, 12,  6,  6,  5, -1,  5, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 41 */ { 7, 12,  6,  6,  5,  6, -1, -1, -1, -1, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1},
/* 42 */ { 7, 12,  6,  6,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1,  5, -1, -1},
/* 43 */ { 7,  7,  6, 12,  6, -1,  5, -1, -1, -1, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1},
/* 44 */ { 7,  7, 12,  6, -1,  6, -1,  5, -1, -1, -1, -1, -1, -1,  5, -1, -1,  5, -1, -1},
/* 45 */ { 7,  7,  6, 12,  5, -1,  5, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
//         0   1   2   3   4  -1   6   7   8   9  10  11  12  13  14  15  16  17  18  19
/* 46 */ { 7,  7, 12,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 47 */ { 7, 12,  5,  7, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 48 */ { 7, 12,  7,  5,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 49 */ { 7,  7,  5, 12,  5,  6,  5, -1, -1, -1, -1, -1,  5,  6, -1, -1, -1, -1, -1, -1},
/* 50 */ { 7,  7, 12,  5,  6,  5, -1,  5, -1, -1, -1, -1, -1, -1,  5, -1, -1,  6, -1, -1},
/* 51 */ { 7, 12,  5,  7,  5,  5, -1, -1, -1,  6, -1,  5, -1, -1,  5, -1, -1, -1, -1, -1},
/* 52 */ { 7, 12,  7,  5,  5,  5, -1, -1,  6, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 53 */ { 7,  7,  6, 12,  6,  5,  5, -1, -1, -1, -1, -1,  6,  5, -1, -1, -1, -1, -1, -1},
/* 54 */ { 7,  7, 12,  6,  5,  6, -1,  5, -1, -1, -1, -1, -1, -1,  6, -1, -1,  5, -1, -1},
/* 55 */ { 7,  7,  6,  7, 12,  6,  6, -1, -1,  5, -1, -1, -1, -1, -1,  5, 12, -1, -1, -1},
/* 56 */ { 7,  7,  7,  6,  6, 12, -1,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5, 12},
/* 57 */ { 8, 12,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 58 */ { 8, 12,  5,  5,  5,  5, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 59 */ { 8, 12,  5,  5,  5, -1, -1, -1,  5, -1, -1, -1,  5,  5, -1, -1, -1, -1, -1, -1},
/* 60 */ { 8, 12,  5,  5, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1,  5, -1, -1,  5, -1, -1},
/* 61 */ { 8, 12,  5,  5,  5,  6, -1, -1, -1,  5, -1, -1, -1, -1,  5, -1, -1, -1, -1, -1},
/* 62 */ { 8, 12,  5,  5,  6,  5, -1, -1,  5, -1, -1, -1,  5, -1, -1, -1, -1, -1, -1, -1},
/* 63 */ { 8, 12,  5,  5,  5,  5, -1, -1,  5,  5, -1, -1, 12, -1, 12, -1, -1, -1, -1, -1},
/* 64 */ { 8, 12,  5,  5,  5,  5, -1, -1,  5, 12, -1, -1, 12, -1, 12, -1, -1, -1, -1, -1},
/* 65 */ { 8, 12,  5,  5,  5,  5, -1, -1, 12,  5, -1, -1, 12, -1, 12, -1, -1, -1, -1, -1},
/* 66 */ { 8,  7,  6, 12,  5,  6,  5, -1, -1, -1, -1, -1,  5,  5,  5, -1, -1, -1, -1, -1},
/* 67 */ { 8,  7, 12,  6,  6,  5, -1,  5, -1, -1, -1, -1,  5, -1,  5, -1, -1,  5, -1, -1}
};

struct edge {
	struct vertex* v;		// vertex the edge is connected to
	struct edge* next;		// next edge clockwise around v
	struct edge* prev;		// previous edge clockwise around v
	struct edge* rev;		// reverse edge
	int color;
};

struct vertex {
	int num;
	int deg;
	int weight;
	bool visited;
	struct edge* e;			/* pointer to the first edge */
	struct vertex *next;
	struct vertex *prev;
} *map[MAX_CONF_SIZE + 1];

struct graph {
	char name[100];
	int n;					/* number of vertices */
	struct vertex* vert;	/* pointer to the first vertex */
};

struct configuration {
	char N[100];	// a character string identifying the configuration;
	int n;			// n is the number of vertices of the free completion;
	int r;			// r is the ring-size;
	int a;			// a is the cardinality of C ; and
	int b;			// b is the cardinality of C' (see discussion before (3.2) in the paper);
    int x;
    struct edge* X[4];
	struct graph g;
} conf[633];

void reinsert_to_edge_list(struct edge*);
void contract_edge(struct graph*, struct edge*);
void decontract_edge(struct graph*, struct edge*);
void color_dfs(struct vertex*, int*);
void color_vertices(struct graph* g);
struct edge* find_edge(struct vertex*, struct vertex*);
void add_edge(struct vertex*, struct vertex*);
struct graph* read_graph(char*);
struct graph* read_graph_colored(char*);
void free_graph(struct graph*);
void print_dfs(struct vertex*);
void print_graph(struct graph*);
void apply_rules(struct edge*);
struct vertex* find_hub(struct graph*);
void base_case_coloring(struct graph*);
void parallel_edges_coloring(struct graph*, struct edge*, struct edge*);
void remove_from_edge_list(struct edge*);
void remove_from_vertex_list(struct graph*, struct vertex*);
void add_to_vertex_list(struct graph*, struct vertex*);
void degree_three_elimination(struct graph*, struct vertex*);
void find_coloring(struct graph*);
void check_graph(struct graph*);
void set_visited_false(struct graph*);
void scs_3(struct graph*, struct edge*, struct edge*, struct edge*);

// debug function
void* my_malloc(size_t size){
    void* p = malloc(size);
    memory_in_use++;
    return p;
}

// debug function
void my_free(void* size){
    free(size);
    memory_in_use--;
}

// debug defines
#define malloc(X) my_malloc(X)
#define free(X) my_free(X)
#define debug printf

// adds an edge removed from an edge_list back to the edge list
void reinsert_to_edge_list(struct edge* e){
	if(e == NULL){
		printf("Trying to add a NULL edge in reinsert_to_edge_list\n");
		ERROR = true;
		return;
	}
	e->next->prev = e;
	e->prev->next = e;
	e->v->deg++;
}

// contracts an edge from a graph
void contract_edge(struct graph* g, struct edge * e){
	struct vertex* v1 = e->v;
	struct vertex* v2 = e->rev->v;
	struct vertex* vnew = (struct vertex*) malloc(sizeof(struct vertex));
	int i;
	
	// 1. remove edges: e, e->rev, e->next, e->next->rev, e->rev->next, e->rev->next->rev
	remove_from_edge_list(e);
	remove_from_edge_list(e->rev);
	remove_from_edge_list(e->next);
	remove_from_edge_list(e->next->rev);
	remove_from_edge_list(e->rev->next);
	remove_from_edge_list(e->rev->next->rev);
	// 2. fix the next and prev of the edges next to deleted edges
	e->next->next->prev = e->rev->prev;
	e->rev->prev->next = e->next->next;
	e->rev->next->next->prev = e->prev;
	e->prev->next = e->rev->next->next;
	// *. for safety measures
	v1->e = e;
	v2->e = e->rev;
	// 3. connect all edges to vnew
	e = e->next->next;
	for (i = 0; i < v1->deg+v2->deg; i++) {
		e->v=vnew;
		e = e->next;
	}
	vnew->e=e;
	// 4. Some bookkeeping
	vnew->num = num_label;
	num_label++;
	vnew->deg = v1->deg + v2->deg;
	add_to_vertex_list(g,vnew);
	remove_from_vertex_list(g,v1);
	remove_from_vertex_list(g,v2);
}

// given a contracted edge and the graph it belongs, decontracts the edge
void decontract_edge(struct graph* g, struct edge * e){
	struct vertex* v1 = e->v;
	struct vertex* v2 = e->rev->v;
	struct vertex* v_old = e->prev->v;
	int i;
	
	// 1. reinsert edges: e->rev->next->rev, e->rev->next, e->next->rev, e->next, e->rev, e
	reinsert_to_edge_list(e->rev->next->rev);
	reinsert_to_edge_list(e->rev->next);
	reinsert_to_edge_list(e->next->rev);
	reinsert_to_edge_list(e->next);
	reinsert_to_edge_list(e->rev);
	reinsert_to_edge_list(e);
	// 2. reconnect edges to v1 and v2
	for (i = 0; i < v1->deg; i++) {
		e->v=v1;
		e = e->next;
	}
	e = e->rev;

	for (i = 0; i < v2->deg; i++) {
		e->v=v2;
		e = e->next;
	}
	// 3. bookkeeping
	remove_from_vertex_list(g,v_old);
	add_to_vertex_list(g,v1);
	add_to_vertex_list(g,v2);
	free(v_old);
}

// DFS to color the vertices
// Assumes vertices numbered 1 to n
void color_dfs(struct vertex* v, int* colored){
	struct edge * e = v->e;
	int i;
	for(i = 0; i < v->deg; i++){
		if( colored[e->rev->v->num] == -1 ){
			colored[e->rev->v->num] = (colored[v->num])^(e->color);
			color_dfs(e->rev->v, colored);
		}
		e = e->next;
	}
}

// colors edges of graph
void color_vertices(struct graph* g){
	int i;
	int* colored = (int*) malloc(sizeof(int) * (g->n + 1));
	for(i = 0; i <= g->n; i++) colored[i] = -1;
	colored[g->vert->num] = 0;
	color_dfs(g->vert, colored);
	printf("Coloring:\n");
	for(i = 1; i <= g->n; i++) printf("%d: %d\n", i, colored[i]);
}

// finds edge between v and u in the list of edges of v
struct edge* find_edge(struct vertex* v, struct vertex* u){
	struct edge* e = v->e;
	int i;
	if(e == NULL){
		printf("Invalid function input in find_edge: e == NULL \n");
		 ERROR = true;
	}
	for(i = 0; i < v->deg; i++){
		if( e->rev->v == u ) return e;
		e = e->next;
	}
	printf("find_edge couldn't find requested edge: %d -> %d\n",v->num, u->num);
	ERROR = true;
	return NULL;
}

// adds edge between v and u to the end of the list of edges of v, creates it if it doesn't exist
// DOES NOT change the degree of v and u
void add_edge(struct vertex* v, struct vertex* u){
	struct edge* e_v;
	struct edge* e_u;
	if(v == u){
		printf("Incorrect function input: add_edge recieved a self loop at vertex %d\n",v->num);
		ERROR = true;
		return;
	}
	// creates the right edge if this is the first time processing the edge
	if( u->e == NULL ){
		e_v = (struct edge*) malloc(sizeof(struct edge));
		e_u = (struct edge*) malloc(sizeof(struct edge));
		if(e_v == NULL || e_u == NULL){
			printf("Memory error: not enough space for edge in function add_edge\n");
			ERROR = true;
			return;
		}
		e_v->v = v;
		e_u->v = u;
		e_v->color = e_u->color = 0;
		e_v->rev = e_u;
		e_u->rev = e_v;
	}
	// finds the right edge if this is the second time processing the edge
	else{
		e_u = find_edge(u, v);
		if(ERROR) return;
		if(e_u == NULL){
			printf("Invalid output: find_edge\n");
			ERROR = true;
			return;
		}
		e_v = e_u->rev;
	}

	if(v->e == NULL){
		v->e =  e_v;
		e_v->next = e_v->prev = e_v;
	}
	else{
		v->e->prev->next = e_v;
		e_v->prev = v->e->prev;
		v->e->prev = e_v;
		e_v->next = v->e;
	}
}

// quadratic, but can be improved to nlogn if necessary
struct graph* read_graph(char* data){
	// first line is the number of vertices
	// second line is list of degrees of each vertex
	// subsequent lines (starting from i=0)
  	// are the vertices that vertex i is adjacent to
	int i, j, u, n;
	struct graph* g = (struct graph*) malloc(sizeof(struct graph));
	struct vertex* v1;
	struct vertex* v2;
	struct edge* e;
	FILE* ptr = fopen(data,"r");
	
	if(g == NULL){
		printf("Memory error: not enough space for graph in function read_graph\n");
		ERROR = true;
		return NULL;
	}
	g->vert = NULL;
	g->n = 0;

	fscanf(ptr,"%d",&n);
	for(i = 1; i <= n; i++){
		v1 = (struct vertex*) malloc(sizeof(struct vertex));
		if(v1 == NULL){
			printf("Memory error: not enough space for new vertex in function read_graph\n");
			ERROR = true;
			return NULL;
		}
		v1->num = num_label;
		num_label++;
		add_to_vertex_list(g, v1);
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		v1->e = NULL;
		fscanf(ptr, "%d", &v1->deg);
		if(v1->deg < 0){
			printf("Invalid degree, must be >= 0\n");
			ERROR = true;
			return NULL;
		}
		v1 = v1->next;
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		for(j = 0; j < v1->deg; j++){
			fscanf(ptr,"%d",&u);
			if(u <= 0 && u > g->n){
				printf("Invalid vertex, must be between 1 and n\n");
				ERROR = true;
				return NULL;
			}
			v2 = v1->next;
			while(v2->num != u){
				v2 = v2->next;
			}
			add_edge(v1, v2);
		}
		v1 = v1->next;
	}
	fclose(ptr);
	return g;
}

// debug function
// read a graph with a fixed tri coloring
struct graph* read_graph_colored(char *data){
	// modified copy of read_graph
	int i, j, u;
	struct graph* g = (struct graph*) malloc(sizeof(struct graph));
	struct vertex* v1;
	struct vertex* v2;
	struct edge* e;

	FILE* ptr = fopen(data,"r");
	fscanf(ptr,"%d",&g->n);

	v1 = (struct vertex*) malloc(sizeof(struct vertex));
	if(v1 == NULL){
		printf("Memory error: not enough space for new vertex in function read_graph\n");
		ERROR = true;
		return NULL;
	}
	g->vert = v1;
	v1->num = num_label;
	num_label++;
	for(i = 2; i <= g->n; i++){
		v2 = (struct vertex*) malloc(sizeof(struct vertex));
		if(v2 == NULL){
			printf("Memory error: not enough space for new vertex in function read_graph\n");
			ERROR = true;
			return NULL;
		}
		v2->num = num_label;
		num_label++;
		v1->next = v2;
		v2->prev = v1;
		v1 = v2;
	}
	v2->next = g->vert;
	g->vert->prev = v2;

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		v1->e = NULL;
		fscanf(ptr, "%d", &v1->deg);
		if(v1->deg < 0){
			printf("Invalid degree, must be >= 0\n");
			ERROR = true;
			return NULL;
		}
		v1 = v1->next;
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		for(j = 0; j < v1->deg; j++){
			fscanf(ptr,"%d",&u);
			if(u <= 0 && u > g->n){
				printf("Invalid vertex, must be between 1 and n\n");
				ERROR = true;
				return NULL;
			}
			v2 = v1->next;
			while(v2->num != u){
				v2 = v2->next;
			}
			add_edge(v1, v2);
			fscanf(ptr,"%d", &v1->e->prev->color);
		}
		v1 = v1->next;
	}
	return g;
}

// frees the whole graph
void free_graph(struct graph* g){
	struct vertex* v = g->vert;
	struct edge* e;
	int i,j;
	// frees the edges
	for(i = 0; i < g->n; i++){
		e =  v->e;
		for(j = 1; j < v->deg; j++){
			e = e->next;
			free(e->prev);
		}
		free(e);
		v = v->next;
	}
	// frees the vertices
	for(i = 1; i < g->n; i++){
		v = v->next;
		free(v->prev);
	}
	free(v);
	// frees the graph
	free(g);
}

// debug function
// assumes graph is correct
void print_dfs(struct vertex* v){
	int i;
	struct edge* e = v->e;
	v->visited = true;
	printf("vertex %d, deg %d : ", v->num, v->deg);
	for(i = 1; i <= v->deg; i++){
		printf("%d ",e->rev->v->num);
		e = e->next;
	}
	printf("\n");
	for(i = 1; i <= v->deg; i++){
		if( e->rev->v->visited == false ){
			print_dfs(e->rev->v);
		}
		e = e->next;
	}
}

// debug function
// assumes graph from 1 to n
void print_graph(struct graph* g){
	int i,j;
	struct vertex* v = g->vert;
	struct edge* e;
	
	printf("Edges colors\n");
	v = g->vert;
	for(i = 0; i < g->n; i++){
		e = v->e;
		printf("Vertex %d, deg %d\n",v->num, v->deg);
		for(j = 0; j < v->deg; j++){
			printf("Edge %d %d, color %d\n",e->v->num,e->rev->v->num,e->color);
			e = e->next;
		}
		v = v->next;
	}
	printf("\n");
	set_visited_false(g);
	print_dfs(g->vert);
}

// applies the rules to odify the weight function
void apply_rules(struct edge* e){
	struct vertex* v[20];
	int cur;
	int i, j;
	bool works;
	
	// to account for symmetry / invert we add three new vertices to the descriptions of the rules
	// 17 is the invert of 13
	// 18 is the invert of 15
	// 19 is the invert of 16
	
	v[0]  = e->v;
	v[1]  = e->rev->v;
	v[2]  = e->next->rev->v;
	v[3]  = e->prev->rev->v;
	v[4]  = e->next->next->rev->v;
	v[5]  = e->prev->prev->rev->v;
	v[6]  = e->rev->prev->prev->rev->v;
	v[7]  = e->rev->next->next->rev->v;
	v[8]  = e->next->rev->prev->prev->rev->v;
	v[9]  = e->prev->rev->next->next->rev->v;
	v[10] = e->next->rev->prev->prev->prev->rev->v;
	v[11] = e->prev->rev->next->next->next->rev->v;
	v[12] = e->next->next->next->rev->v;
	v[13] = e->next->next->next->next->rev->v;
	v[14] = e->prev->prev->prev->rev->v;
	v[15] = e->rev->prev->prev->prev->rev->v;
	v[16] = e->rev->prev->prev->prev->prev->rev->v;
	v[17] = e->prev->prev->prev->prev->rev->v;
	v[18] = e->rev->next->next->next->rev->v;
	v[19] = e->rev->next->next->next->next->rev->v;
	for(i = 0; i < NUM_RULES; i++){
		works = true;
		for(j = 0; j < rule_size[i]; j++){
			cur = rule_vertices[i][j];
			if( v[cur]->deg < min_deg[i][cur] || max_deg[i][cur] < v[cur]->deg ) works = false; 
		}
		if(works){
			v[0]->weight--;
			v[1]->weight++;
		}
	}
}

// finds the hub of a cartwheel with positive weight
struct vertex* find_hub(struct graph* g){
	struct vertex* v = g->vert;
	struct edge* e;
	int i, j;
	for(i = 0; i < g->n; i++){
		v->weight = 10 * ( 6 - v->deg );
		v = v->next;
	}
	for(i = 0; i < g->n; i++){
		e = v->e;
		for(j = 0; j < v->deg; j++){
			if( e->rev->v->visited == false ){
				apply_rules(e);	
			}
			e = e->next;	
		}
		if(v->weight > 0) return v;
		v->visited = true;
		v = v->next;	
	}
	printf("find_hub couldn't find a vertex with weight positive\n");
	ERROR = true;
	return NULL;
}

// colors sufficiently small triangulations (n <= 4)
void base_case_coloring(struct graph* g){
	if(g->n < 1){
		printf("Empty graph at base_case_coloring\n");
		ERROR = true;
	}
	else if(g->n == 2){
		g->vert->e->color = 1;
		g->vert->e->rev->color = 1;
		printf("Empty graph at base_case_coloring\n");
		ERROR = true;
	}
	else if(g->n == 3){
		// must be a triangle
		g->vert->e->color = 1;
		g->vert->e->rev->color = 1;
		g->vert->e->next->color = 2;
		g->vert->e->next->rev->color = 2;
		g->vert->e->rev->next->color = 3;
		g->vert->e->rev->next->rev->color = 3;
	}
	else if(g->n == 4){
		// must be a triangular pyramid if we don't have parallel edges
		// edges from g->vert
		g->vert->e->color = 1;
		g->vert->e->rev->color = 1;
		g->vert->e->next->color = 2;
		g->vert->e->next->rev->color = 2;
		g->vert->e->next->next->color = 3;
		g->vert->e->next->next->rev->color = 3;
		// other edges
		g->vert->e->rev->next->color = 2;
		g->vert->e->rev->next->rev->color = 2;
		g->vert->e->rev->prev->color = 3;
		g->vert->e->rev->prev->rev->color = 3;
		g->vert->e->prev->rev->next->color = 1;
		g->vert->e->prev->rev->next->rev->color = 1;
	}
	// if g->n == 1 just return
	return;
}

// sets v->visited = false for all vertices in the graph
void set_visited_false(struct graph* g){
	struct vertex* v = g->vert;
	int i;
	for(i = 0; i < g->n; i++){
		v->visited = false;
		v = v->next;
	}
}

// dfs to separate the vertex lists of the inside and outside graphs of a cycle with visited = true, remove from g1 and add to g2
void graph_separating_dfs(struct vertex* v, struct graph* g1, struct graph* g2){
	struct edge* e = v->e;
	int i;
	v->visited = true;
	remove_from_vertex_list(g1, v);
	add_to_vertex_list(g2, v);
	for(i = 0; i < v->deg; i++){
		if(e->rev->v->visited == false) graph_separating_dfs(e->rev->v, g1, g2);
	}
}

// changes the colors in a graph
void color_change(struct graph* g, int perm[4]){
	struct vertex* v = g->vert;
	struct edge* e;
	int i, j;
	for(i = 0; i < g->n; i++){
		e = v->e;
		for(j = 0; j < v->deg; j++){
			e->color = perm[ e->color ];
			e = e->next;
		}
		v = v->next;
	}
}

// subcase of the short circuit subroutine, colors the graph recursively if it has e1 and e2 as parallel edges
void parallel_edges_coloring(struct graph* g1, struct edge* e1, struct edge* e2){
	struct edge* e;
	struct vertex* v;
	struct vertex* v1 = e1->v;
	struct vertex* v2 = e2->v;
	struct vertex* v1_copy = (struct vertex*) malloc(sizeof(struct vertex));;
	struct vertex* v2_copy = (struct vertex*) malloc(sizeof(struct vertex));;
	struct graph* g2 = (struct graph*) malloc(sizeof(struct graph));
	int perm[4];
	
	if(v1_copy == NULL || v2_copy == NULL){
		printf("Not enough space for a new vertex in parallel_edges_coloring\n");
		ERROR = true;
		return;
	}
	if(g2 == NULL){
		printf("Not enough space for a new graph in parallel_edges_coloring\n");
		ERROR = true;
		return;
	}
	if(v1 != e2->rev->v || v2 != e1->rev->v){
		printf("Invalid parallel edges at parallel_edges_coloring: distinct pairs of vertices\n");
		ERROR = true;
		return;
	}
	if(e1->rev == e2){
		printf("Invalid parallel edges at parallel_edges_coloring: same edge\n");
		ERROR = true;
		return;	
	}
	
	g2->vert = NULL;
	g2->n = 0;
	
	// create 2 new vertices and separate G_inside and G_outside
	
	v1_copy->num = num_label++;
	v2_copy->num = num_label++;
	v1_copy->deg = 0;
	v2_copy->deg = 0;
	v1_copy->e = e1;
	v2_copy->e = e1->rev;
	v1->e = e2->rev;
	v2->e = e2;
	
	add_to_vertex_list(g2, v1_copy);
	add_to_vertex_list(g2, v2_copy);
	
	set_visited_false(g1);
	v1->visited = true;
	v2->visited = true;

	// organizes the vertices and edges inside the loop 
	e = e1->next;	
	while(e != e2->rev){
		if(e->rev->v->visited == false){
			graph_separating_dfs(e->rev->v, g1, g2);
		}
		e->v = v1_copy;
		e = e->next;
		v1->deg--;
		v1_copy->deg++;
	}
	e = e1->rev->prev;
	while(e != e2){
		if(e->rev->v->visited == false){
			graph_separating_dfs(e->rev->v, g1, g2);
		}
		e->v = v2_copy;
		v2->deg--;
		v2_copy->deg++;
		e = e->prev;
	}
	
	e1->v->deg--;
	e1->rev->v->deg--;
	
	e1->rev->v = v2_copy;
	e1->v = v1_copy;
	
	v1_copy->deg++;
	v2_copy->deg++;
	
	
	e1->prev->next = e2->rev;
	e2->rev->prev->next = e1;
	e = e1->prev;
	e1->prev = e2->rev->prev;
	e2->rev->prev = e;
	
	e2->next->prev = e1->rev;
	e1->rev->next->prev = e2;
	e = e2->next;
	e2->next = e1->rev->next;
	e1->rev->next = e;

	// G1 = e1 + G_inside
	// G2 = e2 + G_outside
	
	find_coloring(g1);
	find_coloring(g2);
	
	
	// merge colorings
	if(e1->color != e2->color){
		perm[e1->color ^ e2->color] = e1->color ^ e2->color;
		perm[e1->color] = e2->color;
		perm[e2->color] = e1->color;
		if(g1->n <= g2->n) color_change(g1, perm);
		else color_change(g2, perm);
	}
	
	// undo the changes
	e1->prev->next = e2->rev;
	e2->rev->prev->next = e1;
	e = e1->prev;
	e1->prev = e2->rev->prev;
	e2->rev->prev = e;
	
	e2->next->prev = e1->rev;
	e1->rev->next->prev = e2;
	e = e2->next;
	e2->next = e1->rev->next;
	e1->rev->next = e;
	
	e = e1->rev;
	while(e != e2){
		e->v = v2;
		e = e->prev;
		v2->deg++;
	}
	e = e1;
	while(e != e2->rev){
		e->v = v1;
		e = e->next;
		v1->deg++;
	}
	
	v = g2->vert;
	while(g2->n > 0){
		remove_from_vertex_list(g2, v);
		add_to_vertex_list(g1, v);
	}
	free(v1_copy);
	free(v2_copy);
	free(g2);
}

// removes an edge e from the list of edges of e->v
void remove_from_edge_list(struct edge* e){
	if(e == NULL){
		printf("Trying to remove a NULL edge in remove_from_edge_list\n");
		ERROR = true;
		return;
	}
	if(e->v->deg == 1){
		printf("Trying to remove the only edge of a vertex in remove_from_edge_list\n");
		ERROR = true;
		return;
	}
	if(e->v->e == e){
		e->v->e = e->v->e->next;
	}
	e->next->prev = e->prev;
	e->prev->next = e->next;
	e->v->deg--;
}

// removes a vertex v from the vertex list of graph g
// NOTE: does NOTHING to the edges of v
void remove_from_vertex_list(struct graph* g, struct vertex* v){
	if(g->n == 1){
		g->n = 0;
		g->vert = NULL;
		return;
	}
	if(g->vert == v){
		g->vert = v->next;
	}
	v->next->prev = v->prev;
	v->prev->next = v->next;
	g->n--;
}

// adds vertex v to the "end" of the vertex list of graph g
void add_to_vertex_list(struct graph* g, struct vertex* v){
	if(g == NULL){
		printf("Null graph given to add_to_vertex_list\n");
		ERROR = true;
		return;
	}
	if(v == NULL){
		printf("Null vertex given to add_to_vertex_list\n");
		ERROR = true;
		return;
	}
	
	g->n++;
	if(g->vert == NULL){
		g->vert = v;
		v->next = v->prev = v;
		return;
	}
	g->vert->prev->next = v;
	v->prev = g->vert->prev;
	g->vert->prev = v;
	v->next = g->vert;
}

// puts back edges of v2 into edges of v1
// assumes first edge to be added is v2->e->next, CW of v1->e;
void fuse_vertex(struct vertex* v1, struct vertex* v2){
    struct edge* e1 = v1->e;
    struct edge* e1_end = e1->next;
    struct edge* e2 = v2->e->next;
    v1->deg += v2->deg-2;
    e1->next = e2;
    e2->prev = e1;
    // change source vertex
    while(e2->next != v2->e) {
        e2->v = v1;
        e2 = e2->next;
    }
    e2->prev->next = e1_end;
    e1_end->prev = e2->prev;
}

// eliminates vertices of degree three
void degree_three_elimination(struct graph* g, struct vertex* head){
	struct vertex* v;
	struct vertex* tail;	// linked list of vertices deleted
	struct edge* e;
	int i;
	if(head->deg != 3){
		printf("Invalid input at degree_three_elimination, vertex %d has degree %d\n",head->num, head->deg);
		ERROR = true;
	}
	// let v be the next meeber in the vertex list
	v = head->next;
	// remove v from the vertex list
	remove_from_vertex_list(g, head);
	// create linked list of vertices to delete 
	tail = head;
	head-> prev = tail->next = NULL;
	while(v != g->vert){
		if(v->deg == 3){
			// remove it from the vertex list
			remove_from_vertex_list(g, v);
			// add v to the list of vertices to delete 
			tail->next = v;
			// update v
			v = v->next;
			// update the list of vertices to delete
			tail->next->prev = tail;
			tail->next->next = NULL;
			tail = tail->next;
		}
		else v = v->next;
	}
	v = head;
	while(v != NULL){
		e = v->e;
		for(i = 0; i < v->deg; i++){
			// disconnect e->rev from the graph
			e->rev->next->prev = e->rev->prev;
			e->rev->prev->next = e->rev->next;
			e->rev->v->deg--;
			if(e->rev->v->e == e->rev){
				e->rev->v->e = e->rev->next;
			}
			// possibly add e->rev->v to the list of vertices to delete
			if(e->rev->v->deg == 3 && g->n >= 4){
				// remove e->rev->v from the vertex list
				remove_from_vertex_list(g, e->rev->v);
				// add v to the list of vertices to delete 
				tail->next = e->rev->v;
				e->rev->v->prev = tail;
				e->rev->v->next = NULL;
				tail = tail->next;
			}
			e = e->next;
		}
		v = v->next;
	}
	find_coloring(g);
	if(ERROR) return;
	v = tail;
	while(v != NULL){
		// add v back to the graph and colors the edges of v in the correct color
		e = v->e;
		for(i = 0; i < v->deg; i++){
			e->rev->next->prev = e->rev;
			e->rev->prev->next = e->rev;
			e->rev->v->deg++;
			e->color = e->rev->color = e->rev->next->color ^ e->rev->prev->color;
			e = e->next;	
		}
		v = v->prev;
		// add the next vertex back to the list of vertices of g
		if(v != NULL) add_to_vertex_list(g, v->next);
	}
	// add the head back to the list of vertices of g
	add_to_vertex_list(g, head);
}

// dfs to check graph isomorphism
bool iso_dfs(int r, struct vertex* vg, struct vertex* vc){
	struct edge* eg;
	struct edge* ec;
	int i;
	
	map[vc->num] = vg;
	eg = vg->e;
	ec = vc->e;
	if(vc->num <= r) return true;
	if(vg->deg != vc->deg) return false;
	for(i = 0; i < vg->deg; i++){
		if(ec->rev->v->visited == false){
			ec->rev->v->e = ec->rev;
			eg->rev->v->e = eg->rev;
			if( ! iso_dfs(r, eg->rev->v, ec->rev->v) ) return false;
		}
	}
	return true;
}

// given a hub finds a configuration in its cartwheel
int find_configuration(struct vertex* hub){
	int i, j, k;
	struct edge* e1;
	struct edge* e2;
	struct vertex* v;
	
	for(i = 0; i < NUM_CONF; i++){
		v = conf[i].g.vert;
		for(j = conf[i].r + 1; j <= conf[i].n; j++){
			if(hub->deg != v->deg){
				v = v->next;
				continue;
			}
			e1 = hub->e;
			e2 = v->e;
			for(k = 0; k < hub->deg; k++){
				hub->e = e1;
				v->e = e2;
				set_visited_false(&conf[i].g);
				if( iso_dfs(conf[i].r, hub, v) ) return i;
				e2 = e2->next;	
			}
			v = v->next;
		}
	}
	printf("Error: find_configuration couldn't find a configuration with hub %d\n",hub->num);
	ERROR = true;
	return -1;
}

// IMPLEMENT
// main coloring function
void find_coloring(struct graph* g){
	struct vertex* v;
	struct vertex* hub;
	struct edge* e1;
	struct edge* e2;
	int i, j, k;
	int conf_num;
	
	// if the graph is small we are at a base case
	if(g->n <= 3){
		base_case_coloring(g);
		return;
	}
	
	// searches for parallel edges and run SCS for size = 2 if we find any
	/*
	set_visited_false(g);
	v = g->vert;
	for(i = 0; i < g->n; i++){
		e1 = v->e;
		for(j = 0; j < v->deg; j++){
			if(e1->rev->v->visited == true){	// found parallel edges
				e2 = v->e;
				for(i = 0; i < j; i++){
					if(e1->rev->v == e2->rev->v){
						parallel_edges_coloring(g, e1, e2->rev);
						break;	
					}
					e2 = e2->next;
				}
				return;
			}
			e1->rev->v->visited = true;
			e1 = e1->next;
		}
		for(j = 0; j < v->deg; j++){
			e1->rev->v->visited = false;
			e1 = e1->next;
		}
		v = v->next;
	}
	
	// if the graph is small we are at a base case, now with no parallel edges
	if(g->n == 4){
		base_case_coloring(g);
		return;
	}
	
	// searches for vertices of degree <= 3 and deletes them
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->deg == 3 ){
			degree_three_elimination(g, v);
			return;	
		}
		v = v->next;
	}
		
	// searches for a vertex of degree 4 and run SCS for the short circuit
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->deg == 4 ) break;
		v = v->next;
	}
	if(i < g->n){
		// run SCS for the short circuit formed by the neighbors of v1
		// add the vertices of degree <= 3 back
		return;
	}
	*/
	// no simple reduction, search for a hub 
	hub = find_hub(g);
	if(ERROR) return;
	
	debug("Hub == %d\n",hub->num);
	
	
	conf_num = find_configuration(hub);
	if(ERROR) return;
	
	debug("Config = %d\n",conf_num);
	
	for(i = 1; i <= conf[conf_num].n; i++){
		debug("Map[%d] = %d\n",i,map[i]->num);
	}
	
	// look for a short circuit in the cartweel of hub
	// contract the configuration
	// call find_coloring recursively
	// undo contraction and extend coloring
	
}

// debug function
void check_graph(struct graph* g){
	struct vertex *v;
	struct edge *e;
	int i,j;
	
	// basic structural integrity
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->next->prev != v ){
			debug("Vertex list invalid, v->next->prev != v for v->num == %d\n",v->num);
			ERROR = true;
//			return;
		}
		if( v->prev->next != v ){
			debug("Vertex list invalid, v->prev->next != v for v->num)== %d\n",v->num);
			ERROR = true;
//			return;
		}
		if(i > 0 && v == g->vert){
			debug("Repeated vertex in vertex list\n");
			ERROR = true;
//			return;
		}
		e = v->e;
		for(j = 0; j < v->deg; j++){
			if(e->v != v){
				debug("Invalid edge, e->v != v for vertex %d\n",v->num);
				ERROR = true;
//				return;
			}
			if(e->rev->rev != e){
				debug("Invalid edge pair, e->rev->rev != e for e->v->num == %d, e->rev->v->num == %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
//				return;
			}
			if(e->rev->v == v){
				debug("Self loop at vertex %d\n",v->num);
				ERROR = true;
//				return;
			}
			if(e->next->prev != e){
				debug("Invalid edge pair, e->next->prev != e for e->v->num == %d, e->rev->v->num = %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
//				return;
			}
			if(e->prev->next != e){
				debug("Invalid edge pair, e->prev->next != e for e->v->num == %d, e->rev->v->num = %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
//				return;
			}
			if(j > 0 && e == v->e){
				debug("Invalid edge list, repeated edge in the edge list of %d\n",v->num);
				ERROR = true;
//				return;
			}
			e = e->next;	
		}
		if(e != v->e){
			debug("Invalid degree for vertex %d, edge list too small\n",v->num);
			ERROR = true;
//			return;
		}
		v = v->next;
	}
	if( v != g->vert ){
		debug("Invalid number of vertices in the graph, edge list too small\n");
		ERROR = true;
//		return;
	}
	// check triangulation
	v = g->vert;
	for(i = 0; i < g->n; i++){
		e = v->e;
		for(j = 0; j < v->deg; j++){
			if( e->rev->next->rev->next->rev->v != v){
				debug("Non-triangular face with vertices: \n");
				ERROR = true;
//				return;
			}
			e = e->next;
		}
		v = v->next;	
	}
	if(!ERROR) debug("Graph ok\n");
}

// reads list of configurations from text file according to the structure defined above
void read_conf(char* data){
	// 1. N a character string identifying the configuration;
	// 2. n is the number of vertices of the free completion;
	// 3. r is the ring-size;
	// 4. a is the cardinality of C ; and
	// 5. b is the cardinality of C' (see discussion before (3.2) in the paper);
	// 6. k [2*k integers]; k is the number of edges in X, each edge being represented as a pair of integers;
	// 7. the adjacency list of the free completion in a standard form (the second column contains the degrees in the free completion);
	// 8. (ignored) coordinates of vertices of the free completion; 
	// the i-th entry in the coordinate list is 1024 x + y where [x,y] are the coordinates of vertex i, (0 < = x,y < 1024) .

	char N[100];
	int i, j, k, u, skip;
    int X_edges[8];
	struct vertex* v1;
	struct vertex* v2;
	struct edge* e;
	FILE* ptr = fopen(data,"r");

	for (i = 0; i < 633; i++) {
		fscanf(ptr,"%s",conf[i].N);
		fscanf(ptr,"%d",&conf[i].n);
		fscanf(ptr,"%d",&conf[i].r);
		fscanf(ptr,"%d",&conf[i].a);
		fscanf(ptr,"%d",&conf[i].b);
        fscanf(ptr,"%d",&conf[i].x);
        
		conf[i].g.vert = NULL;
		conf[i].g.n = 0;

		// create the vertices
		for(j = 1; j <= conf[i].n; j++){
			v1 = (struct vertex*) malloc(sizeof(struct vertex));
			if(v1 == NULL){
				printf("read_conf: Memory error: not enough space for new vertex in function read_graph\n");
				ERROR = true;
				return;
			}
			v1->num = j;
			add_to_vertex_list(&conf[i].g, v1);
			v1->e = NULL;
		}
		v1 = conf[i].g.vert;
        // store the vertices in X: make the edges after graph constructed below
        for(j = 0; j < conf[i].x; j++) {
            int a;
            int b;
            fscanf(ptr, "%d", &X_edges[2*j]);
            fscanf(ptr, "%d", &X_edges[2*j+1]);
            if(X_edges[2*j] > conf[i].n | X_edges[2*j+1] > conf[i].n){
                printf("read_conf: Invalid vertex in X has number higher than n \n");
                ERROR = true;
                return;
            }
        }
        // create the graph from adjacency list (similar to read_graph)
		for(j = 1; j <= conf[i].n; j++){
			v1->e = NULL;
			fscanf(ptr,"%d",&skip);
			fscanf(ptr, "%d", &v1->deg);
			if(v1->deg < 0){
				printf("Invalid degree, must be >= 0\n");
				ERROR = true;
				return;
			}
            // for each vertex store its neighbors
			for(k = 0; k < v1->deg; k++){
				fscanf(ptr,"%d",&u);
				if(u <= 0 && u > conf[i].g.n){
					printf("Invalid vertex, must be between 1 and n\n");
					ERROR = true;
					return;
				}
				v2 = v1->next;
				while(v2->num != u){
					v2 = v2->next;
				}
				add_edge(v1, v2);
			}
			v1 = v1->next;
		}
        // store the edges to conf.X by going through vertices and finding them
        for (j = 0; j < conf[i].x; j++) {
            // find the first vertex
            v1 = conf[i].g.vert;
            while(v1->num != X_edges[2*j]){
                v1 = v1->next;
            }
            e = v1->e;
            while(e->rev->v->num != X_edges[2*j+1]) {
                e = e->next;
            }
            conf[i].X[j] = e;
        }
		for (j = 0; j < conf[i].g.n; j++) {
			fscanf(ptr,"%d",&skip);;
		}
	}
	fclose(ptr);
}

struct graph* split_graph(struct graph* g1, int size, struct edge* circuit[5]) {
    int i;
    struct graph* g2;
    struct edge*e;
    struct vertex* vnew_list[5];
    struct vertex * v_interior; // used in graph separating dfs

    // Some of these steps below could be combined into one loop, but
    // for the sake of readability they were separated since the number
    // of times each is called is leq 6

    // 1. create vertices
    for (i = 0; i < size; i++)
    {
        vnew_list[i] = (struct vertex*) malloc(sizeof(struct vertex));
        vnew_list[i]->num = circuit[i]->v->num;
    }
    // 2. add edges between vertices
    for (i = 0; i < size; i++)
    {
        add_edge(vnew_list[i], vnew_list[(i+1)%size]);
        vnew_list[i]->deg++;
        add_edge(vnew_list[i], vnew_list[((i-1)+size)%size]);
        vnew_list[i]->deg++;
    }
    // 3. enforce ordering on vertices to have clockwise (same as input edges in scs)
    for (i = 0; i < size; i++)
    {
        vnew_list[i]->e = find_edge(vnew_list[i], vnew_list[(i+1)%size]);
        circuit[i]->v->e = circuit[i];
    }
    // 4. create new graph g2
    g2 = (struct graph*) malloc(sizeof(struct graph));
    g2->n = 0;
	g2->vert = NULL;
    // 5. call graph separating dfs (alters the vertex list of g1 and g2)
    for (i = 0; i < size; i++)
    {
        circuit[i]->v->visited = true;
    }
    v_interior = circuit[0]->next->rev->v; // assumed that there is an interior vertex
    graph_separating_dfs(v_interior,g1,g2);
    for (i = 0; i < size; i++)
    {
        add_to_vertex_list(g2,vnew_list[i]);
    }
    // 6. remap the internal edges into g2
    for (i = 0; i < size; i++) {
        e = circuit[i]->next;
        e->prev = vnew_list[i]->e;
        vnew_list[i]->e->next = e;
        while (e!=circuit[(i-1+size)%size]->rev) {
            e->v = vnew_list[i];
            vnew_list[i]->deg++;
            e = e->next;
        }
        vnew_list[i]->e->prev->prev = e->prev; // connect outer parts
        e->prev->next = vnew_list[i]->e->prev;
    }
    // 7. remap the internal edges out of g1
    for (i = 0; i < size; i++)
    {
        circuit[i]->next = circuit[(i-1+size)%size]->rev;
        circuit[(i-1+size)%size]->rev->prev = circuit[i];
        circuit[i]->v->deg -= vnew_list[i]->deg - 2;
    }
    return g2;
}


// short circuit subroutine for circuit size 3
// cw for g2
// split the graph into two graphs, and run find_coloring on both
// creates a new circuit for one of the graphs, the other uses original.
// ei are the edges of the short circuit, in cw orientation
void scs_3(struct graph* g1,struct edge* e1,struct edge* e2,struct edge* e3){
    struct graph* g2;
    struct edge* e;
    // 1. create new vertices
    struct vertex * vnew_1;
    struct vertex * vnew_2;
    struct vertex * vnew_3;
    struct edge* edge_list[3];
    edge_list[0] = e1;
    edge_list[1] = e2;
    edge_list[2] = e3;
    g2 = split_graph(g1, 3, edge_list);
    vnew_1 = g2->vert->prev->prev->prev;
    vnew_2 = g2->vert->prev->prev;
    vnew_3 = g2->vert->prev;
    // 6. color the two divided graph
	find_coloring(g1);
	find_coloring(g2);
    // 7. match the colorings
    int perm[4];
    perm[e1->color]=vnew_1->e->color;
    perm[e2->color]=vnew_2->e->color;
    perm[e3->color]=vnew_3->e->color;
    color_change(g2, perm);
    // 8. fuse the graphs back together
    fuse_vertex(e1->v, vnew_1);
    fuse_vertex(e2->v, vnew_2);
    fuse_vertex(e3->v, vnew_3);
    // 9. change the vertex list of g1
    remove_from_vertex_list(g2,vnew_1);
    remove_from_vertex_list(g2,vnew_2);
    remove_from_vertex_list(g2,vnew_3);
    // 10. combine the vertex lists
    struct vertex* v_temp = g1->vert->next;
    g1->vert->next = g2->vert->next;
    g2->vert->next->prev = g1->vert;
    g2->vert->next = v_temp;
    v_temp->prev = g2->vert;
    g1->n += g2->n;
}

int main(){
	struct graph* g;
	char data[256];
	
	read_conf("configurations.txt");
	
	printf("Enter file name including extension: \n");
	scanf("%s", data);
//	g = read_graph("rtest.txt");
/*
	struct vertex *v1, *v2;
	v1 = g->vert;
	v2 = v1->next;
	printf("%d %d\n",v1->num, v2->num);
	contract_edge( g, find_edge(v1, v2) );
	print_graph(g);
*/	
	
	find_coloring(g);
	print_graph(g);
	color_vertices(g);
	free_graph(g);
	printf("Leaking memory: %d\n",memory_in_use);
	return 0;
}
