/* 

Copyright (c) 2012 Marek Kudla.

MIT License

counts existing kmers in fasta file 

Program is complete - works
Let's call it version 0.3

This will be my basic framework for bioinformatics studies and other programs.

Requires code cleanup

synopsis:
kmer_search -i inputfastafilename.fa -o outputFileName -m -k <kmerLength>
Program goes through the input fasta file and counts kmers of length specified by -k <kmerLength>.
Output goes to the outputFileName and is a tab delimited table of: position	kmer_string	count
If optional -m parameter is specified then multiplicity is taken into account. Fasta file name string has to have the multiplicity information in the form of: >idnumber-multiplicitynumber
There are strong limits on fasta string length, in no situation exceed 1024 character (to be fixed).
Filenames cannot be longer than 255 letters.
Only ACTG allowed !!!
Fasta file has to be properly formatted:
>3-12
ACCACACA
>4-4343
CCGAGCGACGA
>5-77
CCACCCACACCACACA

*/

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define DEBUGMODE
#define SHOWPARAMS

//global variables 
//                 CAUTION!!
	int multiplicates = FALSE;
	char inputFileName[255];	//SEVERE WARNING HERE - BUFFER OVERFLOW POSSIBILITY
	char outputFileName[255];	//SEVERE WARNING HERE - BUFFER OVERFLOW POSSIBILITY
	FILE *INPUT, *OUTPUT;
	int kmerLength = -1;
	int stopProgram = FALSE;
//global variables end here



int main (int argc, char *argv[])
{

	strcpy(inputFileName, "dummy1");	// failsafe for uninitialized strings
	strcpy(outputFileName, "dummy2");

	fGetProgramOptions(argc, argv);

	#ifdef SHOWPARAMS
	fParametersprint();
	#endif

	stopProgram = fParametersChech();

	if (stopProgram == FALSE) {

		#ifdef DEBUGMODE
		printf("Parameters passed \n");
		#endif

		int scoreTableLength = pow(4, kmerLength);
		int scoreTable[scoreTableLength];

		#ifdef DEBUGMODE
		printf("kmer length %i score table length (should be 4^kmerLength) %i \n",kmerLength, scoreTableLength);
		#endif

		for (int i=0;i<scoreTableLength;i++) //4^kmerLength
		{
			scoreTable[i] = 0; // fill it with zeros this is crucial
		}
		
		// fill in with function calls : open files, 
		INPUT = fopen(inputFileName, "r");
		char stringBuffer[1024]; // hope it will suffice, use with fgets and a limiter
					 // OK, something has to be done here 1024 is nowhere enough for big enough fasta files
		
		long multiplicateNumber = 1;
		while(fgets(stringBuffer, 1024, INPUT)) // main loop - fill condition dependent on line read
		{
			if(stringBuffer[0] == '>') 
			{

				if(multiplicates == TRUE)
				{

					char * pTokenString;
					pTokenString = strtok(stringBuffer, "-");
					if(pTokenString != NULL)
					{
						pTokenString = strtok(NULL, "-");
						multiplicateNumber = atoi(pTokenString);
							#ifdef DEBUGMODE
							printf("Here is the calculated number: %d \n", multiplicateNumber);
							#endif

					}

				}

			}
			else 
			{
				for(int i=0;i<strlen(stringBuffer)-kmerLength; i++)
				{
					int offsetForCounttable = 0;
					for(int j = 0; j<kmerLength; j++)
					{
						int whichValue = 0;
						switch(stringBuffer[i+j])
						{
							case 'a':
							case 'A':
								whichValue=0;
								break;
							case 'c':
							case 'C':
								whichValue=1;					
								break;
							case 'g':
							case 'G':
								whichValue=2;
								break;
							case 't':
							case 'T':
								whichValue=3;
								break;
						}
						offsetForCounttable += pow(4,j) * whichValue;
					}
					scoreTable[offsetForCounttable]+= multiplicateNumber;	
				}

			}

			#ifdef DEBUGMODE
			printf("String: %s", stringBuffer);
			#endif
			
			
		}
		
		
		tableprint(scoreTable, scoreTableLength, outputFileName);
		fclose(INPUT);
		fclose(OUTPUT);
		return 0;
	}
	else
	{
		printf("Parameters failed, exiting \n");
		return 1;
	}

}


void fGetProgramOptions(int argc, char *argv[]) 
{
	int opt;
	while ((opt=getopt(argc, argv, "i:o:k:mh"))!=-1) 
	{

		switch (opt) 
		{
		case 'i':
			strcpy(inputFileName, optarg);
			break;
		
		case 'o':
			strcpy(outputFileName, optarg);
			break;

		case 'k':
			kmerLength = atoi(optarg);
			break;		

		case 'm':
			multiplicates = TRUE;
			break;
		
		case 'h':
			printf("synopsis: \n \
kmer_search -i inputfastafilename.fa -o outputFileName -m -k <kmerLength> \n \
Program goes through the input fasta file and counts kmers of length specified by -k <kmerLength>. \n \
Output goes to the outputFileName and is a tab delimited table of: kmer_index	kmer_string	count \n \
If optional -m parameter is specified then multiplicity is taken into account. \n \
Fasta file name string has to have the multiplicity information in the form of: >idnumber-multiplicitynumber \n \
There are strong limits on fasta string length, in no situation exceed 1024 character (to be fixed). \n \
Filenames cannot be longer than 255 letters. \n ");
			break;

		case ':':
			printf("Required parameter not specified. \n");
			break;

		case '?':
			printf("Unknown parameter. %c\n", optopt);
			break;
		}
	}
}

void fParametersprint()
{
	printf ("Parameter i: %s\n",inputFileName);
	printf ("Parameter o: %s\n",outputFileName);
	printf ("Parameter k: %i\n",kmerLength);
	printf ("Parameter m: %i\n",multiplicates);
}

int fParametersChech()
{
	if(kmerLength>0 && kmerLength<10)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void tableprint (int * scoreTable, int scoreTableLength, char * outputFileName)
{
	OUTPUT = fopen(outputFileName, "w");
	for (int i=0;i<scoreTableLength;i++)
	{
		char temp_string[32];
		int current_digit;
		int copy_of_i = i;
		temp_string[kmerLength] = '\0';		
		for(int m=kmerLength-1; m+1>0;m--) {
			current_digit = copy_of_i / pow(4, m);
			switch(current_digit)
			{
				case 0:
					temp_string[m] = 'A';
					break;
				case 1:
					temp_string[m] = 'C';
					break;
				case 2:
					temp_string[m] = 'G';
					break;
				case 3:
					temp_string[m] = 'T';
					break;
			}
			copy_of_i -= current_digit * pow(4,m);
		}
		fprintf(OUTPUT, "%i\t%s\t%i\n", i,temp_string ,scoreTable[i]);
	}
}


