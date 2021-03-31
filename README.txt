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



Assumptions
------------

In vaccineStatus, if a citizen is not in the
vaccinated skiplist, they are considered unvaccinated.

In list-nonVaccinated-Persons, only citizens that
are recorded to be non-vaccinated are shown.

The thought behind this is that, if used for a
large dataset and you use list-nonVaccinated-Persons,
you only want to know who needs to be vaccinated without
question. On the contrary, vaccineStatus seems like it would
be simpler to assume that someone is not vaccinated if they are
inexistant in the records, because it would be extremely hard
to keep track of every vaccine that hasn't been done for every
citizen.

That way, a citizen is recorded as not vaccinated when they're
aiming to be vaccinated in the future and they show up in
list-nonVaccinated-Persons without too much unneeded information.



Invalid input checking
-----------------------

When a command is parsed, I check whether there are enough
arguments and if they are valid. If the arguments weren't
enough, or at least one of them was invalid, an error
message is shown.

Invalid arguments are any types of problematic input
regarding their type. For example, the format of dates
is checked so that an argument like "01//" is invalid.

Another example is whether countries or viruses exist
in the database or not. If they aren't and a query
depends on them already existing, an error message is shown.
For instance, popStatusByAge can't give any statistics if
the virus is not already known, because we have no
information about who is vaccinated or not.




Age population statistics
--------------------------

Ages are grouped as following:

0-19
20-39
40-59
60+

It's different to the requsted output
but with this grouping there are no
common sub-sections between the groups.
Also the groups are evenly sized, except 60+.



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
