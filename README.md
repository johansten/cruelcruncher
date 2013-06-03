
# CruelCruncher

From deep down the lost and found bin.

This way originally the result of trying to create a State-of-the-Art executable
packer for the Amiga, after reading about the Burrows-Wheeler transform somewhere
around the middle of 1996. Rewritten in C/C++/x86 asm for Windows in 1998.

It replicates the results from Peter Fenwick's papers from early 1996 on the BWT,
and uses a BWT with a Move-to-Front stage and arithmetic coding (using the
"Structured model" from Fenwick.)

For the arithmetic coder there is an added optimization stage that takes place
to find the parameters for adjusting the statistical models. This is done
using a search over the parameter space with a "dummy"-encoder that return the
total number of bits that would have been encoded.

The name is of course a tribute to the Commodore C64-cruncher made by
Galleon/Oneway.

# Example usage - compression

	FileInputBuffer input("input.dat");
	FileOutputStream output("output.dat");
	compress(input, output);

# Example usage - decompression

## From file to file

	FileInputStream input("input.dat");
	FileOutputBuffer output("output.txt");
	decompress(input, output);

## When you need the output to go somewhere specific in memory

	FileInputStream input("input.dat");

	header h;
	readHeader(input, h);
	uint8* pBuffer = new uint8[h.fileSize];

	MemoryOutputBuffer output(pBuffer, h.fileSize);
	decompress(input, output, &h);

## When you need the output in memory, but don't care much for where

	FileInputStream input("input.dat");
	MemoryOutputBuffer output();
	decompress(input, output);

## From a specific location in a FILE*, to a specific memory location

	fseek(pFile, position, SEEK_SET);
	FileInputStream input(pFile, size);
	MemoryOutputBuffer output(pBuffer, size);
	decompress(input, output);
