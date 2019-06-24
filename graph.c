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
	int color;
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
} conf[NUM_CONF + 1];

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
void scs_4(struct graph*,struct edge*,struct edge*,struct edge*, struct edge*);
struct edge* create_edge_between(struct vertex*, struct vertex*, struct edge*, struct edge*);
void graph_separating_dfs(struct vertex*, struct graph*, struct graph*);
void color_change(struct graph*, int*);
void fuse_vertex(struct vertex*, struct vertex*);

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

	for (i = 1; i <= NUM_CONF; i++) {
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
    for (i = 0; i < size; i++) {
        vnew_list[i] = (struct vertex*) malloc(sizeof(struct vertex));
        vnew_list[i]->num = circuit[i]->v->num;
        vnew_list[i]->e = NULL;
        vnew_list[i]->deg = 0;
    }
    // 2. add edges between vertices
    for (i = 0; i < size; i++) {
        add_edge(vnew_list[i], vnew_list[(i+1)%size]);
        vnew_list[i]->deg++;
        add_edge(vnew_list[i], vnew_list[((i-1)+size)%size]);
        vnew_list[i]->deg++;
    }
    // 3. enforce ordering on vertices to have clockwise (same as input edges in scs)
    for (i = 0; i < size; i++) {
        vnew_list[i]->e = find_edge(vnew_list[i], vnew_list[(i+1)%size]);
        circuit[i]->v->e = circuit[i];
    }
    // 4. create new graph g2
    g2 = (struct graph*) malloc(sizeof(struct graph));
    g2->n = 0;
	g2->vert = NULL;
    // 5. call graph separating dfs (alters the vertex list of g1 and g2)
    for (i = 0; i < size; i++){
        circuit[i]->v->visited = true;
    }
    v_interior = circuit[0]->next->rev->v; // assumed that there is an interior vertex
    graph_separating_dfs(v_interior,g1,g2);
    for (i = 0; i < size; i++) {
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
    for (i = 0; i < size; i++) {
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

// deletes edges of a configuration in a graph
// stores them in a pointer array
// needs to then be triangulated
struct edge** delete_conf(struct vertex* vertex_mapping[26], int conf_num) {
    struct configuration config = conf[conf_num]; // current configuration
    int i, j, k, l;
    k = 0;
    struct vertex* v;
    struct edge* e;
    struct edge** edge_deleted = (struct edge**) malloc(sizeof(struct edge*)*26);
    v = vertex_mapping[config.r+1];
    e = v->e;
    for (i = config.r+1; i <= config.n; i++) { // from R+1 to N we have configuration's vertices
        v = vertex_mapping[i];
        e = v->e;
        l = v->deg;
        for (j = 0; j < l; j++) {
            e->prev->next = e->next;
            e->next->prev = e->prev;
            if (e->rev->v->e == e->rev) {
                e->rev->v->e = e->rev->v->e->next;
            }
            e->rev->next->prev = e->rev->prev;
            e->rev->prev->next = e->rev->next;
            e->v->deg--;
            e->rev->v->deg--;
            edge_deleted[k++]=e;
            e = e->next;
        }
    }
    return edge_deleted;
}

// reinserts deleted edges of configuration back into the graph
// number of deleted edges is (3n-2r-3)/2, formula based off euler
// called like: reinsert_conf(edge_deleted, 3*conf0.n-2*conf0.r-3);
void reinsert_conf(struct edge** edge_deleted, int num_edges) {
    struct edge* e;
    int i;
    for (i = 0; i < num_edges; i++) {
        e = edge_deleted[num_edges-1-i];
        e->prev->next = e;
        e->next->prev = e;
        e->rev->next->prev = e->rev;
        e->rev->prev->next = e->rev;
        e->v->deg++;
        e->rev->v->deg++;
    }
}

// creates an edge between e1->v and e2->v, cyclically after e1 around v and after e2 around v2
struct edge* create_edge_between(struct vertex* v1, struct vertex* v2, struct edge* e1, struct edge* e2){
	struct edge* e_new1 = (struct edge*) malloc(sizeof(struct edge));
	struct edge* e_new2 = (struct edge*) malloc(sizeof(struct edge));
	
	if(e_new1 == NULL || e_new2 == NULL){
		printf("Memory error: not enough space for new vertex in fcreate_edge_between\n");
		return NULL;
	}
	// make the edge point to one another
	e_new1->rev = e_new2;
	e_new2->rev = e_new1;
	// initialize their colors as zero
	e_new1->color = 0;
	e_new2->color = 0;
	// initialize their vertices
	e_new1->v = v1;
	e_new2->v = v2;
	// put e_new1 in the right position in the edge list of v1
	if(e1 != NULL){
		e1->next->prev = e_new1;
		e_new1->next = e1->next; 
		e1->next = e_new1;
		e_new1->prev = e1; 
	}
	else{
		v1->e = e_new1;
		e_new1->next = e_new1->prev = e_new1;
	}
	v1->deg++;
	// put e_new2 in the right position in the edge list of v2
	if(e2 != NULL){
		e2->next->prev = e_new2;
		e_new2->next = e2->next; 
		e2->next = e_new2;
		e_new2->prev = e2; 
	}
	else{
		v2->e = e_new2;
		e_new2->next = e_new2->prev = e_new2;
	}
	v2->deg++;
	// returns one of the new edges
	return e_new1;	
}


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
	remove_from_vertex_list(g2, v1_copy);
	remove_from_vertex_list(g2, v2_copy);
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

// add symmetry support 
// dfs to check graph isomorphism
bool iso_dfs(int r, struct vertex* vg, struct vertex* vc, bool clockwise){
	struct edge* eg;
	struct edge* ec;
	int i;
	
	map[vc->num] = vg;
	eg = vg->e;
	ec = vc->e;
	vc->visited = true;
	if(vc->num <= r) return true;
	if(vg->deg != vc->deg) return false;
	for(i = 0; i < vg->deg; i++){
		if(ec->rev->v->visited == false){
			ec->rev->v->e = ec->rev;
			eg->rev->v->e = eg->rev;
			if( ! iso_dfs(r, eg->rev->v, ec->rev->v, clockwise) ) return false;
		}
		eg = eg->next;
		ec = clockwise ? ec->next : ec->prev;
	}
	return true;
}

// add symmetry support 
// given a hub finds a configuration in its cartwheel
int find_configuration(struct vertex* hub){
	int i, j, k;
	struct edge* e1;
	struct edge* e2;
	struct vertex* v;
	
	for(i = 1; i <= NUM_CONF; i++){
		v = conf[i].g.vert->prev;
		for(j = conf[i].r + 1; j <= conf[i].n; j++){
			if(hub->deg != v->deg){
				v = v->prev;
				continue;
			}
			e1 = hub->e;
			e2 = v->e;
			for(k = 0; k < hub->deg; k++){
				hub->e = e1;
				v->e = e2;
				
				set_visited_false(&conf[i].g);
				if( iso_dfs(conf[i].r, hub, v, true) ) return i;
				set_visited_false(&conf[i].g);
				if( iso_dfs(conf[i].r, hub, v, false) ) return i;
				e1 = e1->next;	
			}
			v = v->prev;
		}
	}
	printf("Error: find_configuration couldn't find a configuration with hub %d\n",hub->num);
	ERROR = true;
	return 0;
}

// finds a short circuit in the neighborhood of hub
// assumes no parallel edges, no loops and min deg >= 5
// if no short circuit is found then all vertices in the cartwheel of hub 
// are distinct and hats/spokes have no extra edges between them
struct edge** find_short_circuit(struct vertex* hub){
	struct edge** circuit;
	struct edge* e1;
	struct edge* e2;
	struct edge* e3;
	struct vertex* v1;
	struct vertex* v2;
	struct vertex* v3;
	struct vertex* vsc = NULL;
	int i, j, k, size, cur;
	bool second_path = false;
	
	circuit = (struct edge**) malloc(sizeof(struct edge*) * 5);
	
	if(circuit == NULL){
		printf("Memory error: insuficcient memory for circuit in find_short_circuit\n");
	}
	
	/* sets visited to false */
	hub->visited = false;
	e1 = hub->e;
	for(i = 0; i < hub->deg; i++){
		e2 = e1->rev;
		v1 = e2->v;
		v1->visited = false;
		e2 = e2->next->next;	/* starts at the first hat and then go through the fans */
		for(j = 4; j < v1->deg; j++){	/* stops before the second hat */
			v2 = e2->rev->v;
			v2->visited = false;
			e3 = e2->rev->next->next;
			for(k = 3; k < v2->deg; k++){
				if(j == 4 && k == 3){		// hats must go one more edge before leaving the cartwheel
					e3 = e3->next;
					continue;
				}
				v3 = e3->rev->v;
				v3->visited = false;
				e3 = e3->next;
			}
			e2 = e2->next;		
		}
		e1 = e1->next;
	}
	/* sets the hub and the spokes to visited */
	/* since we have no parallel edge and no loops, the hubs and spokes are all different */
	e1 = hub->e;
	hub->visited = true;
	for(i = 0; i < hub->deg; i++){
		e1->rev->v->visited = true;
		e1 = e1->next;
	}
	/* checks the hats and the fans to look for visited vertices(spokes) */
	/* finds cycles of size 3 */
	e1 = hub->e;
	for(i = 0; i < hub->deg; i++){
		e2 = e1->rev;
		e2 = e2->next->next;	/* starts at the first hat and then go through the fans */
		v1 = e2->v;
		for(j = 4; j < v1->deg && vsc == NULL; j++){	/* stops before the second hat */
			v2 = e2->rev->v;
			if( v2->visited ){
				vsc = v2;
				size = 3;
				circuit[2] = e2->rev;
				circuit[1] = e1->rev;
				debug("size = 3\n");
				break;
			}
			e2 = e2->next;		
		}
		e1 = e1->next;
	}
	if(vsc == NULL){
		/* sets the hats and the fans to visited */
		/* finds cylces of size 4 */
		e1 = hub->e;
		for(i = 0; i < hub->deg; i++){
			e2 = e1->rev;
			e2 = e2->next->next;	/* starts at the first hat and then go through the fans */
			v1 = e2->v;
			for(j = 4; j < v1->deg; j++){	/* stops before the second hat */
				v2 = e2->rev->v;
				if(v2->visited){
					vsc = v2;
					size = 4;
					circuit[3] = e1->rev;
					circuit[2] = e2->rev;
					debug("size = 4\n");
					break;
				}
				v2->visited = true;
				e2 = e2->next;		
			}
			e1 = e1->next;
		}
	}
	if(vsc == NULL){
		/* goes to distance 3 and looks for visited vertices (hats and fans) */
		/* finds cylces of size 5 */
		e1 = hub->e;
		for(i = 0; i < hub->deg && vsc == NULL; i++){
			e2 = e1->rev->next->next;	/* starts at the first hat and then go through the fans */
			v1 = e2->v;
			for(j = 4; j < v1->deg && vsc == NULL; j++){	/* stops before the second hat */
				e3 = e2->rev->next->next;
				v2 = e3->v;
				for(k = 3; k < v2->deg; k++){
					if(j == 4 && k == 3){		// hats must go one more edge before leaving the cartwheel
						e3 = e3->next;
						continue;
					}
					v3 = e3->rev->v;
					if(v3->visited){
						vsc = v3;
						size = 5;
						circuit[4] = e1->rev;
						circuit[3] = e2->rev;
						circuit[2] = e3->rev;
						debug("size = 5, v1 = %d, v2 = %d, v3 = %d\n",v1->num, v2->num, v3->num);
						debug("e1 = %d -> %d\n",e1->v->num, e1->rev->v->num);
						debug("e2 = %d -> %d\n",e2->v->num, e2->rev->v->num);
						debug("e3 = %d -> %d\n",e3->v->num, e3->rev->v->num);
						break;
					}
					e3 = e3->next;
				}
				e2 = e2->next;		
			}
			e1 = e1->next;
		}
	}
	if(vsc == NULL) return NULL;
	/* looks for the short circuit once we have a repeated vertex */
	e1 = hub->e;
	debug("Hub = %d\n",hub->num);
	for(i = 0; i < hub->deg; i++){
		e2 = e1->rev;
		v1 = e2->v;
		debug("  Spoke = %d\n",v1->num);
		if(v1 == vsc){
			circuit[0] = e1;
			return circuit;
		}
		e2 = e2->next->next;	/* starts at the first hat and then go through the fans */
		for(j = 4; j < v1->deg; j++){	/* stops before the second hat */
			v2 = e2->rev->v;
			debug("    Hat/fan = %d\n",v2->num);
			if(v2 == vsc){
				circuit[0] = e1;
				circuit[1] = e2;
				return circuit;
			}
			e2 = e2->next;		
		}
		e1 = e1->next;
	}
	debug("Couldn't complete circuit\n");
	return circuit;
}

// debug function
// assumes graph is correct
void print_dfs(struct vertex* v){
	int i;
	struct edge* e = v->e;
	v->visited = true;
	printf("    vertex %d, deg %d : ", v->num, v->deg);
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
	
//	printf("Edges colors\n");
	v = g->vert;
	for(i = 0; i < g->n; i++){
		e = v->e;
		printf("Vertex %d, deg %d\n",v->num, v->deg);
		for(j = 0; j < v->deg; j++){
			printf("  Edge %d %d, color %d\n",e->v->num,e->rev->v->num,e->color);
			e = e->next;
		}
		v = v->next;
	}
	printf("\n");
	set_visited_false(g);
//	print_dfs(g->vert);
}

// IMPLEMENT
// main coloring function
void find_coloring(struct graph* g){
	struct vertex* v;
	struct vertex* hub;
	struct edge* e1;
	struct edge* e2;
	struct edge* e3;
	struct edge* e4;
	struct edge* e5;
	struct edge** short_circuit;
	int i, j, k;
	int conf_num;
	
	// if the graph is small we are at a base case
	if(g->n <= 3){
		base_case_coloring(g);
		return;
	}
	
	// searches for parallel edges and run SCS for size = 2 if we find any
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
		if( v->deg == 4 ){
			e1 = v->e->rev->prev;
			e2 = e1->rev->prev->prev;
			e3 = e2->rev->prev->prev;
			e4 = e3->rev->prev->prev;
			scs_4(g, e1, e2, e3, e4);
			return;
		}
		v = v->next;
	}
	
	// no simple reduction, search for a hub 
	hub = find_hub(g);
	if(ERROR) return;
	
	debug("Hub == %d\n",hub->num);
	
	short_circuit = find_short_circuit(hub);
	
	if(short_circuit == NULL) debug("No short circuit found\n");
	else{
		int size = 3;
		if(short_circuit[3] != NULL) size++;
		if(short_circuit[4] != NULL) size++;
		int i;
		debug("Found a short circuit\n");
		for(i = 0; i < size; i++){
			debug("  Edge %d %d\n",short_circuit[i]->v->num, short_circuit[i]->rev->v->num);
		}
	}
		
	conf_num = find_configuration(hub);
	if(ERROR) return;
	
	for(i = 1; i <= conf[conf_num].n; i++){
		debug("Map[%d] = %d\n",i,map[i]->num);
	}
	// look for a short circuit in the cartweel of hub
	// contract the configuration
	// call find_coloring recursively
	// undo contraction and extend coloring
	return;
}


void renumber_vertices(struct graph* g){
	struct vertex* v = g->vert;
	int i;
	for(i = 1; i <= g->n; i++){
		v->num = i;
		v = v->next;
	}	
}

void output_graph(struct graph* g) {
    struct edge* e;
    struct vertex* v;
    int i, j;
    
    renumber_vertices(g);
    
    printf("%d\n", g->n);
    v = g->vert;
    for (i = 0; i < g->n;i++) {
        printf("%d ", v->deg);
        v = v->next;
    }
    printf("\n");
    for (i = 0; i < g->n;i++) {
        e = v->e;
        for (j = 0; j < v->deg; j++) {
            printf("%d ", e->rev->v->num);
            e = e->next;
        }
        printf("\n");
        v = v->next;
    }
}

// debug function
void check_graph(struct graph* g){
	struct vertex *v;
	struct edge *e;
	struct edge *e2;
	int i,j;
	
	// basic structural integrity
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->next->prev != v ){
			printf("Vertex list invalid, v->next->prev != v for v->num == %d\n",v->num);
			ERROR = true;
//			return;
		}
		if( v->prev->next != v ){
			printf("Vertex list invalid, v->prev->next != v for v->num)== %d\n",v->num);
			ERROR = true;
//			return;
		}
		if(i > 0 && v == g->vert){
			printf("Repeated vertex in vertex list\n");
			ERROR = true;
//			return;
		}
		e = v->e;
		for(j = 0; j < v->deg; j++){
			if(e->v != v){
				printf("Invalid edge, e->v != v for vertex %d\n",v->num);
				ERROR = true;
//				return;
			}
			if(e->rev->rev != e){
				printf("Invalid edge pair, e->rev->rev != e for e->v->num == %d, e->rev->v->num == %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
//				return;
			}
			if(e->rev->v == v){
				printf("Self loop at vertex %d\n",v->num);
				ERROR = true;
//				return;
			}
			if(e->next->prev != e){
				printf("Invalid edge pair, e->next->prev != e for e->v->num == %d, e->rev->v->num = %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
//				return;
			}
			if(e->prev->next != e){
				printf("Invalid edge pair, e->prev->next != e for e->v->num == %d, e->rev->v->num = %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
//				return;
			}
			if(j > 0 && e == v->e){
				printf("Invalid edge list, repeated edge in the edge list of %d\n",v->num);
				ERROR = true;
//				return;
			}
			e = e->next;	
		}
		if(e != v->e){
			printf("Degree does not match size of edge list for vertex %d\n",v->num);
			ERROR = true;
//			return;
		}
		v = v->next;
	}
	if( v != g->vert ){
		printf("Number of vertices does not match size of vertex list\n");
		ERROR = true;
//		return;
	}
	// check triangulation
	v = g->vert;
	for (i = 0; i < g->n; i++) {
		e = v->e;
		for (j = 0; j < v->deg; j++) {
			if( e->rev->next->rev->next->rev->v != v){
				printf("Non-triangular face with vertices: %d", e->v->num);
				e2 = e->rev->next;
				while(e2 != e){
					printf(" %d",e2->v->num);
					e2 = e2->rev->next;
				}
				printf("\n");
				ERROR = true;
//				return;
			}
			e = e->next;
		}
		v = v->next;	
	}
	// checks coloring
	bool colored = (g->vert->e->color != 0);
	v = g->vert;
	for (i = 0; i < g->n; i++) {
		e = v->e;
		for (j = 0; j < v->deg; j++) {
			if(colored){
				if(e->color >= 4 || e->color < 0){
					ERROR = true;
					printf("Error: invalid color in edge %d %d\n",e->v->num, e->rev->v->num);
				}
				else if(e->color == 0){
					ERROR = true;
					printf("Error: graph only partially colored\n");
				}
				if(e->color != e->rev->color){
					ERROR = true;
					printf("Error: edge %d %d and the reverse have different colors\n",e->v->num, e->rev->v->num);
				}
				if(e->color == e->next->color){
					ERROR = true;
					printf("Error: invalid coloring at face %d %d %d\n",e->v->num, e->next->rev->v->num, e->rev->v->num);
				}
			}
			else if(e->color != 0){
				ERROR = true;
				printf("Error: graph only partially colored\n");
			}
			e = e->next;
		}
		v = v->next;	
	}
	if(ERROR){
		output_graph(g);
		scanf("%*c");
	}
//	if(!ERROR) printf("Graph ok\n");
}

// performs a rib change of starting at edge e and going to the face between e and e->next
// admits that the previous face is the infinity face of a near triangulation (not a triangle)
void rib(struct edge* e, int color){
	while(e->next->rev->next->rev->next->rev == e){		// condition for face of size 3
		e->color ^= color;
		e->rev->color = e->color;
		if(e->next->color == color) e = e->next->rev->next;
		else e = e->next;
	}
	e->color ^= color;
	e->rev->color = e->color;
}

// circuit size four, contains vertex inside circuit, edges in cw order
void scs_4(struct graph* g1,struct edge* e1,struct edge* e2,struct edge* e3, struct edge* e4) {
    int i, j;
    int c_i = 0;
    int c_j = 0;
    struct graph* g2;
    struct vertex *vnew_1, *vnew_2, *vnew_3, *vnew_4;
    struct vertex* v_temp;
    struct edge* edge_list[4];
    struct edge* e;
    struct edge * enew_1;
    struct edge * enew_2;
    struct edge * enew_3;
    struct edge * enew_4;
    int perm[] = {0, 0, 0, 0};
    
    edge_list[0] = e1;
    edge_list[1] = e2;
    edge_list[2] = e3;
    edge_list[3] = e4;
    g2 = split_graph(g1, 4, edge_list);
    vnew_1 = g2->vert->prev->prev->prev->prev; // fourth last addition is added vertex
    vnew_2 = g2->vert->prev->prev->prev; // third ""
    vnew_3 = g2->vert->prev->prev; // second ""
    vnew_4 = g2->vert->prev; // last ""
    enew_1 = find_edge(vnew_1, vnew_2);
    enew_2 = find_edge(vnew_2, vnew_3);
    enew_3 = find_edge(vnew_3, vnew_4);
    enew_4 = find_edge(vnew_4, vnew_1);
    // // Add an edge to H1 with ends v1, v3 forming a triangulation T1
    e = create_edge_between(e1->v, e3->v, e1, e3);
    find_coloring(g1); // check C1 or C2
    if (e1->color != e2->color && e1->color == e4->color && e2->color == e3->color) {
        c_i = 1;
    } 
	else if (e1->color != e2->color && e1->color == e3->color && e2->color == e4->color) {
        c_i = 2;
    } 
	else {
        printf("ERROR: (scs4) c_i neither 1 or 2 \n");
        return;
    }
    remove_from_edge_list(e);
    remove_from_edge_list(e->rev);
    rib(e1->rev,e->color);
    if (c_i == 1 && e1->color == e2->color && e2->color == e3->color && e3->color == e4->color) {
        c_j = 0;
    } 
	else if (c_i == 1 && e1->color == e3->color) {
        c_j = 2;
    } 
	else if (c_i == 2 && e1->color != e2->color) {
        c_j = 1;
    } 
	else if (c_i == 2 && e1->color == e2->color){
        c_j = 3;
    } 
	else {
        printf("ERROR: (scs4) Can't determine C_i and C_j\n");
        return;
    }
    if (c_i == 1 && c_j == 0) { // case 1: G1 is C0
        // Merge vertex 2 and 4 in G2
        e = enew_4->next;
        for (i = 0; i < vnew_4->deg - 2; i++) {
            e->v = vnew_2;
            e = e->next;
        }
        i = vnew_4->deg;
        vnew_2->deg += i-2;
        remove_from_edge_list(enew_4);
        remove_from_edge_list(enew_4->rev);
        remove_from_edge_list(enew_3);
        remove_from_edge_list(enew_3->rev);
        enew_1->rev->next = enew_4->next;
        enew_4->next->prev = enew_1->rev;
        enew_2->prev = enew_3->rev->prev;
        enew_3->rev->prev->next = enew_2;
        remove_from_vertex_list(g2, vnew_4);
        // color graph
        find_coloring(g2);
        // Separate vertex 2 and 4 in G2
        add_to_vertex_list(g2, vnew_4);
        enew_3->rev->prev->next = enew_3;
        enew_2->prev = enew_1->rev;
        enew_4->next->prev = enew_4;
        enew_1->rev->next = enew_2;
        reinsert_to_edge_list(enew_3->rev);
        reinsert_to_edge_list(enew_3);
        reinsert_to_edge_list(enew_4->rev);
        reinsert_to_edge_list(enew_4);
        vnew_2->deg -= i-2;
        e = enew_4->next;
        for (i = 0; i < vnew_4->deg - 2; i++) {
            e->v = vnew_4;
            e = e->next;
        }
        enew_4->color = enew_1->color;
        enew_4->rev->color = enew_1->color;
        enew_3->color = enew_2->color;
        enew_3->rev->color = enew_2->color;
        // G2 may be C0 or C1, but G1 is fixed C0
        if (enew_1->color != enew_2->color) { // G2 is C1
            rib(e1->rev, enew_1->color^enew_2->color); // Make G1 C1
            perm[enew_1->color]=e1->color;
            perm[enew_2->color]=e2->color;
            perm[enew_1->color^enew_2->color] = e1->color^e2->color;
            // if both G1 G2 are C0, no need to do any recoloring
            color_change(g2, perm);
        } 
		else if (enew_1->color != e1->color){ // both c0 but colored different
            perm[enew_1->color]=e1->color;
            perm[e1->color] = enew_1->color;
            perm[e1->color^enew_1->color] = e1->color^enew_1->color;
            color_change(g2, perm);
        }
    } 
	else if ((c_i == 1 && c_j == 2) | (c_i == 2 && c_j == 1)) { // case 2
        // Add edge between 1 and 3
        e = create_edge_between(vnew_1, vnew_3, find_edge(vnew_1, vnew_4), find_edge(vnew_3,vnew_2));
        find_coloring(g2);
        remove_from_edge_list(e);
        remove_from_edge_list(e->rev);
        // convert G1 to whatever G2 is. if same then no ribs needed
        if (c_j == 1 && enew_1->color != enew_4->color) { // G1 is C1 but G2 is C2
            rib(e1->rev,e1->color^e2->color);
        } 
		else if (c_j == 2 && enew_1->color == enew_4->color) { // G1 is C2 but G2 is C1
            rib(e1->rev,e1->color^e2->color);
        }
        perm[enew_1->color]=e1->color;
        perm[enew_2->color]=e2->color;
        perm[enew_1->color^enew_2->color] = e1->color^e2->color;
        color_change(g2, perm);
    } 
	else if (c_i == 2 && c_j == 3) { // case 3
        // Add an edge between 2 and 4
        e = create_edge_between(vnew_2, vnew_4, find_edge(vnew_2, vnew_1), find_edge(vnew_4,vnew_3));
        find_coloring(g2);
        // printf("colored g2\n");
        remove_from_edge_list(e);
        remove_from_edge_list(e->rev);
        if (enew_1->color != enew_2->color) { // G1 is C3 but G2 is C2
            rib(e1->rev,e1->color^e4->color); // Make G2 into C2
        }
        perm[enew_1->color]=e1->color;
        perm[enew_4->color]=e4->color;
        perm[enew_1->color^enew_4->color] = e1->color^e4->color;
        color_change(g2, perm);
    } 
	else {
        debug("ERROR: (scs_4) not able to recolor\n");
        return;
    }
    // Join G1 and G2 together again
    enew_1->v->e = enew_1; // when deleting edges v->e was reassigned
    enew_2->v->e = enew_2;
    enew_3->v->e = enew_3;
    enew_4->v->e = enew_4;
    e1->v->e = e1;
    e2->v->e = e2;
    e3->v->e = e3;
    e4->v->e = e4;

    fuse_vertex(e1->v, vnew_1);
    fuse_vertex(e2->v, vnew_2);
    fuse_vertex(e3->v, vnew_3);
    fuse_vertex(e4->v, vnew_4);
    remove_from_vertex_list(g2,vnew_1);
    remove_from_vertex_list(g2,vnew_2);
    remove_from_vertex_list(g2,vnew_3);
    remove_from_vertex_list(g2,vnew_4);
    // // // combine the vertex lists
    v_temp = g1->vert->next;
    g1->vert->next = g2->vert->next;
    g2->vert->next->prev = g1->vert;
    g2->vert->next = v_temp;
    v_temp->prev = g2->vert;
    g1->n += g2->n;
}

//debug
void test_scs4(struct graph* g) {
    struct vertex* v1 = g->vert;
    struct vertex* v2 = v1->next;
    struct vertex* v3 = v2->next;
    struct vertex* v4 = v3->next;
    v1->e = find_edge(v1,v2);
    v2->e = find_edge(v2,v3);
    v3->e = find_edge(v3,v4);
    v4->e = find_edge(v4,v1);
    // printf("%d %d %d %d\n", v1->e->rev->v->num, v2->e->rev->v->num, v3->e->rev->v->num, v4->e->rev->v->num);
    scs_4(g,v1->e, v2->e, v3->e, v4->e);
}

int main(){
	struct graph* g;
	char data[256];
	
	read_conf("configurations.txt");
	
/*	g = read_graph("data5.txt");
    test_scs4(g);
*/	
/*
	printf("Enter file name including extension: \n");
//	scanf("%s", data);
*/	

/*
	g = read_graph("data_cycle_3.txt");
	struct vertex *v1 = g->vert;
	struct edge** short_circuit = find_short_circuit(v1);
	
	if(short_circuit == NULL) debug("No short circuit found\n");
	else{
		int size = 3;
		if(short_circuit[3] != NULL) size++;
		if(short_circuit[4] != NULL) size++;
		int i;
		for(i = 0; i < size; i++){
			debug("Edge %d %d\n",short_circuit[i]->v->num, short_circuit[i]->rev->v->num);
		}
	}
*/	
	g = read_graph("test1.txt");
	find_coloring(g);
	
/*	int cfg = find_configuration(v1);
	printf("Found config %d\n",cfg);
	int i;
	for(i = 1; i <= conf[cfg].n; i++){
		printf("map[%d] = %d\n",i, map[i]->num);
	}
*/
//	find_coloring(g);
//	print_graph(g);
//	color_vertices(g);
//	free_graph(g);
//	printf("Leaking memory: %d\n",memory_in_use);
	scanf("%*c");
	return 0;
}
