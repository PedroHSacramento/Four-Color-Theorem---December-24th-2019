#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define debug //

int num_label = 1;
FILE* ptr; 

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
	struct edge* e;			/* pointer to the first edge */
	struct vertex *next;
	struct vertex *prev;
};

struct graph {
	char name[100];
	int n;					/* number of vertices */
	struct vertex* vert;	/* pointer to the first vertex */
};

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

// eliminates vertices of degree three
void degree_three_elimination(struct graph* g){
	struct vertex* v;
	struct vertex* head;	// linked list of vertices deleted
	struct vertex* tail;	// linked list of vertices deleted
	struct edge* e;
	int i, n = g->n;
	// let v be the next meeber in the vertex list
	
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->deg == 3 ){
			head = v;
			break;
		}
		v = v->next;
	}
	if(i == g->n) return;
	
	// let v be the next member in the vertex list
	v = head->next;
	// remove v from the vertex list
	remove_from_vertex_list(g, head);
	// create linked list of vertices to delete 
	tail = head;
	head-> prev = tail->next = NULL;	
	// use n as limit since g->n varies
	for(i = 0; i <= n; i++){
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
	v = g->vert;
	for(i = 0; i < g->n; i++){
		if( v->deg == 3 ){
			printf("%d\n",i);
		}
		v = v->next;
	}
}

void print_graph(struct graph* g, FILE* ptr){
	int i,j;
	struct vertex* v = g->vert;
	struct edge* e;
	
	fprintf(ptr, "%d\n",g->n);
	v = g->vert;
	for(i = 1; i <= g->n; i++){
		while( v->num != i ) v = v->next;
		fprintf(ptr, "%d ",v->deg);
	}
	fprintf(ptr, "\n");
	for(i = 1; i <= g->n; i++){
		while( v->num != i ) v = v->next;
		e = v->e;
		for(j = 0; j < v->deg; j++){
			fprintf(ptr, "%d ",e->rev->v->num);
			e = e->next;
		}
		fprintf(ptr, "\n");
	}
	fprintf(ptr, "\n");
}

// finds edge between v and u in the list of edges of v
struct edge* find_edge(struct vertex* v, struct vertex* u){
	struct edge* e = v->e;
	int i;
	if(e == NULL){
		printf("Invalid function input in find_edge: e == NULL \n");
	}
	for(i = 0; i < v->deg; i++){
		if( e->rev->v == u ) return e;
		e = e->next;
	}
//	printf("find_edge couldn't find requested edge: %d -> %d\n",v->num, u->num);
//	ERROR = true;
	return NULL;
}

// adds vertex v to the "end" of the vertex list of graph g
void add_to_vertex_list(struct graph* g, struct vertex* v){
	if(g == NULL){
		printf("Null graph given to add_to_vertex_list\n");
		return;
	}
	if(v == NULL){
		printf("Null vertex given to add_to_vertex_list\n");
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

// adds edge between v and u to the end of the list of edges of v, creates it if it doesn't exist
// DOES NOT change the degree of v and u
void add_edge(struct vertex* v, struct vertex* u){
	struct edge* e_v;
	struct edge* e_u;
	
	debug("Adding edge %d %d\n",v->num, u->num);
	
	if(v == u){
		printf("Incorrect function input: add_edge recieved a self loop at vertex %d\n",v->num);
		return;
	}
	// creates the right edge if this is the first time processing the edge
	if( u->e == NULL ){
		e_v = (struct edge*) malloc(sizeof(struct edge));
		e_u = (struct edge*) malloc(sizeof(struct edge));
		if(e_v == NULL || e_u == NULL){
			printf("Memory error: not enough space for edge in function add_edge\n");
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
		if(e_u == NULL){
			printf("Invalid output: find_edge %d %d\n", u->num, v->num);
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

// creates an edge between e1->v and e2->v, cyclically after e1 around v and after e2 around v2
struct edge* create_edge_between(struct vertex* v1, struct vertex* v2, struct edge* e1, struct edge* e2){
	struct edge* e_new1 = (struct edge*) malloc(sizeof(struct edge));
	struct edge* e_new2 = (struct edge*) malloc(sizeof(struct edge));
	
	if(e_new1 == NULL || e_new2 == NULL){
		printf("Memory error: not enough space for new vertex in fcreate_edge_between\n");
		return;
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

void create_triangle(struct graph* g, struct edge* e){
	struct vertex* v1 = e->v;
	struct vertex* v2 = e->rev->v;
	struct vertex* v3 = (struct vertex*) malloc(sizeof(struct vertex));
	struct edge* e_aux;
	
	v3->deg = 0;
	v3->num = num_label;
	num_label++;
	v3->e = NULL;
	add_to_vertex_list(g, v3);
	
	e_aux = create_edge_between(v1, v3, e, NULL);
	create_edge_between(v2, v3, e->rev->prev, e_aux->rev);
}

void renumber_vertices(struct graph* g){
	struct vertex* v = g->vert;
	int i;
	for(i = 1; i <= g->n; i++){
		v->num = i;
		v = v->next;
	}	
}

// fills the face between e and e->next with edges until it is a triangulation
void face_fill(int size, struct graph* g, struct edge* e){
	struct edge* e1;
	struct edge* e2;
	struct vertex* v1;
	struct vertex* v2;
	int a, dist;
	
	if( size == 3 ) return;
	
//	print_graph(g);
	
	e1 = e;
	do{
		a = rand() % size;
		while(a > 0){
			e1 = e1->rev->prev;
			a--;
		}
		v1 = e1->v;
		e2 = e1;
		a = dist = rand() % (size - 3) + 2;	// a \in [2, size-2]
		while(a > 0){
			e2 = e2->rev->prev;
			a--;
		}
		v2 = e2->v;
//		debug("Trying edge %d %d\n",v1->num, v2->num);
	} while(find_edge(v1, v2) != NULL);
	e = create_edge_between(v1, v2, e1, e2);
	face_fill(dist + 1, g, e->rev);
	face_fill(size - dist + 1, g, e);
}

// fills the face between e and e->next with edges until it is a triangulation
//
void face_fill_deg_fix(int size, struct graph* g, struct edge* e){
	struct edge* e1;
	struct edge* e2;
	struct vertex* v1;
	struct vertex* v2;
	int i, dist;
	
	if( size == 3 ) return;
	
//	print_graph(g);
	while(size > 3){
		e1 = e;
		for(i = 0; i < size; i++){
			if(e1->v->deg <= 4 && e1->rev->v->deg >= 5) break;
			e1 = e1->rev->prev;
		}
		v1 = e1->v;
		e2 = e1->rev->prev->rev->prev;
		v2 = e2->v;
		e = create_edge_between(v1, v2, e1, e2);
		size--;
	}
}

struct graph* cycle_generator(int size){
	// first line is the number of vertices
	// second line is list of degrees of each vertex
	// subsequent lines (starting from i=0)
  	// are the vertices that vertex i is adjacent to
	int i, a;
	struct graph* g = (struct graph*) malloc(sizeof(struct graph));
	struct vertex* v1;
	struct edge* e;
	
	if(g == NULL){
		printf("Memory error: not enough space for graph in function read_graph\n");
		return NULL;
	}
	g->vert = NULL;
	g->n = 0;

	for(i = 1; i <= size; i++){
		v1 = (struct vertex*) malloc(sizeof(struct vertex));
		if(v1 == NULL){
			printf("Memory error: not enough space for new vertex in function read_graph\n");
			return NULL;
		}
		v1->num = num_label;
		num_label++;
		add_to_vertex_list(g, v1);
	}

	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		v1->e = NULL;
		v1->deg = 2;
		v1 = v1->next;
	}

	// creates a loop of size n
	v1 = g->vert;
	for(i = 1; i <= g->n; i++){
		add_edge(v1, v1->prev);
		add_edge(v1, v1->next);
		v1 = v1->next;
	}
	face_fill(size, g, v1->e);
	return g;
}

struct graph* random_planar_graph(int size){
	int n, i, a;
	struct graph* g = (struct graph*) malloc(sizeof(struct graph));
	struct vertex* v1;
	struct vertex* v2;
	struct edge* e;
	
	if(g == NULL){
		printf("Memory error: not enough space for graph in function read_graph\n");
		return NULL;
	}
	g->vert = NULL;
	g->n = 0;

	v1 = (struct vertex*) malloc(sizeof(struct vertex));
	v2 = (struct vertex*) malloc(sizeof(struct vertex));
	if(v1 == NULL || v2 == NULL){
		printf("Memory error: not enough space for new vertex in function read_graph\n");
		return NULL;
	}
	v1->num = num_label;
	num_label++;
	v2->num = num_label;
	num_label++;
	add_to_vertex_list(g, v1);
	add_to_vertex_list(g, v2);
	create_edge_between(v1, v2, NULL, NULL);
	e = v1->e;
	for(i = 2; i < size; i++){
		a = rand() % size;
		while(a > 0){
			e = e->rev->prev;
			a--;
		}
		create_triangle(g, e);
		e = e->next;
	}
	face_fill_deg_fix(size, g, e);
//	degree_three_elimination(g);
	renumber_vertices(g);
	return g;
}

int main(){
	int n;
	char data[100];
	srand (time(NULL));
	printf("Input size of the graph\n");
	scanf("%d",&n);
	printf("Input output file name\n");
	scanf("%s",data);
	ptr = fopen(data,"w");
	print_graph(random_planar_graph(n), ptr);
	return 0;
}
