/* planar_graph_coloring.c
	This is a program to 4-color planar graphs. It is based on th paper
	'Four-Colour Theorem' by N. Robertson, D. P. Sanders, P. D. Seymour and R.
	Thomas. It was developed by Pedro Sacramento and Haidar Jamal under the
	guidance of Dr. Robin Thomas as a part of Georgia Institute of Technology's
	Matematics Department Summer REU 2019.
	Version 1.1, August 21th, 2019 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// number of different rules we have to apply when finding vertices with positive N_{P}(v)
// the number 67 differs from the paper's 32 because the paper groups together simmetric rules
#define NUM_RULES      67
// maximum size of the configurations described in the paper
#define MAX_CONF_SIZE  26
// number of distinct configuration described in the paper
#define NUM_CONF      633
// maximum size of the "name" assigned to configurations in unavoidable.conf
// currently these names serve no purpouse in the code
#define MAX_NAME_SIZE  20

#define debug printf

// used to label the ring colorings visited when using ribs to adjust configurations, assumes the size of the graph is less than INT_MAX
int cur_dfs = 1;
// array representing each of the possible colorings of rings of size up to 14, use to adjust configurations				
int ring_col_visited[133000];
// global variable used to indicate that the program failed somehow
bool ERROR = false;
// global variable used in the debug function output_graph
bool outputted = false;
// global variable used to give different labels to new vertices created throughout the code
int num_label = 1;
// used to brute force colorings for small graphs
int color[100], color_res[100];
// array with the number of different vertices considered in each rule
int rule_size[] = { 2,  2,  3,  3,  4,  4,  5,  5,  5,  5,  6,  6,
	6,  6,  7,  7,  4,  4,  5,  5,  6,  6,  6,  7,  7,  8,  8,  8,
	6,  6,  8,  8,  8,  8,  9,  9,  8,  8, 10, 10,  8,  8,  8,  8,
	8,  8,  6,  6,  9,  9,  9,  9,  9,  9, 10, 10,  7,  7,  8,  8,
	8,  8, 10, 10, 10, 10, 10};
// array with the different vertices used in each rule
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
// array with the minimum degree each vertex needs to have to obey each rule
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
// array with the minimum degree each vertex needs to have to obey each rule
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
// auxiliary array for the 
int dp[][8] = {
	{    1,     0,     0,     0,     0,     0,     0,     0},
	{    0,     1,     1,     0,     1,     0,     0,     0},
	{    3,     0,     0,     2,     0,     2,     2,     0},
	{    0,     7,     7,     0,     7,     0,     0,     6},
	{   21,     0,     0,    20,     0,    20,    20,     0},
	{    0,    61,    61,     0,    61,     0,     0,    60},
	{  183,     0,     0,   182,     0,   182,   182,     0},
	{    0,   547,   547,     0,   547,     0,     0,   546},
	{ 1641,     0,     0,  1640,     0,  1640,  1640,     0},
	{    0,  4921,  4921,     0,  4921,     0,     0,  4920},
	{14763,     0,     0, 14762,     0, 14762, 14762,     0},
	{    0, 44287, 44287,     0, 44287,     0,     0, 44286}
};

struct edge {
	struct vertex* v;		// vertex the edge is connected to
	struct edge* next;		// next edge clockwise around v
	struct edge* prev;		// previous edge clockwise around v
	struct edge* rev;		// reverse edge
	int color;				// color, starts at 0 and becomes 1, 2 or 3 once colored
};

struct vertex {
	int num;				// unique identifier for the vertex
	int deg;				// degree of the vertex in the graph
	int weight;				// weight/charge of the vertex, used to find hubs
	bool visited;			// auxiliary variable used in graph searches
	struct edge* e;			// pointer to the first edge
	struct vertex *next;	// next element in vertex linked list
	struct vertex *prev;	// previous elemnt in vertex linked list
};

struct graph {
	char name[100];
	int n;					// number of vertices
	struct vertex* vert;	// pointer to the first vertex
};

struct configuration {
	char N[MAX_NAME_SIZE];			// character string identifying the configuration;
	int n;							// n is the number of vertices of the free completion;
	int r;							// r is the ring-size;
	int a;							// a is the cardinality of C ; and
	int b;							// b is the cardinality of C' (see discussion before (3.2) in the paper);
	int x;							// |X|
	struct edge* X[4];				// set X as described in the paper
	struct graph* g;				// graph representing the configuration
	int *ring_colorings;			// array containing possible ring colorings of this configuration, mathced with conf_colorings
	int *conf_colorings;			// array containing possible configuration colorings of this configuration
} conf[NUM_CONF + 1];

// read functions
void read_conf(char*);
struct graph* read_graph(char*);

// utility functions, they perform different types of manipulations over the graph
void remove_from_edge_list(struct edge*);
void reinsert_to_edge_list(struct edge*);
void fuse_vertex(struct vertex*, struct vertex*);
void contract_edge(struct graph*, struct edge*);
void decontract_edge(struct graph*, struct edge*);
struct edge* find_edge(struct vertex*, struct vertex*);
void add_edge(struct vertex*, struct vertex*);
void add_to_vertex_list(struct graph*, struct vertex*);
void remove_from_vertex_list(struct graph*, struct vertex*);
struct edge* create_edge_between(struct vertex*, struct vertex*, struct edge*, struct edge*);
void set_visited_false(struct graph*);
void graph_separating_dfs(struct vertex*, struct graph*, struct graph*);
void free_graph(struct graph*);
struct graph* split_graph(struct graph*, int, struct edge**);

// coloring functions
void find_coloring(struct graph*);
void parallel_edges_coloring(struct graph*, struct edge*, struct edge*);
void base_case_coloring(struct graph*);
void degree_three_elimination(struct graph*, struct vertex*);
void scs_3(struct graph*, struct edge*, struct edge*, struct edge*);
void scs_4(struct graph*, struct edge*, struct edge*, struct edge*, struct edge*);
void scs_5(struct graph*, struct edge*, struct edge*, struct edge*, struct edge*, struct edge*);
void reduce_and_color_conf(struct graph*, int, struct vertex**);
void color_dfs(struct vertex*, int*);
void color_vertices(struct graph* g);
void conf_brute_force_coloring(int);

// auxiliary coloring functions, they perform intermediate steps for the coloring functions
struct vertex* find_hub(struct graph*);
void apply_rules(struct edge*);
void find_ring_colorings(int);
void color_change(struct graph*, int*);
struct edge** find_short_circuit(struct vertex*);
int find_configuration(struct vertex*, struct vertex**);
struct vertex** find_nth_configuration(struct vertex*, int, struct vertex**);
bool iso_dfs(int, struct vertex*, struct vertex*, struct vertex**, bool);
struct edge** delete_conf(struct graph*, int, struct vertex**);
void reinsert_conf(struct graph*, int, struct vertex**, struct edge**);
void rib(struct edge*, int);
void rib_conf(struct edge*, int);
void rotate_circuit(struct edge**, int);
int find_scs5_coloring_case(struct edge**);
bool* find_scs5_coloring_pattern(struct edge**);
void find_scs5_coloring_pattern_dfs(struct edge**, bool*);
bool change_scs5_circuit_dfs(struct edge**, int, bool*);
int find_index_of_coloring(struct edge**, int);
int search_conf_colorings(int*, int, int, int);
int change_circuit_dfs(struct edge**, int, int);

// debug functions
void check_graph(struct graph*);
void output_graph(struct graph*);
void print_dfs(struct vertex*);
void print_graph(struct graph*);

// reads list of configurations from text file according to the structure defined above
// http://people.math.gatech.edu/~thomas/FC/ftpinfo.html for more info
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

		conf[i].g = (struct graph*) malloc(sizeof(struct graph));

		if( conf[i].g == NULL ){
			printf("read_conf: Memory error: not enough space for new graph in function read_graph\n");
			ERROR = true;
			return;
		}

		conf[i].ring_colorings = NULL;
		conf[i].conf_colorings = NULL;
		conf[i].g->vert = NULL;
		conf[i].g->n = 0;

		// create the vertices
		for(j = 1; j <= conf[i].n; j++){
			v1 = (struct vertex*) malloc(sizeof(struct vertex));
			if(v1 == NULL){
				printf("read_conf: Memory error: not enough space for new vertex in function read_graph\n");
				ERROR = true;
			return;
		}
		v1->num = j;
		add_to_vertex_list(conf[i].g, v1);
		v1->e = NULL;
		}
		v1 = conf[i].g->vert;
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
				if(u <= 0 && u > conf[i].g->n){
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
			v1 = conf[i].g->vert;
			while(v1->num != X_edges[2*j]){
				v1 = v1->next;
			}
			e = v1->e;
			while(e->rev->v->num != X_edges[2*j+1]) {
				e = e->next;
			}
			conf[i].X[j] = e;
		}
		for (j = 0; j < conf[i].g->n; j++) {
			fscanf(ptr,"%d",&skip);
		}
	}
	fclose(ptr);
}

// splits the graph into two graphs, one containing the inside and another containing the outside
// of the given circuit, size refers to the size of the circuit, returns the other graph
struct graph* split_graph(struct graph* g1, int size, struct edge* circuit[5]) {
	int i;
	struct graph *g2;
	struct edge *e;
	struct vertex *vnew_list[5];
	struct vertex * v_interior; // used in graph separating dfs
	
	// create copy of circuit
	for (i = 0; i < size; i++) {
		vnew_list[i] = (struct vertex*) malloc(sizeof(struct vertex));
		if( vnew_list[i] == NULL ){
			printf("Memory error: not enough space for vertex in split graph\n");
			ERROR = true;
			return;
		}
		vnew_list[i]->num = circuit[i]->v->num;
		vnew_list[i]->e = NULL;
		vnew_list[i]->deg = 0;
	}
	// add edges between vertices
	for (i = 0; i < size; i++) {
		add_edge(vnew_list[i], vnew_list[(i+1)%size]);
		vnew_list[i]->deg++;
		add_edge(vnew_list[i], vnew_list[((i-1)+size)%size]);
		vnew_list[i]->deg++;
	}
	// enforce ordering on vertices to have clockwise (same as input edges in scs)
	for (i = 0; i < size; i++) {
		vnew_list[i]->e = find_edge(vnew_list[i], vnew_list[(i+1)%size]);
		circuit[i]->v->e = circuit[i];
	}
	// create new graph g2
	g2 = (struct graph*) malloc(sizeof(struct graph));
	if( g2 == NULL ){
		printf("Memory error: not enough space for graph in split graph\n");
		ERROR = true;
		return;
	}
	g2->n = 0;
	g2->vert = NULL;
	// call graph separating dfs (alters the vertex list of g1 and g2)
	set_visited_false(g1);
	for (i = 0; i < size; i++) circuit[i]->v->visited = true;
	for (i = 0; i < size; i++){
		e = circuit[i]->next;
		while (e!= circuit[(i-1+size)%size]->rev) {
			if (!e->rev->v->visited) {
				graph_separating_dfs(e->rev->v,g1,g2);
			}
			e = e->next;
		}
	}
	for (i = 0; i < size; i++)  add_to_vertex_list(g2,vnew_list[i]);
	// remap the internal edges into g2
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
	// remap the internal edges out of g1
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
void scs_3(struct graph* g1, struct edge* e1, struct edge* e2, struct edge* e3){
	struct graph *g2;
	struct edge *e, *edge_list[3], *enew_1, *enew_2, *enew_3;
	struct vertex *vnew_1, *vnew_2, *vnew_3, *v_temp;
	int perm[4];

	edge_list[0] = e1;
	edge_list[1] = e2;
	edge_list[2] = e3;
	g2 = split_graph(g1, 3, edge_list);
	vnew_1 = g2->vert->prev->prev->prev;
	vnew_2 = g2->vert->prev->prev;
	vnew_3 = g2->vert->prev;
	// color the two divided graph
	find_coloring(g1);
	if(ERROR) return;
	find_coloring(g2);
	if(ERROR) return;
	enew_1 = find_edge(vnew_1, vnew_2);
	enew_2 = find_edge(vnew_2, vnew_3);
	enew_3 = find_edge(vnew_3, vnew_1);
	// match the colorings
	perm[enew_1->color] = e1->color;
	perm[enew_2->color] = e2->color;
	perm[enew_3->color] = e3->color;
	color_change(g2, perm);
	// fuse the graphs back together
	e1->v->e = e1;
	e2->v->e = e2;
	e3->v->e = e3;
    vnew_1->e = enew_1;		// when finding the coloring v->e might have been reassigned
    vnew_2->e = enew_2;
    vnew_3->e = enew_3;
	fuse_vertex(e1->v, vnew_1);
	fuse_vertex(e2->v, vnew_2);
	fuse_vertex(e3->v, vnew_3);
	// change the vertex list of g1
	remove_from_vertex_list(g2, vnew_1);
	remove_from_vertex_list(g2, vnew_2);
	remove_from_vertex_list(g2, vnew_3);
	// combine the vertex lists
	v_temp = g1->vert->next;
	g1->vert->next = g2->vert->next;
	g2->vert->next->prev = g1->vert;
	g2->vert->next = v_temp;
	v_temp->prev = g2->vert;
	g1->n += g2->n;
	// free allocated memory
	free(vnew_1);
	free(vnew_2);
	free(vnew_3);
	free(g2);
}

// deletes config in graph edge by edge
// returns list of edges deleted in order
struct edge** delete_conf(struct graph* g, int conf_num, struct vertex** map) {
	int i, j, deg;
	int r = conf[conf_num].r;
	int n = conf[conf_num].n;
	int k = 0;
	struct edge** edge_deleted = (struct edge**) malloc(sizeof(struct edge)*(3*n-2*r-3)*2);
	struct vertex* v;
	struct edge* e;

	if( edge_deleted == NULL ){
		printf("Memory error: not enough space for deleted edges in function delete_conf\n");
		ERROR = true;
		return NULL;
	}

	for (i = r + 1; i <= n; i++) {
		v = map[i];
		deg = v->deg;
		for (j = 0; j < deg; j++) {
			e = v->e;
			remove_from_edge_list(e);
			remove_from_edge_list(e->rev);
			edge_deleted[k++] = e;
			edge_deleted[k++] = e->rev;
		}
		remove_from_vertex_list(g, v);
	}
	return edge_deleted;
}

// reinserts deleted edges of configuration back into the graph
void reinsert_conf(struct graph* g, int conf_num, struct vertex** map, struct edge** edge_deleted) {
	struct edge* e;
	int i;
	int r = conf[conf_num].r;
	int n = conf[conf_num].n;
	// by Euler's formula
	for (i = (3*n-2*r-3)*2; i > 0 ; i--) {
		reinsert_to_edge_list(edge_deleted[i-1]);
	}
	for (i = r + 1; i <= n; i++) {
		add_to_vertex_list(g, map[i]);
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
	if(e->v->e == NULL) e->v->e = e;
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

	// reinsert edges: e->rev->next->rev, e->rev->next, e->next->rev, e->next, e->rev, e
	reinsert_to_edge_list(e->rev->next->rev);
	reinsert_to_edge_list(e->rev->next);
	reinsert_to_edge_list(e->next->rev);
	reinsert_to_edge_list(e->next);
	reinsert_to_edge_list(e->rev);
	reinsert_to_edge_list(e);
	// reconnect edges to v1 and v2
	for (i = 0; i < v1->deg; i++) {
		e->v=v1;
		e = e->next;
	}
	e = e->rev;

	for (i = 0; i < v2->deg; i++) {
		e->v=v2;
		e = e->next;
	}
	e->next->color = e->rev->prev->color;
	e->next->rev->color = e->rev->prev->color;
	e->rev->next->color = e->prev->color;
	e->rev->next->rev->color = e->prev->color;
	// bookkeeping
	remove_from_vertex_list(g,v_old);
	add_to_vertex_list(g,v1);
	add_to_vertex_list(g,v2);
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

// converts the edge coloring to a vertex coloring and outputs it
void color_vertices(struct graph* g){
	int i;
	int* colored = (int*) malloc(sizeof(int) * (g->n + 1));
	for(i = 0; i <= g->n; i++) colored[i] = -1;
	colored[g->vert->num] = 0;
	color_dfs(g->vert, colored);
	printf("Coloring:\n");
	for(i = 1; i <= g->n; i++) printf("Vertex %d, color %d\n", i, colored[i]);
	free(colored);
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
// input is the name of the file containing the graph
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
	// reads graph size
	fscanf(ptr,"%d",&n);
	for(i = 1; i <= n; i++){
		// creates vertices one by one
		v1 = (struct vertex*) malloc(sizeof(struct vertex));
		if(v1 == NULL){
			printf("Memory error: not enough space for new vertex in function read_graph\n");
			ERROR = true;
			return NULL;
		}
		v1->num = num_label;
        v1->deg = 0;
		num_label++;
		add_to_vertex_list(g, v1);
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		v1->e = NULL;
		// reads all the degrees
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
			// reads all the edges
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

	// for more info on the specifics of how this works refer to the paper 'Discharging Cartwheels'
	// by the same authors. The start of page 8 specifically describes the numbering of the vertices
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
	printf("ERROR: find_hub couldn't find a vertex with weight positive\n");
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
		// just a single edge
		g->vert->e->color = 1;
		g->vert->e->rev->color = 1;
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
	// if g->n == 1 there are no edges
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
		e = e->next;
	}
}

// changes the colors in a graph
void color_change(struct graph* g, int perm[4]){
	struct vertex* v = g->vert;
	struct edge* e;
	int i, j;
	for(i = 0; i < g->n; i++){
		e = v->e;
		e->color = perm[ e->color ];
		e = e->next;
		while(e != v->e){
			e->color = perm[ e->color ];
			e = e->next;
		}
//		for(j = 0; j < v->deg; j++){
//			e->color = perm[ e->color ];
//			e = e->next;
//		}
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

	// create 2 new vertices, copies of the vertices of the parallel edges

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
	// separates the graphs in the inside and outside of the circuit formed by the parallel edges
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
	if(ERROR) return;
	find_coloring(g2);
	if(ERROR) return;

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
	
	while(g2->n > 0){
		v = g2->vert;
		remove_from_vertex_list(g2, v);
		add_to_vertex_list(g1, v);
	}
	free(v1_copy);
	free(v2_copy);
	free(g2);
}

// removes an edge e from the list of edges of e->v
// does NOTHING to e->rev
void remove_from_edge_list(struct edge* e){
	if(e == NULL){
		printf("Trying to remove a NULL edge in remove_from_edge_list\n");
		ERROR = true;
		return;
	}
	if(e->v->deg == 1){
		e->v->e = NULL;
		e->v->deg--;
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
	struct edge *e1 = v1->e;
	struct edge *e1_end = e1->next;
	struct edge *e2 = v2->e->next;
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

// dfs to check subgraph isomorphism between the graph and a configuration
bool iso_dfs(int r, struct vertex* vg, struct vertex* vc, struct vertex** map, bool clockwise){
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
		if( ! ec->rev->v->visited ){
			ec->rev->v->e = ec->rev;
			eg->rev->v->e = eg->rev;
			if( ! iso_dfs(r, eg->rev->v, ec->rev->v, map, clockwise) ) return false;
		}
		eg = eg->next;
		ec = clockwise ? ec->next : ec->prev;
	}
	return true;
}

// given a hub finds a configuration in its cartwheel
struct vertex** find_nth_configuration(struct vertex* hub, int conf_num, struct vertex** map){
	int i, j;
	int n = conf[conf_num].n, r = conf[conf_num].r;
	struct edge* e1;
	struct edge* e2;
	struct vertex* v;

	// if the hub is inside the configuration
	v = conf[conf_num].g->vert->prev;
	for(i = n; i > r; i--){
		if(hub->deg == v->deg){	// good to go
			e1 = hub->e;
			e2 = v->e;
			for(j = 0; j < hub->deg; j++){
				hub->e = e1;
				v->e = e2;
				set_visited_false(conf[conf_num].g);
				if( iso_dfs(r, hub, v, map, true) ) return map;
				set_visited_false(conf[conf_num].g);
				if( iso_dfs(r, hub, v, map, false) ) return map;
				e1 = e1->next;
			}
		}
		v = v->prev;
	}
	// if the hub is in the ring of the configuration
	for( ; i > 0; i--){
		e1 = v->e;
		while(e1->rev->v->num != (v->num % r) + 1) e1 = e1->next;
		e1 = e1->next;
		e2 = hub->e;
		for(j = 0; j < hub->deg; j++){
			if(e2->rev->v->deg == e1->rev->v->deg){
				e1->rev->v->e = e1->rev;
				e2->rev->v->e = e2->rev;
				set_visited_false(conf[conf_num].g);
				if( iso_dfs(r, e2->rev->v, e1->rev->v, map, true) ) return map;
				set_visited_false(conf[conf_num].g);
				if( iso_dfs(r, e2->rev->v, e1->rev->v, map, false) ) return map;
			}
			e2 = e2->next;	
		}
		v = v->prev;
	}
	return NULL;
}

// given a hub finds a configuration in its cartwheel
int find_configuration(struct vertex* hub, struct vertex** map){
	int i;
	for(i = 1; i <= NUM_CONF; i++){
		if( find_nth_configuration(hub, i, map) != NULL) return i;
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
		ERROR = true;
		printf("Memory error: insufficient memory for circuit in find_short_circuit\n");
		return NULL;
	}
	for(i = 0; i <  5; i++) circuit[i] = NULL;

	// sets visited to false
	hub->visited = false;
	e1 = hub->e;
	for(i = 0; i < hub->deg; i++){
		e2 = e1->rev;
		v1 = e2->v;
		v1->visited = false;
		e2 = e2->next->next;	// starts at the first hat and then go through the fans
		for(j = 4; j < v1->deg; j++){	// stops before the second hat
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
	// sets the hub and the spokes to visited
	// since we have no parallel edge and no loops, the hubs and spokes are all different
	e1 = hub->e;
	hub->visited = true;
	for(i = 0; i < hub->deg; i++){
	e1->rev->v->visited = true;
	e1 = e1->next;
	}
	// checks the hats and the fans to look for visited vertices(spokes)
	// finds cycles of size 3
	e1 = hub->e;
	for(i = 0; i < hub->deg && vsc == NULL; i++){
		e2 = e1->rev;
		e2 = e2->next->next;	// starts at the first hat and then go through the fans
		v1 = e2->v;
		for(j = 4; j < v1->deg && vsc == NULL; j++){	// stops before the second hat
			v2 = e2->rev->v;
			if( v2->visited ){
				vsc = v2;
				size = 3;
				circuit[2] = e1->rev;
				circuit[1] = e2->rev;
				break;
			}
			e2 = e2->next;
		}
		e1 = e1->next;
	}
	if(vsc == NULL){
		// sets the hats and the fans to visited
		// finds cylces of size 4
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
					break;
				}
				v2->visited = true;
				e2 = e2->next;
			}
			e1 = e1->next;
		}
	}
	if(vsc == NULL){
		// goes to distance 3 and looks for visited vertices (hats and fans)
		// finds cylces of size 5
		e1 = hub->e;
		for(i = 0; i < hub->deg && vsc == NULL; i++){
			e2 = e1->rev->next->next;	// starts at the first hat and then go through the fans
			v1 = e2->v;
			for(j = 4; j < v1->deg && vsc == NULL; j++){	// stops before the second hat
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
						break;
					}
					e3 = e3->next;
				}
				e2 = e2->next;
			}
			e1 = e1->next;
		}
	}
	if(vsc == NULL){
		free(circuit);
		return NULL;
	}
	// looks for the short circuit once we have a repeated vertex 
	e1 = hub->e;
	for(i = 0; i < hub->deg; i++){
		v1 = e1->rev->v;
		if(v1 == vsc){
			circuit[0] = e1;
			return circuit;
		}
		e1 = e1->next;
	}
	e1 = hub->e;
	for(i = 0; i < hub->deg; i++){
		e2 = e1->rev->next->next;	// starts at the first hat and then go through the fans
		v1 = e2->v;
		for(j = 4; j < v1->deg; j++){	// stops before the second hat
			v2 = e2->rev->v;
			if(v2 == vsc){
				circuit[0] = e1;
				circuit[1] = e2;
				return circuit;
			}
			e2 = e2->next;
		}
		e1 = e1->next;
	}
	return circuit;
}

int reduce_circuit(struct edge **circuit, int size){
	struct edge* e;
	int i;
	if(size == 5){
		for(i = 0; i < 5; i++){
			e = find_edge(circuit[i]->v, circuit[(i+2)%5]->v);
			if(e != NULL){
				if(circuit[i]->next != e && circuit[i]->prev != e){		// triangle has a vertex inside
					if(i != 4){
						circuit[0] = circuit[i];
						circuit[1] = circuit[i+1];
						circuit[2] = e->rev;
					}
					else{
						circuit[1] = e->rev;
						circuit[2] = circuit[4];
					}
					size = 3;
				}
				else{		// quadrilateral has a vertex inside
					if(i < 4){			// check if the circuit has a vertex inside
						circuit[i] = e;
						for(i++; i < 4; i++) circuit[i] = circuit[i+1];
					}
					else circuit[0] = e;
					size = 4;
				}
				break;
			}
		}
	}
	if(size == 4){
		e = find_edge(circuit[0]->v, circuit[2]->v);
		if(e != NULL){
			if(e->next != circuit[0] && e->prev != circuit[0]) circuit[2] = e->rev;
			else{
				circuit[0] = circuit[3];
				circuit[1] = e;
			}
			size = 3;
		}
		e = find_edge(circuit[1]->v, circuit[3]->v);
		if(e != NULL){
			if(e->next != circuit[1] && e->prev != circuit[1]) circuit[0] = e->rev;
			else{
				circuit[2] = circuit[3];
				circuit[1] = e;
			}
			size = 3;
		}
	}
	return size;
}

// main coloring function
void find_coloring(struct graph* g){
	struct vertex* v;
	struct vertex* hub;
	struct vertex** map;
	struct edge *e1, *e2, *e3, *e4, *e5;
	struct edge** short_circuit;
	int i, j, k;
	int conf_num;
	
	// if the graph is small we are at a base case
	if(g->n <= 3){		
		base_case_coloring(g);		
		if(ERROR){
			printf("Base case coloring failed\n");
			output_graph(g);
		}
		return;
	}
	// searches for parallel edges and run SCS for size = 2 if we find any
	set_visited_false(g);
	v = g->vert;
	v = g->vert;

	for(i = 0; i < g->n; i++){
		e1 = v->e;
		for(j = 0; j < v->deg; j++){
			if(e1->rev->v->visited == true){	// found parallel edges
				e2 = v->e;
				for(k = 0; k < j; k++){
					if(e1->rev->v == e2->rev->v){
						parallel_edges_coloring(g, e1, e2->rev);
						if(ERROR){
							printf("Parallel edges failed\n");
							output_graph(g);
						}
						return;
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
		if(ERROR){
			printf("Base case coloring failed\n");
			output_graph(g);
		}
		return;
	}

	// searches for vertices of degree <= 3 and deletes them
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->deg == 3 ){
			degree_three_elimination(g, v);
			if(ERROR){
				printf("Degree three elimination failed\n");
				output_graph(g);
			}
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
			if(ERROR){
				printf("SCS4 failed\n");
				output_graph(g);
			}
			return;
		}
		v = v->next;
	}
	// no simple reduction, search for a hub
	hub = find_hub(g);
	if(ERROR){
		printf("Find hub failed\n");
		output_graph(g);
		return;
	}
	// look for a short circuit around the hub
	short_circuit = find_short_circuit(hub);
	if(ERROR){
		printf("Find short circuit failed\n");
		output_graph(g);
		return;
	}
	if(short_circuit != NULL){
		int size = 3;
		if(short_circuit[3] != NULL) size++;
		if(short_circuit[4] != NULL) size++;
		size = reduce_circuit(short_circuit, size);
		if(size == 3) scs_3(g, short_circuit[0], short_circuit[1], short_circuit[2]);
		else if(size == 4) scs_4(g, short_circuit[0], short_circuit[1], short_circuit[2], short_circuit[3]);
		else if(size == 5) scs_5(g, short_circuit[0], short_circuit[1], short_circuit[2], short_circuit[3], short_circuit[4]);
		if(ERROR){
			printf("SCS%d failed\n",size);
			output_graph(g);
		}
		return;
	}

	map = (struct vertex**) malloc((sizeof(struct vertex*)) * (MAX_CONF_SIZE + 1));
	if(map == NULL){
		printf("Not enough space for map in find_configuration\n");
		ERROR = true;
		return;
	}
	map[1] = NULL;
	conf_num = find_configuration(hub, map);
	if(ERROR){
		output_graph(g);
		return;
	}

	int n = conf[conf_num].n;
	for(i = 1; i < n; i++){
		for(j = i + 1; j <= n; j++){
			if(map[i] == map[j]){		// repeated vertex in the free completion implies short circuit
				short_circuit = find_short_circuit(map[i]);
				if(short_circuit == NULL){
					printf("Repeated vertex in conf failed\n");
					ERROR = true;
					return;
				}
				int size = 3;
				if(short_circuit[3] != NULL) size++;
				if(short_circuit[4] != NULL) size++;				
				size = reduce_circuit(short_circuit, size);
				if(size == 3) scs_3(g, short_circuit[0], short_circuit[1], short_circuit[2]);
				else if(size == 4) scs_4(g, short_circuit[0], short_circuit[1], short_circuit[2], short_circuit[3]);
				else if(size == 5) scs_5(g, short_circuit[0], short_circuit[1], short_circuit[2], short_circuit[3], short_circuit[4]);
				if(ERROR){
					printf("SCS%d failed\n",size);
					output_graph(g);
				}
                return;
			}
		}
	}
	reduce_and_color_conf(g, conf_num, map);
	if(ERROR){
		printf("Reduce and color conf failed\n");
		output_graph(g);
	}
	free(short_circuit);
	free(map);
	return;
}

// if the program fails, we renumber the edges from 1 to g->n and output it so it can be used as input
void output_graph(struct graph* g) {
	struct edge* e;
	struct vertex* v = g->vert;
	int i, j;
	char c;

	if( outputted ) return;
	outputted = true;
	
	printf("Program failed, would you like to see the current graph (y/n)?");
	scanf(" %c",&c);
	if(c != 'y') return;

	// renumber vertices
	for(i = 1; i <= g->n; i++){
		v->num = i;
		v = v->next;
	}
	// output graph
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

// checks if the graph is valid
void check_graph(struct graph* g){
	struct vertex *v, *v2;
	struct edge *e;
	struct edge *e2;
	int i,j;

	// basic structural integrity
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->num <= 0 ){
			printf("Invalid vertex index, must be positive\n");
			ERROR = true;
			return;
		}
		if( v->next->prev != v ){
			printf("Vertex list invalid, v->next->prev != v for v->num == %d\n",v->num);
			ERROR = true;
			return;
		}
		if( v->prev->next != v ){
			printf("Vertex list invalid, v->prev->next != v for v->num)== %d\n",v->num);
			ERROR = true;
			return;
		}
		if(i > 0 && v == g->vert){
			printf("Repeated vertex in vertex list\n");
			ERROR = true;
			return;
		}
		e = v->e;
		for(j = 0; j < v->deg; j++){
			if(e->v != v){
				printf("Invalid edge, e->v != v for vertex %d\n",v->num);
				ERROR = true;
				return;
			}
			if(e->rev->rev != e){
				printf("Invalid edge pair, e->rev->rev != e for e->v->num == %d, e->rev->v->num == %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
				return;
			}
			if(e->rev->v == v){
				printf("Self loop at vertex %d\n",v->num);
				ERROR = true;
				return;
			}
			if(e->next->prev != e){
				printf("Invalid edge pair, e->next->prev != e for e->v->num == %d, e->rev->v->num = %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
				return;
			}
			if(e->prev->next != e){
				printf("Invalid edge pair, e->prev->next != e for e->v->num == %d, e->rev->v->num = %d\n",e->v->num ,e->rev->v->num);
				ERROR = true;
				return;
			}
			if(j > 0 && e == v->e){
				printf("Invalid edge list, repeated edge in the edge list of %d\n",v->num);
				ERROR = true;
				return;
			}
			e = e->next;
		}
		if(e != v->e){
			printf("Degree does not match size of edge list for vertex %d\n",v->num);
			ERROR = true;
			return;
		}
		v2 = g->vert;
		for(j = 0; j < i; j++){
			if(v->num == v2->num){
				printf("Two different vertices have the same label: %d\n",v->num);
				ERROR = true;
				return;
			}
			v2 = v2->next;
		}
		v = v->next;
	}
	if( v != g->vert ){
		printf("Number of vertices does not match size of vertex list\n");
		ERROR = true;
		return;
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
				return;
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
					return;
				}
				else if(e->color == 0){
					ERROR = true;
					printf("Error: graph only partially colored\n");
					return;
				}
				if(e->color != e->rev->color){
					ERROR = true;
					printf("Error: edge %d %d and the reverse have different colors\n",e->v->num, e->rev->v->num);
					return;
				}
				if(e->color == e->next->color){
					ERROR = true;
					printf("Error: invalid coloring at face %d %d %d\n",e->v->num, e->next->rev->v->num, e->rev->v->num);
					return;
				}
			}
			else if(e->color != 0){
				ERROR = true;
				printf("Error: graph only partially colored\n");
				return;
			}
			e = e->next;
		}
		v = v->next;
	}
	if(ERROR) output_graph(g);
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

// rotates a circuit of size 5
void rotate_circuit(struct edge** circuit, int a){
	struct edge* e = circuit[0];
	circuit[0] = circuit[a%5];
	circuit[a%5] = circuit[(2*a)%5];
	circuit[(2*a)%5] = circuit[(3*a)%5];
	circuit[(3*a)%5] = circuit[(4*a)%5];
	circuit[(4*a)%5] = e;
}

// given a coloring of the edge return which case it is
int find_scs5_coloring_case(struct edge** circuit){
	int sum, color;
	
	sum = circuit[0]->color;
	sum += circuit[1]->color;
	sum += circuit[2]->color;
	sum += circuit[3]->color;
	sum += circuit[4]->color;
	color = (sum - 6) / 2; 	// the most frequent color
	if(circuit[0]->color != color){
		if(circuit[1]->color != color) return 0;
		if(circuit[2]->color != color) return 1;
		if(circuit[3]->color != color) return 2;
		if(circuit[4]->color != color) return 3;
	}
	else if(circuit[1]->color != color){
		if(circuit[2]->color != color) return 4;
		if(circuit[3]->color != color) return 5;
		if(circuit[4]->color != color) return 6;
	}
	else if(circuit[2]->color != color){
		if(circuit[3]->color != color) return 7;
		if(circuit[4]->color != color) return 8;
	}
	else if(circuit[3]->color != color){
		if(circuit[4]->color != color) return 9;
	}
	ERROR = true;
	printf("Couldn't find coloring case for short circuit of size 5\n");
	return -1;
}

// dfs to apply 6.2 to a circuit of size 5
void find_scs5_coloring_pattern_dfs(struct edge** circuit, bool* visited){
	int i, color_case;
	int other_color;
	bool outside;
	
	outside = (circuit[0]->prev->rev == circuit[4] && circuit[1]->prev->rev == circuit[0]);

	for(i = 0; i < 5; i++){
		other_color = (circuit[i]->color % 3) + 1;
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
		color_case = find_scs5_coloring_case(circuit);
		if( ! visited[color_case] ){
			visited[color_case] = true;
			find_scs5_coloring_pattern_dfs(circuit, visited);
		}
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
		//
		other_color = (other_color % 3) + 1;
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
		color_case = find_scs5_coloring_case(circuit);
		if( ! visited[color_case] ){
			visited[color_case] = true;
			find_scs5_coloring_pattern_dfs(circuit, visited);
		}
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
	}
}

// Applies 6.2 to find the possible colorings of a circuit of size 5
// Rotates the circuits so we only get cases C1, D1, and E respectively 1, 2 and 3
bool* find_scs5_coloring_pattern(struct edge** circuit){
	bool *visited = malloc(sizeof(bool) * 10);
	int i;

	for(i = 0; i < 10; i++) visited[i] = false;
	visited[find_scs5_coloring_case(circuit)] = true;
	find_scs5_coloring_pattern_dfs(circuit, visited);
	return visited;

}

// a dfs using rib changes to search for a specific coloring case of a circuit of size 5
bool change_scs5_circuit_dfs(struct edge** circuit, int target_case, bool* visited){
	int i, cur_case, other_color;
	bool outside = (circuit[0]->prev->rev == circuit[4] && circuit[1]->prev->rev == circuit[0]);
	
	for(i = 0; i < 5; i++){
		other_color = (circuit[i]->color % 3) + 1;
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
		cur_case = find_scs5_coloring_case(circuit);
		if( cur_case == target_case ) return true;
		if( ! visited[cur_case] ){
			visited[cur_case] = true;
			if( change_scs5_circuit_dfs(circuit, target_case, visited) ) return true;
		}
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
		other_color = (other_color % 3) + 1;
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
		cur_case = find_scs5_coloring_case(circuit);
		if( cur_case == target_case ) return true;
		if( ! visited[cur_case] ){
			visited[cur_case] = true;
			if( change_scs5_circuit_dfs(circuit, target_case, visited) ) return true;
		}
		if(outside) rib(circuit[i], other_color);
		else rib(circuit[i]->rev, other_color); 
	}
	return false;
}

// adjusts a circuit of size 5 to a specific circuit coloring case
void change_scs5_circuit(struct edge** circuit, int target_case){
	bool coloring_visited[10];
	int i, cur_case;

	cur_case = find_scs5_coloring_case(circuit);
	if( cur_case == target_case ) return;
	for(i = 0; i < 10; i++) coloring_visited[i] = false;
	coloring_visited[cur_case] = true;
	if( ! change_scs5_circuit_dfs(circuit, target_case, coloring_visited) ){
		ERROR = true;
		printf("Couldn't find the correct coloring for the circuit in scs5\n");
	}
}

// circuit size four, contains vertex inside circuit, edges in cw order
void scs_4(struct graph *g1, struct edge *e1, struct edge *e2, struct edge *e3, struct edge *e4) {
    int i, j, c_i = 0, c_j = 0, inside_color, temp_deg;
    int perm[] = {0, 0, 0, 0};
    struct graph *g2;
    struct vertex *vnew_1, *vnew_2, *vnew_3, *vnew_4, *v_temp;
    struct edge *edge_list[4], *e, *enew_1, *enew_2, *enew_3, *enew_4;

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
    if(ERROR) return;
    
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
    free(e->rev);
    free(e);
    rib(e1->rev, e->color);
    inside_color = e->color;

    if (c_i == 1 && e1->color == e2->color && e2->color == e3->color && e3->color == e4->color) c_j = 0;
	else if (c_i == 1 && e1->color == e3->color) c_j = 2;
	else if (c_i == 2 && e1->color != e2->color) c_j = 1;
	else if (c_i == 2 && e1->color == e2->color) c_j = 3;
	else {
		ERROR = true;
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
        temp_deg = vnew_4->deg;
        vnew_2->deg += temp_deg - 2;
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
		if(ERROR) return;
        
        // might have changed to v4's edges in coloring
        enew_1->v->e = enew_1;		
        enew_2->v->e = enew_2;
        enew_3->v->e = enew_3;
        enew_4->v->e = enew_4;
        
        // Separate vertex 2 and 4 in G2
        add_to_vertex_list(g2, vnew_4);
        enew_2->prev = enew_1->rev;
        enew_1->rev->next = enew_2;
        reinsert_to_edge_list(enew_3->rev);
        reinsert_to_edge_list(enew_3);
        reinsert_to_edge_list(enew_4->rev);
        reinsert_to_edge_list(enew_4);
        vnew_2->deg -= temp_deg - 2;
        
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
            rib(e1->rev, inside_color); // Make G1 C1
            perm[enew_1->color] = e1->color;
            perm[enew_2->color] = e2->color;
            perm[enew_1->color^enew_2->color] = e1->color^e2->color;
            color_change(g2, perm);
        }
		else if (enew_1->color != e1->color){ // both c0 but colored different
            perm[enew_1->color] = e1->color;
            perm[e1->color] = enew_1->color;
            perm[e1->color^enew_1->color] = e1->color^enew_1->color;
            color_change(g2, perm);
        }
    }
	else if ((c_i == 1 && c_j == 2) | (c_i == 2 && c_j == 1)) { // case 2
        // Add edge between 1 and 3
        e = create_edge_between(vnew_1, vnew_3, find_edge(vnew_1, vnew_4), find_edge(vnew_3,vnew_2));
        find_coloring(g2);
		if(ERROR) return;
        remove_from_edge_list(e);
        remove_from_edge_list(e->rev);
        free(e->rev);
        free(e);
        // convert G1 to whatever G2 is. if same then no ribs needed
        if (c_j == 1 && enew_1->color != enew_4->color) { // G1 is C1 but G2 is C2
            rib(e1->rev, e1->color ^ e2->color);
        }
		else if (c_j == 2 && enew_1->color == enew_4->color) { // G1 is C2 but G2 is C1
            rib(e1->rev, e1->color ^ e2->color);
        }
        perm[enew_1->color] = e1->color;
        perm[enew_2->color] = e2->color;
        perm[enew_1->color ^ enew_2->color] = e1->color ^ e2->color;
        color_change(g2, perm);
    }
	else if (c_i == 2 && c_j == 3) { // case 3
        // Add an edge between 2 and 4
        e = create_edge_between(vnew_2, vnew_4, find_edge(vnew_2, vnew_1), find_edge(vnew_4,vnew_3));
        find_coloring(g2);
		if(ERROR) return;
        remove_from_edge_list(e);
        remove_from_edge_list(e->rev);
        free(e->rev);
        free(e);
        if (enew_1->color != enew_2->color) { // G1 is C3 but G2 is C2
            rib(e1->rev,e1->color^e4->color); // Make G2 into C2
        }
        perm[enew_1->color]=e1->color;
        perm[enew_4->color]=e4->color;
        perm[enew_1->color^enew_4->color] = e1->color^e4->color;
        color_change(g2, perm);
    }
	else {
        printf("ERROR: (scs_4) not able to recolor\n");
        ERROR = true;
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
    // combine the vertex lists
    v_temp = g1->vert->next;
    g1->vert->next = g2->vert->next;
    g2->vert->next->prev = g1->vert;
    g2->vert->next = v_temp;
    v_temp->prev = g2->vert;
    g1->n += g2->n;
    free(g2);
    free(vnew_1);
    free(vnew_2);
    free(vnew_3);
    free(vnew_4);
    free(enew_1->rev);
    free(enew_1);
    free(enew_2->rev);
    free(enew_2);
    free(enew_3->rev);
    free(enew_3);
    free(enew_4->rev);
    free(enew_4);
}

int find_coloring_case(bool *visited, struct edge **circuit, struct edge **circuit2){
	if( visited[3] && visited[0] && visited[6] ){
		return 1;
		// C1
	}
	else if( visited[0] && visited[4] && visited[1] ){
		rotate_circuit(circuit, 1);
		rotate_circuit(circuit2, 1);
		return 1;
		// C2
	}
	else if( visited[4] && visited[7] && visited[5] ){
		rotate_circuit(circuit, 2);
		rotate_circuit(circuit2, 2);
		return 1;
		// C3
	}
	else if( visited[7] && visited[9] && visited[8] ){
		rotate_circuit(circuit, 3);
		rotate_circuit(circuit2, 3);
		return 1;
		// C4
	}
	else if( visited[9] && visited[3] && visited[2] ){
		rotate_circuit(circuit, 4);
		rotate_circuit(circuit2, 4);
		return 1;
		// C5
	}
	else if( visited[5] && visited[6] && visited[7] && visited[8] ){
		return 2;
		// D1
	}
	else if( visited[8] && visited[1] && visited[9] && visited[2] ){
		rotate_circuit(circuit, 1);
		rotate_circuit(circuit2, 1);
		return 2;
		// D2
	}
	else if( visited[2] && visited[5] && visited[3] && visited[6] ){
		rotate_circuit(circuit, 2);
		rotate_circuit(circuit2, 2);
		return 2;
		// D3
	}
	else if( visited[6] && visited[8] && visited[0] && visited[1] ){
		rotate_circuit(circuit, 3);
		rotate_circuit(circuit2, 3);
		return 2;
		// D4
	}
	else if( visited[1] && visited[2] && visited[4] && visited[5] ){
		rotate_circuit(circuit, 4);
		rotate_circuit(circuit2, 4);
		return 2;
		// D5
	}
	else if( visited[0] && visited[4] && visited[7] && visited[9] && visited[3] ){
		return 3;
		// E
	}
	ERROR = true;
	printf("Couldn't find correct scs5 coloring case\n");
	return -1;
}

// circuit size four, contains at least two vertices inside circuit, edges in cw order
void scs_5(struct graph* g1,struct edge* e1,struct edge* e2,struct edge* e3, struct edge* e4, struct edge* e5) {
	int i, j, coloring_case, temp_deg;
	struct graph *g2;
	struct vertex *vnew_1, *vnew_2, *vnew_3, *vnew_4, *vnew_5;
	struct vertex *v_adj, *v_adj_D1, *v_temp;
	struct vertex vert;
	struct edge *edge_list[5], *edge_list2[5];
	struct edge *e, *e_temp;
	struct edge *enew_1, *enew_2, *enew_3, *enew_4, *enew_5;
	int perm[] = {0, 0, 0, 0};
	bool *visited1, *visited2;
	
	edge_list[0] = e1;
	edge_list[1] = e2;
	edge_list[2] = e3;
	edge_list[3] = e4;
	edge_list[4] = e5;
	g2 = split_graph(g1, 5, edge_list);
	vnew_1 = g2->vert->prev->prev->prev->prev->prev; // fifth last addition is added vertex
	vnew_2 = g2->vert->prev->prev->prev->prev;
	vnew_3 = g2->vert->prev->prev->prev;
	vnew_4 = g2->vert->prev->prev;
	vnew_5 = g2->vert->prev;
	enew_1 = find_edge(vnew_1, vnew_2);
	enew_2 = find_edge(vnew_2, vnew_3);
	enew_3 = find_edge(vnew_3, vnew_4);
	enew_4 = find_edge(vnew_4, vnew_5);
	enew_5 = find_edge(vnew_5, vnew_1);
	edge_list2[0] = enew_1;
	edge_list2[1] = enew_2;
	edge_list2[2] = enew_3;
	edge_list2[3] = enew_4;
	edge_list2[4] = enew_5;
	// Let T1 be obtained from H1 by adding a new vertex adjacent to v1 , ..., v5 .
	v_adj = &vert;
	v_adj->e = NULL;
	v_adj->num = num_label++;
	v_adj->deg = 0;
	add_to_vertex_list(g1, v_adj);
	e = create_edge_between(e1->v, v_adj, e1, NULL);
	e = create_edge_between(e2->v, v_adj, e2, e->rev);
	e = create_edge_between(e3->v, v_adj, e3, e->rev);
	e = create_edge_between(e4->v, v_adj, e4, e->rev);
	e = create_edge_between(e5->v, v_adj, e5, e->rev);
	find_coloring(g1);
	if(ERROR) return;
	j = v_adj->deg;
	for (i = 0; i < j; i++) {
		e = v_adj->e;
		remove_from_edge_list(v_adj->e);
		remove_from_edge_list(e->rev);
		free(e->rev);
		free(e);
	}
	remove_from_vertex_list(g1, v_adj);
	visited1 = find_scs5_coloring_pattern(edge_list);
	coloring_case = find_coloring_case(visited1, edge_list, edge_list2);	// rotates the circuits if necessary

	// find_coloring_case may rotate the graph
	e1 = edge_list[0];
	e2 = edge_list[1];
	e3 = edge_list[2];
	e4 = edge_list[3];
	e5 = edge_list[4];
	enew_1 = edge_list2[0];
	enew_2 = edge_list2[1];
	enew_3 = edge_list2[2];
	enew_4 = edge_list2[3];
	enew_5 = edge_list2[4];
	
	if (coloring_case == 1) { // C1
		//Let T2 be obtained from H2 by deleting e4 and identifying v3 with v5
		v_temp = enew_5->v;
		remove_from_edge_list(enew_4);
		remove_from_edge_list(enew_4->rev);
		remove_from_vertex_list(g2, v_temp);
		
		temp_deg = v_temp->deg;
		enew_3->v->deg += temp_deg;
		e = enew_5;
		for (i = 0; i < temp_deg; i++) {
			e->v = enew_3->v;
			e = e->next;
		}
		enew_2->rev->next = enew_5;
		enew_5->prev = enew_2->rev;
		enew_4->rev->prev->next = enew_3;
		enew_3->prev = enew_4->rev->prev;
		
		find_coloring(g2);
		if(ERROR) return;
		
		enew_1->v->e = enew_1;
		enew_2->v->e = enew_2;
		enew_3->v->e = enew_3; // can get shifted to v5's edge during coloring
		enew_4->v->e = enew_4;
		
		enew_3->prev = enew_2->rev;
		enew_4->rev->prev->next = enew_4->rev;
		enew_5->prev = enew_4->rev;
		enew_2->rev->next = enew_3;
		e = enew_5;
		for (i = 0; i < temp_deg; i++) {
			e->v = v_temp;
			e = e->next;
		}
		enew_3->v->deg -= temp_deg;
		add_to_vertex_list(g2,v_temp);
		reinsert_to_edge_list(enew_4->rev);
		reinsert_to_edge_list(enew_4);
		enew_4->color = enew_3->color;
		enew_4->rev->color = enew_3->color;
		enew_5->v->e = enew_5;
	}
	else if (coloring_case == 2) { // D1		
    	// Let T2 be obtained from H2 by adding two edges with ends v2v4 , v2v5
		e = create_edge_between(enew_2->v, enew_5->v, enew_1->rev, enew_4->rev);
		e_temp = create_edge_between(enew_2->v, enew_4->v, e, enew_3->rev);
		find_coloring(g2);
		if(ERROR) return;
		// Undo changes
		remove_from_edge_list(e);
		remove_from_edge_list(e->rev);
		remove_from_edge_list(e_temp);
		remove_from_edge_list(e_temp->rev);

		free(e->rev);
		free(e);
		free(e_temp->rev);
		free(e_temp);
	}
	else { // E
		// Let T2 be obtained from H2 by adding a new vertex adjacent to v1,v2,v3,v4,v5
		v_adj_D1 = (struct vertex*) malloc(sizeof(struct vertex));
		if( v_adj_D1 == NULL ){	
			printf("Memory error: not enough space for vertex in scs5\n");
			ERROR = true;
			return;
		}
		v_adj_D1->num = num_label;
		v_adj_D1->deg = 0;
		num_label++;
		add_to_vertex_list(g2,v_adj_D1);
		e = create_edge_between(enew_1->v, v_adj_D1, enew_5->rev, NULL);
		create_edge_between(enew_2->v, v_adj_D1, enew_1->rev, e->rev);
		create_edge_between(enew_3->v, v_adj_D1, enew_2->rev, e->rev);
		create_edge_between(enew_4->v, v_adj_D1, enew_3->rev, e->rev);
		create_edge_between(enew_5->v, v_adj_D1, enew_4->rev, e->rev);
		find_coloring(g2);
		if(ERROR) return;
		check_graph(g2);
		// Undo changes
		j = v_adj_D1->deg;
		for (i = 0; i < j; i++) {
			e = v_adj_D1->e;
			remove_from_edge_list(e);
			remove_from_edge_list(e->rev);

			free(e->rev);
			free(e);
		}
		remove_from_vertex_list(g2,v_adj_D1);
		free(v_adj_D1);
	}   
	
	visited1 = find_scs5_coloring_pattern(edge_list);
	visited2 = find_scs5_coloring_pattern(edge_list2);
	for(i = 0; i < 10; i++){
		if( visited1[i] &&  visited2[i] ){
			change_scs5_circuit(edge_list, i);
			change_scs5_circuit(edge_list2, i);
			break;	
		}
	}
	free(visited1);
	free(visited2);
	if(i == 10){
		ERROR = true;
		printf("Error: SCS5 couldn't find a common circuit coloring\n");
		return;
	}
	
	// All five colors appear in the circuit :)
	perm[edge_list2[0]->color] = edge_list[0]->color;
	perm[edge_list2[1]->color] = edge_list[1]->color;
	perm[edge_list2[2]->color] = edge_list[2]->color;
	perm[edge_list2[3]->color] = edge_list[3]->color;
	perm[edge_list2[4]->color] = edge_list[4]->color;	
	color_change(g2, perm);	
	
	// Join G1 and G2 together again
	enew_1->v->e = enew_1;
	enew_2->v->e = enew_2;
	enew_3->v->e = enew_3;
	enew_4->v->e = enew_4;
	enew_5->v->e = enew_5;

	e1->v->e = e1;
	e2->v->e = e2;
	e3->v->e = e3;
	e4->v->e = e4;
	e5->v->e = e5;
	fuse_vertex(e1->v, enew_1->v);
	fuse_vertex(e2->v, enew_2->v);
	fuse_vertex(e3->v, enew_3->v);
	fuse_vertex(e4->v, enew_4->v);
	fuse_vertex(e5->v, enew_5->v);
	remove_from_vertex_list(g2,vnew_1);
	remove_from_vertex_list(g2,vnew_2);
	remove_from_vertex_list(g2,vnew_3);
	remove_from_vertex_list(g2,vnew_4);
	remove_from_vertex_list(g2,vnew_5);
	v_temp = g1->vert->next;
	g1->vert->next = g2->vert->next;
	g2->vert->next->prev = g1->vert;
	g2->vert->next = v_temp;
	v_temp->prev = g2->vert;
	g1->n += g2->n;
	
	free(g2);
	free(vnew_1);
	free(vnew_2);
	free(vnew_3);
	free(vnew_4);
	free(vnew_5);
	free(enew_1->rev);
	free(enew_1);
	free(enew_2->rev);
	free(enew_2);
	free(enew_3->rev);
	free(enew_3);
	free(enew_4->rev);
	free(enew_4);
	free(enew_5->rev);
	free(enew_5);
}

// assumes vertices from 1 to g->n
// finds one possible coloring of the configuration
// not currently in use
void conf_brute_force_coloring(int conf_num){
	struct graph* g;
	struct vertex* v;
	struct edge* e;
	int i,r,n;
	bool works = true;

	// uses 1, 2, 4 and 8 as colors
	r = conf[conf_num].r;
	g = conf[conf_num].g;
	n = conf[conf_num].n;
	v = g->vert;
	for(i = 1; i <= r; i++) v = v->next;
	// sends the colors from 0, 1, 2, 3 to 1, 2, 4, 8 so the brute force is more efficient
	for(i = 1; i <= r; i++){
		color[i] = 1 << color[i];
	}
	for(i = r + 1; i <= n; i++) color[i] = 0;
	while(v != g->vert){
		e = v->e;
		if(color[v->num] == 0){
			color_res[v->num] = 0;
			for(i = 0; i < v->deg; i++){
				color_res[v->num] |= color[e->rev->v->num];
				e = e->next;
			}
			if(color_res[v->num] == 15){
				v = v->prev;
				continue;
			}
			color[v->num] = 1;
		}
		else color[v->num] <<= 1;
		while( (color[v->num] & color_res[v->num]) && color[v->num] < 16 ) color[v->num] <<= 1;
		if(color[v->num] == 16){
			color[v->num] = 0;
			v = v->prev;
		}
		else v = v->next;
	}
	// sends colors 1,2,4,8 back to 0,1,2,3 respec
	for(i = 1; i <= n; i++){
		color[i] /= 2;
		if(color[i] == 4) color[i]--;
	}
}

// given a coloring of a circuit, finds the lexicographical order among the coloring with equal parity for all colors
int find_index_of_coloring(struct edge** circuit, int size){
	int arr[15];
	int pos, x;
	int i,j;

	for(i = 0; i < size; i++){
		arr[i] = circuit[i]->color;
	}
	// find lexicographically minimal permutation of the colors
	if(arr[0] == 2){	//swap 1 and 2
		for(i = 0; i < size; i++){
			if(arr[i] < 3) arr[i] = 3 - arr[i];
		}
	}
	else if(arr[0] == 3){
		for(i = 0; i < size; i++){
			if(arr[i] != 2) arr[i] ^= 2;
		}
	}
	for(i = 0; i < size; i++){
		if(arr[i] == 2) break;
		if(arr[i] == 3){
			for( ; i < size; i++){
				if(arr[i] != 1) arr[i] ^= 1;
			}
		}
	}
	if(size % 2 == 0) x = 0;
	else x = 7;
	pos = 0;
	for(i = 0; i < size; i++){
		for(j = 0; j < arr[i] - 1; j++){
			pos += dp[size - i - 1][x ^ (1 << j)];
		}
		x ^= (1 << (arr[i] - 1));
	}
	return pos;
}

// returns the position of the coloring in the list if found, else returns -1
int search_conf_colorings(int* list, int coloring, int start, int end){
	int mid = (start + end) / 2;
	if(start == end){
		if(list[start] == coloring) return start;
		return -1;
	}
	if(list[mid] < coloring) return search_conf_colorings(list,coloring, mid + 1, end);
	return search_conf_colorings(list,coloring, start, mid);
}

// admits that the previous face is part of the configuration and that the interior of the configuration is colored 0 
// performs a rib change starting at e, fixing color and going to the face between e and e->next
void rib_conf(struct edge* e, int color){
	while(e->color != 0){		// condition for not being inside the deleted conf
		e->color ^= color;
		e->rev->color = e->color;
		if(e->next->color == color) e = e->next->rev->next;
		else e = e->next;
	}
}

// performs a rib change dfs to search for ring colorings for which the interior of the configuration can be filled
int change_circuit_dfs(struct edge** circuit, int size, int conf_num){
	int i, j, other_color, cur_case;
	int pos, res;
	bool outside;	
			
	// the configuration can be either inside, clockwise between circuit[i] and circuit[i-1]->rev
	// or outside, clockwise between circuit[i-1]->rev and circuit[i]. This matters for the rib_conf calls
	outside = (circuit[0]->prev->color == 0) || (circuit[1]->prev->color == 0);	// two checks in case circuit[0] and circuit[size-1] are in a triangle
	
	if(conf[conf_num].ring_colorings == NULL) find_ring_colorings(conf_num);
	cur_case = find_index_of_coloring(circuit, size);
	ring_col_visited[cur_case] = cur_dfs;				// if a coloring wasn't visited so far it has ring_col_visited < cur_dfs, else it is equal
	for(i = 0; i < size; i++){
		other_color = (circuit[i]->color % 3) + 1;		// one of the colors from 1 to 3 different than circuit[i]->color
		if(outside) rib_conf(circuit[i], other_color);
		else rib_conf(circuit[i]->rev, other_color);
		cur_case = find_index_of_coloring(circuit, size);
		pos = search_conf_colorings(conf[conf_num].ring_colorings, cur_case, 0, conf[conf_num].a - 1);
		if( pos >= 0 ) return *(conf[conf_num].conf_colorings + pos);
		if( ring_col_visited[cur_case] != cur_dfs ){
			res = change_circuit_dfs(circuit, size, conf_num);
			if( res >= 0 ) return res;
		}
		if(outside) rib_conf(circuit[i], other_color);
		else rib_conf(circuit[i]->rev, other_color);
		//
		other_color ^= circuit[i]->color;			// the other color from 1 to 3 different than circuit[i]->color
		if(outside) rib_conf(circuit[i], other_color);
		else rib_conf(circuit[i]->rev,other_color);
		cur_case = find_index_of_coloring(circuit, size);
		pos = search_conf_colorings(conf[conf_num].ring_colorings, cur_case, 0, conf[conf_num].a - 1);
		if( pos >= 0 ) return *(conf[conf_num].conf_colorings + pos);
		if( ring_col_visited[cur_case] != cur_dfs ){
			res = change_circuit_dfs(circuit, size, conf_num);
			if( res >= 0 ) return res;
		}
		if(outside) rib_conf(circuit[i], other_color);
		else rib_conf(circuit[i]->rev, other_color);
	}
	return -1;
}

// reduces a configuration, colors it accordingly, undoes the reduction and adjusts the coloring
void reduce_and_color_conf(struct graph* g, int conf_num, struct vertex ** map) {
	struct edge** deleted_conf_edges;
	struct edge* edge_list[conf[conf_num].r];
	struct edge* circuit[14];
	struct edge* contracted_edges[4];
	struct edge *e, *eg, *e2, *e3;
	struct vertex *v, *v1, *v2, *vg, *vnew;
	int i, j, r, n, coloring, pos;

	r = conf[conf_num].r;
	n = conf[conf_num].n;
	// reduce
	if (conf[conf_num].x == 0) {
		vnew = (struct vertex*) malloc(sizeof(struct vertex));
		vnew->deg = 0;
		deleted_conf_edges = delete_conf(g, conf_num, map);
		eg = NULL;
		// test the ordering
		e = find_edge(map[1],map[2]);
		e2 = find_edge(map[2], map[3]);
		e3 = find_edge(map[3], map[4]);
		// configuration deleted from the "inside"
		if ( e->rev->prev == e2 && e2->rev->prev == e3 ) {	// two checks in case map[2]->deg == 2 after deletion
			for (i = 1; i <= r; i++) {
				e = find_edge(map[i], map[i < r ? i+1 : 1]);
				eg = create_edge_between(vnew, map[i], eg, e);
			}
		} 
		else {		// configuration deleted from the "outside"
			for (i = r; i >= 1; i--) {
				e = find_edge(map[i], map[i > 1 ? i-1 : r]);
            	eg = create_edge_between(vnew, map[i], eg, e);
    		}
    	}
    	add_to_vertex_list(g, vnew);
    	vnew->num = num_label++;
    	
		find_coloring(g);
		if(ERROR) return;
		remove_from_vertex_list(g,vnew);
		for (i = 1; i <= r; i++) {
			e = vnew->e;
			remove_from_edge_list(e);
			remove_from_edge_list(e->rev);
		}
		free(vnew);
		reinsert_conf(g, conf_num, map, deleted_conf_edges);
	}
	else{
		for (i = 0; i<conf[conf_num].x; i++) {
            v1 = map[conf[conf_num].X[i]->v->num];
            v2 = map[conf[conf_num].X[i]->rev->v->num];
            // contracted edges can be adjacent, need to reflect changes in map
            for (j = 0; j < i; j++) {
                e = contracted_edges[j];
                if (e->v == v1 || e->rev->v == v1) {
                    v1 = e->prev->v;
                }
				if (e->v == v2 || e->rev->v == v2) {
                    v2 = e->prev->v;
                }
            }
            e = find_edge(v1, v2);
            contract_edge(g, e);
            contracted_edges[i] = e;
        }
        find_coloring(g);
		if(ERROR) return;
    	for (i = conf[conf_num].x-1; i >= 0; i--) {
            decontract_edge(g, contracted_edges[i]);
    	}
    }
    // decolor the inside of the configuration so the ribs work
	for(i = r + 1; i <= n; i++){
		v = map[i];
		e = v->e;
		for(j = 0; j < v->deg; j++){
			e->color = 0;
			e->rev->color = 0;
			e = e->next;
		}
		v = v->next;
	}

    // constructs the circuit so we can call change_circuit_dfs which uses ribs to change the ring coloring to one with an extension to the configuration
    for(i = 1; i <= r; i++){
    	circuit[i-1] = find_edge(map[i], map[(i % r) + 1]);
    }
    // change_circuit_dfs finds out either the change has to be done inside or outside the circuit
	if(conf[conf_num].ring_colorings == NULL) find_ring_colorings(conf_num);
	pos = search_conf_colorings(conf[conf_num].ring_colorings, find_index_of_coloring(circuit, r), 0, conf[conf_num].a - 1);
	if( pos >= 0 ) coloring = *(conf[conf_num].conf_colorings + pos);
    else coloring = change_circuit_dfs(circuit, r, conf_num);
    if(coloring < 0){
    	ERROR = true;
    	printf("change_circuit_dfs couldn't find a coloring of the configuration\n");
    	return;
    }

	bool swap12 = false;
	bool swap13 = false;
	bool swap23 = false;
	if((*(circuit))->color == 2){	//swap 1 and 2
		for(i = 0; i < r; i++){
			if((*(circuit + i))->color != 3) (*(circuit + i))->color ^= 3;
		}
		swap12 = true;
	}
	else if((*(circuit))->color == 3){	// swap 1 and 3
		for(i = 0; i < r; i++){
			if((*(circuit + i))->color != 2) (*(circuit + i))->color ^= 2;
		}
		swap13 = true;
	}
	for(i = 0; i < r; i++){
		if((*(circuit + i))->color == 2) break;
		if((*(circuit + i))->color == 3){	// swap 2 and 3
			for( ; i < r; i++){
				if((*(circuit + i))->color != 1) (*(circuit + i))->color ^= 1;
			}
			swap23 = true;
		}
	}
	
    color[1] = 0;
    for(i = 2; i <= r; i++){
    	color[i] = color[i - 1] ^ (*(circuit + i - 2))->color;
    }
    for(i = n; i > r; i--){
    	color[i] = coloring % 4;
    	coloring /= 4;
    }
	v = conf[conf_num].g->vert;
	for(i = 1; i <= r; i++) v = v->next;
	for(i = r + 1; i <= n; i++){
		e = v->e;
		for(j = 0; j < v->deg; j++){
			eg = find_edge(map[v->num], map[e->rev->v->num]);
			eg->color = color[v->num] ^ color[e->rev->v->num];
			if(swap23 && eg->color != 1) eg->color ^= 1;
			if(swap13 && eg->color != 2) eg->color ^= 2;
			if(swap12 && eg->color != 3) eg->color ^= 3;
			eg->rev->color = eg->color;
			e = e->next;
		}
		v = v->next;
	}
	if(swap12 || swap13 || swap23){
		for(i = 0; i < r; i++){
			(*(circuit + i))->color = (*(circuit + i))->rev->color;
		}
	}
}

// a brute force that finds ring colorings of a configuration in lexicographical order
void find_ring_colorings(int conf_num){
	struct vertex* v;
	struct edge* e;
	int i, j, r, n, x;
	int num_colorings = 0;
	bool works = true;
	int last;
	int ring_cipher, conf_cipher;
	
	// since we care about the lexicographical order of the color of the edges but we color the vertices, 
	// some changes are made to the usual brute force algorithm to take that into account

	conf[conf_num].ring_colorings = (int*) malloc(sizeof(int) * conf[conf_num].a);
	conf[conf_num].conf_colorings = (int*) malloc(sizeof(int) * conf[conf_num].a);

	if( conf[conf_num].ring_colorings == NULL ){
		printf("Not enough memory for ring colorings in find_ring_colorings %d\n",conf_num);
		ERROR = true;
		return;
	}
	if( conf[conf_num].conf_colorings == NULL ){
		printf("Not enough memory for configuration colorings in find_ring_colorings %d\n",conf_num);
		ERROR = true;
		return;
	}

	// uses 0, 1, 2 and 3 as colors
	// color_res uses 1, 2, 4 and 8 to identify colors
	r = conf[conf_num].r;
	n = conf[conf_num].n;
	v = conf[conf_num].g->vert->next;
	color[1] = 0;
	for(i = 2; i <= r; i++){	// color[] receives the first lexicagraphical ring coloring for edges adapted to a vertex coloring 
		color[i] = color[i-1] ^ 1;	// alternates 0 and 1
		color_res[i] = (1 << color[i-1]);
		v = v->next;				// v->num == r + 1 at the end
	}
	if(r % 2 == 1){				// odd case is a bit different since the xor of all edges in a cycle must be zero
		last = r - 1;
		color[r] = 3;
		color_res[r] = 3;
	}
	else last = r;	// last is the last index that follows the pattern 0, 1, 0, 1, ..., it is used for lexicographical purposes
	for(i = r + 1; i <= n; i++) color[i] = -1;
	while(last >= 2){
		if(v->num == last) last--;
		e = v->e;
		if(color[v->num] == -1){
			color_res[v->num] = 0;
			for(i = 0; i < v->deg; i++){
				if(color[e->rev->v->num] != -1) color_res[v->num] |= (1 << color[e->rev->v->num]);
				e = e->next;
			}
			if(color_res[v->num] == 15){
				v = v->prev;
				continue;
			}
			color[v->num] = color[v->num - 1];
		}
		else color[v->num] = ((color[v->num] ^ color[v->num - 1]) + 1) ^ color[v->num - 1];
		while( ( ( ((1 << color[v->num]) & color_res[v->num]) > 0 ) && (color[v->num] ^ color[v->num - 1]) < 4 )	// condition for the color to be valid
			|| // condition for lexicographical
			((v->num == last + 1) && (((color[v->num] == 2) && (color[v->num-1] == 1)) || ((color[v->num] == 3) && (color[v->num-1] == 0))) ) )	
			color[v->num] = ((color[v->num] ^ color[v->num - 1]) + 1) ^ color[v->num - 1];		// receives next color lexicagraphically for the edges
		if((color[v->num] ^ color[v->num - 1]) >= 4){	// tried all colorings, go to previoud vertex and continue 
			color[v->num] = -1;
			v = v->prev;
		}
		else if(v->num == n){
			// found a new coloring, store the coloring
			ring_cipher = 0;
			if(r % 2 == 0) x = 0;
			else x = 7;
			for(i = 1; i < r; i++){
				for(j = 0; j < (color[i] ^ color[i % r + 1]) - 1; j++){
					ring_cipher += dp[r - i][x ^ (1 << j)];		
				}
				x ^= (1 << ((color[i] ^ color[i % r + 1]) - 1));
			}
			conf[conf_num].ring_colorings[num_colorings] = ring_cipher;	
			// ring_cipher receives the index of this coloring among all colorings of the ring that xor to zero
			conf_cipher = color[r + 1];
			for(i = r + 2; i <= n; i++){
				conf_cipher <<= 2;
				conf_cipher += color[i];
			}
			conf[conf_num].conf_colorings[num_colorings] = conf_cipher;	// conf_cipher is simply the vertex color of the interior in base 4
			num_colorings++;
			// go to the next possible ring
			for(i = r + 1; i <= n; i++){
				color[i] = -1;
				color_res[i] = 0;
				v = v->prev;
			}
			// v->num == r
		}
		else v = v->next;
	}
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
}

int main(){
	struct graph* g;
	char data[256];
	read_conf("configurations.txt");
	printf("Please input the name of the file containing the graph.\n");
	scanf("%s",data);
	g = read_graph(data);
	check_graph(g);
	if(! ERROR){
		find_coloring(g);
		color_vertices(g);
	}
	scanf("%c");
	return 0;
}
