/*Name: Alexander Chin
 * Student No. 1046121
 * achin@uoguelph.ca
 * CIS2520         
 * */

#include <stdio.h>
#include <stdlib.h>
#include "ds_memory.h"

/*global variables*/
struct ds_file_struct ds_file;
struct ds_counts_struct ds_counts;

/*This function initializes the block array as 
 * well as filling the allocated space with empty chars*/
int ds_create(char* filename, long size)
{
	FILE* fptr = fopen(filename, "wb+");
	
	int i;
	
	if(fptr)
	{
		/*set the values to the global variable*/
		ds_file.fp = fptr;
		
		/*first block*/
		ds_file.block[0].start = 0;
		ds_file.block[0].alloced = 0;
		ds_file.block[0].length = size;
		
		/*initialize the remainder of the array*/
		for(i = 1; i < MAX_BLOCKS - 1; i++)
		{
			ds_file.block[i].start = 0;
			ds_file.block[i].alloced = 0;
			ds_file.block[i].length = 0;
		}
		
		/*writes the blocks to the file*/
		if(fwrite(ds_file.block, sizeof(ds_file.block[0]), MAX_BLOCKS, fptr) != MAX_BLOCKS)
		{
			return 1;
		}
		
		/*write size bytes to file*/
		for(i = 0; i < size; i++)
		{
			fputc('0', fptr);
		}
		fclose(fptr);
		return 0;
	}
	return 1;
}

/*This function opens the file and reads in the block array*/
int ds_init (char* filename)
{
	/*set globals*/
	ds_file.fp = fopen(filename, "rb+");
	rewind(ds_file.fp);
	
	ds_counts.reads = 0;
	ds_counts.writes = 0;
	
	/*read blocks from file to memory*/
	if(fread(ds_file.block, sizeof(ds_file.block[0]), MAX_BLOCKS, ds_file.fp) != MAX_BLOCKS)
	{
		return 1;
	}
	
	return 0;
}

/*non required helper function*/
void ds_init_test()
{
	int i;
	for(i = 0 ; i < 15; i++)
	{
		printf("Block#	start	length	alloced\n");
		printf("%d	%lu	%lu	%d	\n", i, ds_file.block[i].start, ds_file.block[i].length, ds_file.block[i].alloced);
	}
}

/*This function looks for an open block in the array and sets the allocation accordingly*/
long ds_malloc(long amount)
{
	int i;
	long origLength, origStart;
	
	/*first search*/
	for(i = 0; i < MAX_BLOCKS; i++)
	{
		/*block found*/
		if(ds_file.block[i].length >= amount && ds_file.block[i].alloced == 0)
		{			
			origLength = ds_file.block[i].length; /*save the length for the next block*/
			origStart = ds_file.block[i].start;
			ds_file.block[i].length = amount;
			ds_file.block[i].alloced = 1;	
			
			/*second search*/
			for(i = 0; i < MAX_BLOCKS; i++)
			{
				if(ds_file.block[i].length == 0)
				{
					ds_file.block[i].length = origLength - amount;
					ds_file.block[i].start = origStart + amount;
					ds_file.block[i].alloced = 0;
					break;
				}
			}
			return origStart;
		}
	}
	return -1;
}

/*This function searches for the required block and sets its alloced to 0*/
void ds_free(long start)
{
	int i;
	
	for(i = 0; i < MAX_BLOCKS; i++)
	{
		if(ds_file.block[i].start == start)
		{
			ds_file.block[i].alloced = 0;	
		}
	}
}

/*This function reads the specified data from the file
 * at location start and amount bytes*/
void* ds_read(void *ptr, long start, long bytes)
{
	/*seek to the beginning of the file + the length of the header*/
	if(fseek(ds_file.fp, sizeof(ds_file.block) + start, SEEK_SET) !=0)
	{
		return NULL;
	}
	if(fread(ptr, 1, bytes, ds_file.fp) != bytes)
	{
		return NULL;
	}
	ds_counts.reads++;
	return ptr;
}

/*This function goes to the specified location and writes the given information
 * to the file*/
long ds_write(long start, void* ptr, long bytes)
{
	/*seek to the beginning of the file + the length of the header*/
	if(fseek(ds_file.fp, sizeof(ds_file.block) + start, SEEK_SET) != 0)
	{
		return -1;
	}
	
	/*write the data to the file*/
	if(fwrite(ptr, 1, bytes, ds_file.fp) != bytes)
	{
		return -1;
	}
	
	ds_counts.writes++;
	return start;
}

/*This function writes the block array to the file before closing it*/
int ds_finish()
{
	rewind(ds_file.fp);
	
	/*write blocks from file to memory*/
	if(fwrite(ds_file.block, sizeof(ds_file.block[0]), MAX_BLOCKS, ds_file.fp) != MAX_BLOCKS)
	{
		fclose(ds_file.fp);
		return 0;
	}
	
	printf("reads: %d\n", ds_counts.reads);
    printf("writes: %d\n", ds_counts.writes);
	fclose(ds_file.fp);
		
	return 1;
}
