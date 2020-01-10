/*Name: Alexander Chin
 * Student No. 1046121
 * achin@uoguelph.ca
 * CIS2520         
 * */

#include "ds_memory.h"
#include "ds_array.h"

/*global variable*/
long elements;

/*This function creates the array on memory and prepares the empty array*/
int ds_create_array()
{
	long headLong = 0;
	ds_init("array.bin");
	
	/*allocate space for a long then write zero to top of file*/
	ds_malloc(sizeof(long));
	ds_write(0, &headLong, sizeof(headLong));
	
	/*allocate memory for the rest of the array*/
	ds_malloc(MAX_ELEMENTS * sizeof(int));
	
	if(ds_finish() != 0)
	{
		return -1;
	}
	return 0;
}

/*This function loads the array on disk*/
int ds_init_array()
{
	ds_init("array.bin");
	ds_read(&elements, 0, sizeof(elements));	
	
	return 0;
}

/*The is a helper function that display info about the array*/
void show_array()
{
	int i;
	int cur = 0;
	ds_init_test("array.bin");
	
	/*prints out the array elements*/
	for(i = 0; i < elements; i++)
	{
		ds_read(&cur, sizeof(long) + i*sizeof(int), sizeof(int));
	}
}

/*This function searches the array and replaces a target index with the value passed*/
int ds_replace(int value, long index)
{
	/*range checker*/
	if(index < 0 || index > MAX_ELEMENTS)
	{
		return -1;
	}
	
	ds_write(index*sizeof(int) + sizeof(long), &value, sizeof(value));
	
	return 0;
}

/*This function places a value into an array, pushing the rest values to the next index*/
int ds_insert(int value, long index)
{
	int new = value;
	int old;
	int i;
	int endex;
	
	/*range checking*/
	if(index < 0 || index > MAX_ELEMENTS)
	{
		return -1;
	}
	
	/*handles insertion for an empty array*/
	if(index == elements)
	{
		ds_write(index*(sizeof(int)) + sizeof(long), &new, sizeof(new));
		elements++;
		return 0;
	}
	else
	{
		/*loop from insertion point to the end of the array*/
	    for(i = index; i < elements; i++)
	    { 
		  endex = index*(sizeof(int)) + sizeof(long);
		  ds_read(&old, endex, sizeof(old));/*save the old value*/
		  ds_write(endex, &new, sizeof(new));/*write in new value*/
	      new = old;/*continue*/
	      index++;
    	}
    	/*increment elements and writes the final element*/
    	elements++;
    	ds_write(index*(sizeof(int)) + sizeof(long), &new, sizeof(new));
    }
	return 0;
}

/*This function removes an element from the array and pushes the remaining values back*/
int ds_delete(long index)
{	
	int i;
	int next = 0;
	int cur;
	int nextAddress;
	int curAddress;
	
	/*loops from deleted element to the end of the array*/
	for(i = index; i < elements - 1; i++)
	{ 
      nextAddress = (index+1)*(sizeof(int)) + sizeof(long);/*save the addresses of the current and next element*/
      curAddress = index*(sizeof(int)) + sizeof(long);
	  ds_read(&next, nextAddress, sizeof(next));/*save the next address*/
	  ds_write(curAddress, &next, sizeof(cur));/*write the next address into the current index*/
	  index++;
    }
    /*decrement number of elements*/
	elements--;
	
	return 0;
}

/*This function swaps the elements at the given indices*/
int ds_swap(long index1, long index2)
{
	int element1;
	int element2;
	
	/*check for valid indices*/
	if(index1 < 0 || index1 >= elements || index2 < 0 || index2 >= elements)
	{
		return -1;	
	}
	/*convert indices into coorresponding addresses*/
	index1 = (index1)*(sizeof(int))+sizeof(long);
    index2 = (index2)*(sizeof(int))+sizeof(long);

    /*read in the values of the elements*/
	ds_read(&element1, index1, sizeof(element1));
	ds_read(&element2, index2, sizeof(element2));

    /*perform the swap using the saved addresses and element values*/
	ds_write(index1, &element2, sizeof(element2));
	ds_write(index2, &element1, sizeof(element1));
	
	return 0;
}

/*This function searches for the given element and returns 0 upon a successful find*/
long ds_find(int target)
{
	int i;
	int cur = 0;
	
	/*loop through the elements*/
	for(i = 0; i < elements; i++)
	{
		/*read the current element*/
		ds_read(&cur, sizeof(long) + i*sizeof(int), sizeof(int));
		if(cur == target)
		{
			return 0;
		}
	}
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

/*This function writes the element counter before writing the array to disk*/
int ds_finish_array()
{
	ds_write(0, &elements, sizeof(elements));
	if(ds_finish() == 0)
	{
		return 0;
	}
	return 1;
}
