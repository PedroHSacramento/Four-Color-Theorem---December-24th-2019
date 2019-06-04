#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool ERROR = false;
char error_msg[100];
int memory_in_use = 0;

struct edge {
	struct vertex* v;		// vertex the edge is connected to
	struct edge* next;		// next edge clockwise around v
	struct edge* prev;		// previous edge clockwise around v
	struct edge* rev;		// reverse edge
	int color;
};
//
struct vertex {
	int num;
	int deg;
	struct edge* e;			/* pointer to the first edge */
	struct vertex *next;
	struct vertex *prev;
};

struct graph {
	char name[100];
	int n;					/* number of vertices */
	struct vertex* vert;	/* pointer to the first vertex */
};

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

// prints error messages
static void error(){
	ERROR = true;
	printf("%s",error_msg);
}

// DFS to color the vertices
static void color_dfs(struct vertex* v, int* colored) {
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
static void color_vertices(struct graph* g) {
	int i;
	int* colored = (int*) malloc(sizeof(int) * (g->n + 1));
	for(i = 0; i <= g->n; i++) colored[i] = -1;
	colored[g->vert->num] = 0;
	color_dfs(g->vert, colored);
	for(i = 1; i <= g->n; i++) printf("%d:%d ", i, colored[i]);
	printf("\n");
}

// finds edge between v and u in the list of edges of v
static struct edge* find_edge(struct vertex* v, struct vertex* u){
	struct edge* e = v->e;
	int i;
	if(e == NULL){
		strcpy(error_msg, "Invalid function input in find_edge: e == NULL \n");
		error();
	}
	for(i = 0; i < v->deg; i++){
		if( e->rev->v == u ) return e;
		e = e->next;
	}
	strcpy(error_msg, "find_edgee couldn't find requested edge\n");
	error();
	return NULL;
}

// adds edge between v and u to the end of the list of edges of v, creates it if it doesn't exist
static void add_edge(struct vertex* v, struct vertex* u){
	struct edge* e_v;
	struct edge* e_u;
	if(v == u){
		strcpy(error_msg, "Incorrect function input: add_edge recieved a self loop\n");
		error();
		return;
	}
	// creates the right edge if this is the first time processing the edge
	if( u->e == NULL ){
		e_v = (struct edge*) malloc(sizeof(struct edge));
		e_u = (struct edge*) malloc(sizeof(struct edge));
		if(e_v == NULL || e_u == NULL){
			strcpy(error_msg, "Memory error: not enough space for edge in function add_edge\n");
			error();
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
			strcpy(error_msg, "Invalid output: find_edge\n");
			error();
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
static struct graph* read_graph(char *data){
	// first line is the number of vertices
	// second line is list of degrees of each vertex
	// subsequent lines (starting from i=0)
  	// are the vertices that vertex i is adjacent to
	int i, j, u;
	struct graph* g = (struct graph*) malloc(sizeof(struct graph));
	struct vertex* v1;
	struct vertex* v2;
	struct edge* e;
	FILE* ptr = fopen(data,"r");

	fscanf(ptr,"%d",&g->n);
	v1 = (struct vertex*) malloc(sizeof(struct vertex));
	if(v1 == NULL){
		strcpy(error_msg, "Memory error: not enough space for new vertex in function read_graph\n");
		error();
		return NULL;
	}
	g->vert = v1;
	v1->num = 1;
	for(i = 2; i <= g->n; i++){
		v2 = (struct vertex*) malloc(sizeof(struct vertex));
		if(v2 == NULL){
			strcpy(error_msg, "Memory error: not enough space for new vertex in function read_graph\n");
			error();
			return NULL;
		}
		v2->num = i;
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
			strcpy(error_msg, "Invalid degree, must be >= 0\n");
			error();
			return NULL;
		}
		v1 = v1->next;
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		for(j = 0; j < v1->deg; j++){
			fscanf(ptr,"%d",&u);
			if(u <= 0 && u > g->n){
				strcpy(error_msg, "Invalid vertex, must be between 1 and n\n");
				error();
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

// For debugging: read a graph with a fixed tri coloring
static struct graph* read_graph_colored(char *data){

	// first line is the number of vertices
	// second line is list of degrees of each vertex
	// subsequent lines (starting from i=0)
  	// are the vertices that vertex i is adjacent to

	int i, j, u;
	struct graph* g = (struct graph*) malloc(sizeof(struct graph));
	struct vertex* v1;
	struct vertex* v2;
	struct edge* e;

	FILE* ptr = fopen(data,"r");
	fscanf(ptr,"%d",&g->n);

	v1 = (struct vertex*) malloc(sizeof(struct vertex));
	if(v1 == NULL){
		strcpy(error_msg, "Memory error: not enough space for new vertex in function read_graph\n");
		error();
		return NULL;
	}
	g->vert = v1;
	v1->num = 1;
	for(i = 2; i <= g->n; i++){
		v2 = (struct vertex*) malloc(sizeof(struct vertex));
		if(v2 == NULL){
			strcpy(error_msg, "Memory error: not enough space for new vertex in function read_graph\n");
			error();
			return NULL;
		}
		v2->num = i;
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
			strcpy(error_msg, "Invalid degree, must be >= 0\n");
			error();
			return NULL;
		}
		v1 = v1->next;
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		for(j = 0; j < v1->deg; j++){
			fscanf(ptr,"%d",&u);
			if(u <= 0 && u > g->n){
				strcpy(error_msg, "Invalid vertex, must be between 1 and n\n");
				error();
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

// frees the whole graph // buggy
static void free_graph(struct graph* g){
	struct vertex* v = g->vert;
	struct edge* e;
	int i,j;
	// frees the edges
	for(i = 0; i < g->n; i++){
		e =  v->e;
		for(j = 1; j < v->deg; j++){
			debug("free edge %d %d\n",e->prev->v->num,e->prev->rev->v->num);
			free(e->prev);
			e = e->next;
		}
		free(e);
		v = v->next;
	}
	// frees the vertices
	for(i = 1; i < g->n; i++){
//		free(v->prev);
		v = v->next;
	}
//	free(v);
	// frees the graph
	free(g);
}

// debug function
// assumes graph is correct
static void print_dfs(struct vertex* v, bool* visited){
	int i;
	struct edge* e = v->e;
	visited[v->num]	= true;
	printf("vertex %d : ", v->num);
	for(i = 1; i <= v->deg; i++){
		printf("%d ",e->rev->v->num);
		e = e->next;
	}
	printf("\n");
	for(i = 1; i <= v->deg; i++){
		if( visited[ e->rev->v->num ] == false ){
			print_dfs(e->rev->v, visited);
		}
		e = e->next;
	}
}

// debug function
// assumes graph from 1 to n
static void print_graph(struct graph* g){
	int i;
	bool* visited = (bool*) malloc(sizeof(bool) * (g->n + 1));
	for(i = 0; i <= g->n; i++) visited[i] = false;
	print_dfs(g->vert, visited);
	free(visited);
}

int main(){
	struct graph* g;
	char data[256];
	printf("Enter file name including extension: \n");
	scanf("%s", data);
	g = read_graph(data);
	//g = read_graph_colored(data); // for debugging
	print_graph(g);
	//color_vertices(g); // final step
	//free_graph(g); // currently buggy
	//printf("Leaking memory: %d\n",memory_in_use);
	return 0;
}
