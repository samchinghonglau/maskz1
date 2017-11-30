#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "list.h"

void print (element_t ev) {
  char* e = ev;
  for (int i = 0; i < strlen(e); i++){
  printf ("%lc", e[i]);
  }
  printf ("\n");
}

void print2 (element_t ev) {
  intptr_t e = (intptr_t) ev;
  printf ("%ld\n", e);
}

void number (element_t* rv,element_t av){
	char* a = av;
	intptr_t r;

	if (isdigit(*a)){
		r = strtol(a,NULL,0);
	} else {
		r = -1;
	}
	intptr_t* b = (intptr_t*) rv;
	*b = r;
}

void nul (element_t* rv, element_t av, element_t bv) {
	intptr_t a = (intptr_t) av;
	char* b = bv;
	
	if (*rv == NULL)
    *rv = malloc (sizeof(char)* strlen(b));
	char* r = *rv;
	
	
	if (a == -1){
		for (int i= 0; i < strlen(b); i++)
		r[i] = b[i];
	} else {
		*r = NULL;
	}
	
}

int nonul (element_t av){
	intptr_t* a = (intptr_t) av;
	return !(*a == NULL);
}

int noneg (element_t av){
	intptr_t a = (intptr_t) av;
	return (a >= 0);	
}

void trun (element_t* rv, element_t av, element_t bv) {
	intptr_t a = (intptr_t) av;
	char* b = bv;
	if (*rv == NULL)
		*rv = malloc (sizeof(char)* strlen(bv));
	char* r = *rv;
	b[a] = 0;
	for (int i= 0; i < strlen(b); i++)
		r[i] = b[i];
}

void biggest (element_t* rv, element_t av, element_t bv){
	intptr_t  a = (intptr_t)  av;
    intptr_t  b = (intptr_t)  bv;
    intptr_t* r = (intptr_t*) rv;
	if (b > a)
		*r = b;
}

int main(int argc, char* argv[]){
	struct list* l0 = list_create();
	for (int i = 1; i < argc; i++){
		list_append(l0,argv[i]);
	}
	
	struct list* l1 = list_create();	//numbers plus -1
	list_map1(number,l1,l0);
	list_foreach (print2, l1);
	
	struct list* l2 = list_create();	//char plus null
	list_map2 (nul, l2, l1, l0);
	//list_foreach (print, l2);
	
	struct list* l3 = list_create();	//list of numbers
	list_filter (noneg,l3,l1); 			
	//list_foreach (print2, l3);
	
	struct list* l4 = list_create();	//list of char
	list_filter (nonul,l4,l2);			
	//list_foreach (print, l4);
	
	struct list* l5 = list_create();
	list_map2(trun, l5, l3, l4);
	//list_foreach (print, l5);
	
	list_foreach (print, l0);
	list_foreach (print, l5);
	
	intptr_t s = 0;
	list_foldl (biggest , (element_t*) &s, l3);
    printf ("%ld\n", s);
    
    
    printf("it reaches here!!!\n");
	
	list_destroy (l0);
	list_destroy (l1);
	list_destroy (l2);
	list_destroy (l3);
	list_destroy (l4);
	list_destroy (l5);
	
}