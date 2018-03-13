
// --------------------------------------------------- //
// julia drahozal (2441134)
// eecs 645
// homework 4 : cache simulator
//
// cache.cpp
// simulate function of an L1 cache
// --------------------------------------------------- //

// includes
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// --------------------------------------------------- //
// set up
// --------------------------------------------------- //

// constants
#define cache_size_exp 		15
#define cache_size_num 		(1 << cache_size_exp)
#define addr_size_exp 		32
#define addr_size			(addr_size_exp >> 3)
#define cache_assoc_exp 	3
#define cache_assoc 		(1 << cache_assoc_exp)
#define block_size_exp 		6
#define block_size_num 		(1 << block_size_exp)
#define block_size_mask 	(block_size_num - 1)
#define lines_exp 			(cache_size_exp - cache_assoc_exp - block_size_exp)
#define lines_num 			(1 << lines_exp)
#define lines_mask 			(lines_num - 1)
#define tag_exp 			(addr_size_exp - block_size_exp - lines_exp)
#define tag_num 			(1 << tag_exp)
#define tag_mask			(tag_num - 1)

// block struct
typedef struct entry
{
	int tag = -1;
	bool is_valid = false;
} Entry;

// cache
Entry cache[lines_num][cache_assoc] = {};

// variables
int hits = 0;
int misses = 0;

// --------------------------------------------------- //
// helper functions
// --------------------------------------------------- //

void reportParameters (char * file_name)
{
	printf ("\n------------- FILE --------------\n");
	printf ("Filename: %s\n", file_name);

	printf ("---------- PARAMETERS -----------\n");
	printf ("Cache Associativity:\t\t%08X\n", cache_assoc);
	printf ("Cache Size Parameters:\t\texp: %08X; num: %08X\n",
				cache_size_exp, cache_size_num);
	printf ("Address Size Parameters:\texp: %08X\n", addr_size_exp);
	printf ("Block Size Parameters:\t\texp: %08X; num: %08X; mask: %08X\n",
				block_size_exp, block_size_num, block_size_mask);
	printf ("Line Parameters:\t\texp: %08X; num: %08X; mask: %08X\n",
				lines_exp, lines_num, lines_mask);
	printf ("Tag Parameters:\t\t\texp: %08X; num: %08X; mask: %08X\n",
				tag_exp, tag_num, tag_mask);
}

void reportResults ()
{
	printf ("------------ RESULTS ------------\n");
	printf ("Addresses Processed:\t%d\n", (hits + misses));
	printf ("Hits:\t\t\t%d\n", hits);
	printf ("Misses:\t\t\t%d\n", misses);
	printf ("Hit Ratio:\t\t%.2f%%\n", (((double) hits) / (hits + misses)) * 100);
	printf ("---------------------------------\n\n");
}

int getLine (int addr)
{
	return ((addr >> block_size_exp) & lines_mask);
}

int getTag (int addr)
{
	return ((addr >> (block_size_exp + lines_exp)) & tag_mask);
}

// --------------------------------------------------- //
// main function
// --------------------------------------------------- //

int main (int argc, char ** argv)
{
	// check arguments
	if (argc != 2)
	{
		printf ("Use: ./cache <filename>\n");
		return 1;
	}

	// open file
	char * file_name = argv[1];
	FILE * file = fopen (file_name, "rb");

	// print parameters
	reportParameters (file_name);

	// read file
	int curr_block = 0;
	int addr;
	while (fread (&addr, addr_size, 1, file) != 0)
	{
		// get info
		int line = getLine (addr);
		int tag = getTag (addr);

		// search cache
		bool hit = false;
		for (int block = 0; block < cache_assoc; block++)
		{
			// hit
			if (cache[line][block].is_valid && cache[line][block].tag == tag)
			{
				hit = true;
				break;
			}
		}

		// cache hit
		if (hit)
		{
			hits++;
		}

		// cache miss
		else
		{
			misses++;

			// search for empty block
			bool stored = false;
			for (int block = 0; block < cache_assoc; block++)
			{
				// found empty block
				if (!cache[line][block].is_valid)
				{
					cache[line][block].tag = tag;
					cache[line][block].is_valid = true;
					stored = true;
					break;
				}
			}

			// round robin replacement
			if (!stored)
			{
				cache[line][curr_block].tag = tag;
				cache[line][curr_block].is_valid = true;
				curr_block = (curr_block + 1) % cache_assoc;
			}
		}
	}

	// print results
	reportResults ();

	// close file
	fclose (file);
	return 0;
}
