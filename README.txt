                Vaccine Monitor
               -----------------
            Pavlos Smith - sdi1800181



Compilation
------------

make



Input file generation
----------------------

./testFile.sh [viruses file] [countries file] [numOfEntries] [duplicates allowed]


example:

./testFile.sh tests/viruses.txt tests/countries.txt 10000 0




Execution
----------

./vaccineMonitor -c [input file] -b [bloom filter size]


example:

./vaccineMonitor -c inputFiles/inputFile.txt -b 5000




Commands
---------

Use /help to view all commands along with their syntax
and a short description.




Invalid input checking
-----------------------




Data Structures
----------------

I use dynamic hashtables to keep countries, viruses and citizen records.
Buckets are implemented with linked lists.

Memory for strings is allocated once and only references to that memory
is stored in other structures. For example, citizen names are stored in the
struct of the person and pointers to that person are kept in the citizen hashtable.

A virus struct stores its name, its skiplists and its bloomfilter.

A country struct stores its name, its population, the populations of each age group specifically,
a counter to be used for total population percentages, and an array of counters to be used
for age-group specific percentages.

A person struct stores his/her ID, names, country pointer, and age.

A vaccine record struct stores pointers to a person and a date.
Vaccine records are passed as value to the skiplists for quick
access to vaccination dates.

Skiplists are used for vaccinated and non-vaccinated citizens for
each virus.

Bloomfilters are also used for vaccinations for quick checking
of non-vaccinated people.



Bloom Filters
--------------

Bloom filters are implemented with a char array and masks
with logical shifts for setting bits and checking them.



Skip Lists
-----------

Skip lists are implemented with a struct that contains
the max level of the skip list, the current level,
the probability of creating a new level for a new node
and a pointer to a dummy skip node.

Each skip node contains a key and a generic type item as
well as an array of skip nodes for every level in front
of it.



Known Issues
-------------

If you give incorrect input, small memory leaks are created.
This is due to the way that lines are parsed; words are split
and memory is allocated in between, so freeing the memory after
every error would make the code much larger and unreadable than it
should be. A fix to this would be splitting all the words from the
beggining, checking for errors and then doing the necessary processing,
allowing easy and compact freeing from the beggining.
