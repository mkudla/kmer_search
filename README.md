# kmer_search

counts existing kmers in fasta file 


## synopsis

kmer_search -i inputfastafilename.fa -o outputFileName -m -k <kmerLength>

Program goes through the input fasta file and counts kmers of length specified by -k <kmerLength>.

Output goes to the outputFileName and is a tab delimited table of:

position	kmer_string	count

If optional -m parameter is specified then multiplicity is taken into account. Fasta file name string has to have the multiplicity information in the form of: >idnumber-multiplicitynumber


## notes

Program is complete - it works, let's call it version 0.3

Requires some code cleanup and foolproofing.

* There are strong limits on fasta string length, in no situation exceed 1024 character (to be fixed).

* Filenames cannot be longer than 255 letters.

* Only ACTG allowed in sequence !!!

* Fasta file has to be properly formatted, like this:

\>3-12
ACCACACA
\>4-4343
CCGAGCGACGA
\>5-77
CCACCCACACCACACA


