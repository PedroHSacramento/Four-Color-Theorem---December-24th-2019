# Four-Color-Theorem
Implementation of algorithm to 4-color planar graphs in C. Based on the paper https://people.math.gatech.edu/~thomas/PAP/fc.pdf. Wirtten jointly by Pedro Sacramento and Haidar Jamal as part of a REU program in Georgie Tech over the Summer of 2019 overseen by Dr. Robin Thomas.
This is the version of the code as of December 24th, 2019. There are still some bugs to be fixed but the code is able to reliably color graphs of up to a few hundred vertices.

Sample graphs are provided as example_x.txt. The input graphs must be loopless, planar, triangulated, and without parallel edges. The format of the input graph is as follows: <br/>
<br/>
n <- number of vertices <br/>
degree(v1) degree(v2) … degree(vn) <- degree of vertex 1, vertex 2, …, vertex n. <br/>
neighbor_1_v1 neighbor_2_v1 … neighbor_i_v1 <- clockwise ordering for edges to neighboring vertices
neighbor_1_v2 neighbor_2_v2 … neighbor_j_v2  <br/>
. <br/>
. <br/>
. <br/>
neighbor_1_vn neighbor_2_vn … neighbor_k_vn <br/>
<br/>
Please take the following steps to run this code:
1. download the files graph.c and configurations.txt into a folder or clone the repository
2. open your Command Prompt/Terminal and navigate to the directory containing the files, such as: cd ~/Desktop/Four-Color-Theorem
3. open your planar_graph_coloring.c file and scroll to the main function at the bottom of the file
4. compile your code: gcc -o planar_graph_coloring planar_graph_coloring.c
5. run your code: ./planar_graph_coloring
6. input the name of the file containing the graph to be colored
