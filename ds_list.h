/*Name: Alexander Chin
 * Student No. 1046121
 * achin@uoguelph.ca
 * CIS2520         
 * */

#include <stdio.h> 

struct ds_list_item_struct{
	int item;
	long next;
};

/*linked list functions*/
int ds_create_list();
int ds_init_list();
void show_list();
int ds_replace(int value, long index);
int ds_insert(int value, long index);
int ds_delete(long index);

int getHigher(int val1, int val2);

int ds_swap(long index1, long index2);
long ds_find(int target);
int ds_read_elements(char* filename);
int ds_finish_list();
