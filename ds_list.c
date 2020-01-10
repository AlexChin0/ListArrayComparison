/*Name: Alexander Chin
 * Student No. 1046121
 * achin@uoguelph.ca
 * CIS2520         
 * */

#include "ds_memory.h"
#include "ds_list.h"

long elements;

/*This function uses ds_init to open a file and write the empty array*/
int ds_create_list()
{
	long headLong = -1;
	
	ds_init("list.bin");
	
	/*allocate space for a long then write zero to top of file*/
	ds_malloc(sizeof(long));
	ds_write(0, &headLong, sizeof(headLong));
		
	if(ds_finish() != 0)
	{
		return -1;
	}
	return 0;
}

/*Calls ds_init*/
int ds_init_list()
{
	ds_init("list.bin");
	return 0;
}

/*helper function to display the list*/
void show_list()
{
	long loc = 0;
	int i = 0;
	struct ds_list_item_struct li;
	
	ds_init_test();
	
	/*read the initial location*/
	ds_read(&loc, 0, sizeof(long));
	do
	{
		ds_read(&li, loc, sizeof(struct ds_list_item_struct));
		loc = li.next;
		i++;
	}while(li.next != -1);
	
}

/*This function goes to the index given and replaces the value there*/
int ds_replace(int value, long index)
{	
	struct ds_list_item_struct new;
	struct ds_list_item_struct previous;
	int i;

	/*read the first long from the file*/
	ds_read(&previous.next, 0, sizeof(long));
	
	for(i = index; i > 0; i--)
	{
		if(previous.next == -1)
		{
			return -1;
		}
		else
		{
			/*read the next address to continue traversal*/
			ds_read(&previous, previous.next, sizeof(struct ds_list_item_struct));
		}
	}
	/*get the old node and change the value before writing*/
	ds_read(&new, previous.next, sizeof(struct ds_list_item_struct));
	new.item = value;
	ds_write(previous.next, &new, sizeof(struct ds_list_item_struct));
	return 0;
}

/*This function adds a node to the list at the given index*/
int ds_insert(int value, long index)
{
	struct ds_list_item_struct new;
	struct ds_list_item_struct previous;
	long prevLoc = 0;
	int i;
	
	/*read the first long from the file*/
	ds_read(&previous.next, 0, sizeof(long));
	
	for(i = index; i > 0; i--)
	{
		if(previous.next == -1)
		{
			return -1;
		}
		else
		{
			prevLoc = previous.next;
			ds_read(&previous, previous.next, sizeof(struct ds_list_item_struct));
		}
	}
	
	/*write the new node and add it*/
	new.item = value;
	new.next = previous.next;
	previous.next = ds_malloc(sizeof(struct ds_list_item_struct));
	
	/*empty insertion*/
	if(prevLoc == 0)
	{
		ds_write(previous.next, &new, sizeof(struct ds_list_item_struct));
		
		/*change the header value*/
		ds_write(0, &previous.next, sizeof(long));
	}
	else
	{
	/*write both the old node and the new one*/
	ds_write(previous.next, &new, sizeof(struct ds_list_item_struct));
	ds_write(prevLoc, &previous, sizeof(struct ds_list_item_struct));
	
    }

	return 0;
}

/*this function removes the node at the given index*/
int ds_delete(long index)
{
	struct ds_list_item_struct cur;
	struct ds_list_item_struct previous;
	long prevLoc = 0;
	int i;
	
	/*read the first long from the file*/
	ds_read(&previous.next, 0, sizeof(long));

	/*deleting the first node*/
	if(index == 0)
	{
		/*save the old header*/
		prevLoc = previous.next;
		
		/*read the first node (the target)*/
		ds_read(&previous, prevLoc, sizeof(struct ds_list_item_struct));
		ds_free(prevLoc);
		
		ds_write(0, &previous.next, sizeof(long));
		return 0;
	}

    /*list traversal*/	
	for(i = index; i > 0; i--)
	{
		if(previous.next == -1)
		{
			return -1;
		}
		else
		{
			prevLoc = previous.next;
			ds_read(&previous, previous.next, sizeof(struct ds_list_item_struct));
		}
	}
	/*change the next pointers to remove the node*/
	ds_read(&cur, previous.next, sizeof(struct ds_list_item_struct));
	previous.next = cur.next;
	ds_free(prevLoc);
	ds_write(prevLoc, &previous, sizeof(struct ds_list_item_struct));
	return 0;
}

/*This is a helper function that gets the higher of two values*/
int getHigher(int val1, int val2)
{
	if(val1 > val2)
	{
		return val1;
	}
	else if(val2 > val1)
	{
		return val2;
	}
	else
	{
		return 0;
	}
}

/*This function swaps the items of nodes at the given locations*/
int ds_swap(long index1, long index2)
{	
	struct ds_list_item_struct cur;
	int start = 0;
	int temp1 = 0;
	int temp2 = 0;
	int found = 0;
	int i;
	
	/*read the first long from the file*/
	ds_read(&cur.next, 0, sizeof(long));
	
	if(index1 < 0 || index2 < 0)
	{
		return -1;
	}
	
	/*get the location to begin traversal*/
	start = getHigher(index1, index2);
	
	for(i = start; i >= 0; i--)
	{
		/*range checker*/
		if(cur.next == -1 || found == 2)
		{
			return -1;
		}
		else
		{
			/*traverse the list and search for the given indicies*/
			ds_read(&cur, cur.next, sizeof(struct ds_list_item_struct));
			if(start - i == index1)
			{
				temp1 = cur.item;
				found++;
		    }
		    if(start - i == index2)
		    {
				temp2 = cur.item;
				found++;
			}
		}
	}
	if(found == 2)
	{
		/*call replace to swap the values*/
	  ds_replace(temp2, index1);
	  ds_replace(temp1, index2);
	  return 0;
    }
	
	return -1;
}

/*This function searches the list for the given target*/
long ds_find(int target)
{
	long loc = 0;
	int i = 0;
	struct ds_list_item_struct cur;
	
	/*read the header*/
	ds_read(&loc, 0, sizeof(long));
	
	/*traverse the list*/
	do
	{
		ds_read(&cur, loc, sizeof(struct ds_list_item_struct));
		loc = cur.next;
		if(cur.item == target)
		{
			return i;
		}
		i++;
	}while(cur.next != -1);
	return -1;
}

/*This function reads integers from a text file and stores them in the order they were read*/
int ds_read_elements(char *filename)
{
	int val = 0;
	int counter = 0;
	long fileSize;
	FILE* fptr = fopen(filename, "r");
	
	/*get the length of the file*/
	fseek(fptr, 0, SEEK_END);
	fileSize = ftell(fptr);
	
    rewind(fptr);
	while(ftell(fptr) < fileSize)/*read the file*/
	{
		fscanf(fptr, "%d\n", &val);/*read the values and insert them into the array*/
		ds_insert(val, counter);
		counter++;
	}
		
	fclose(fptr);
	return 0;
}

/*This function calls ds_finish to write the list and closes the file*/
int ds_finish_list()
{
	if(ds_finish() != 0)
	{
		return -1;
	}
	return 0;
}
