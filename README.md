# Four-Color-Theorem
Implementation of algorithm to 4-color planar graphs in C. Based on the paper https://people.math.gatech.edu/~thomas/PAP/fc.pdf. 

Sample graphs are provided as example_x.txt. The input graphs must be loopless, planar triangulation, and with no parallel edges. The format of the input graph is as follows: <br/>
<br/>
n <- number of vertices <br/>
degree(v1) degree(v2) … degree(vn) <- degree of vertex 1, vertex 2, …, vertex n. <br/>
neighbor_1_v1 neighbor_2_v1 … neighbor_i_v1 <- clockwise ordering for edges to neighboring vertices, so that consecutive edges in the list are also consecutive in the graph.  <br/>
neighbor_1_v2 neighbor_2_v2 … neighbor_j_v2  <br/>
. <br/>
. <br/>
. <br/>
neighbor_1_vn neighbor_2_vn … neighbor_k_vn <br/>
<br/>
Please take the following steps to run this code:
1. download the files into a folder or clone the repository
2. open your Command Prompt/Terminal and navigate to the directory containing the files, such as: cd ~/Desktop/Four-Color-Theorem
3. open your graph.c file and scroll to the main function at the bottom of the file
4. change the text inside read_graph(“test.txt”) to the name of the file containing your graph 
5. compile your code: gcc -o graph graph.c
6. run your code: ./graph
