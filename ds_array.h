/*Name: Alexander Chin
 * Student No. 1046121
 * achin@uoguelph.ca
 * CIS2520         
 * */

#include <stdio.h> 
#define MAX_ELEMENTS 256

/*array functions*/
int ds_create_array();
int ds_init_array();

void show_array();

int ds_replace(int value, long index);
int ds_insert(int value, long index);
int ds_delete(long index);
int ds_swap(long index1, long index2);
long ds_find(int target);
int ds_read_elements(char *filename);
int ds_finish_array();
