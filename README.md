# Word-Chain-Generator
The goal of this program is to create a chain of real words (those listed in a wordlist.txt) from the one word to another. Each node in the chain has to be a transformation of the previous. There are 4 transformations allowed in this program:

Letter replacement. Example: from READING to REAPING
Letter insertion. Example: from BEND to BLEND
Letter removal. Example: from HORSE to HOSE
Swapping adjacent letters. Example: from AMPLE to MAPLE

For example, suppose the origin word is SWIM and the destination word is TIDE. One possible chain is: swim - slim - slid - slide - side - tide, where transformations (1) (1) (2) (3) (1) are used in that order.

A chain between two words (if any) are usually not unique. For example, from BEND to BLEND the chain can be: bend - blend or bend - bind - blind - blend. In that case, the program IS GUARANTEED to return the shortest chain, which is the former. The web data structure and breadth-first search were used to ensure this.

The word list used in this program has over 60,000 words. I utilized and implemented several abstract data types and algorithms to efficiently navigate big data and search:

Harsh table
Linked lists
Webs
Stack
Breadth-first search algorithm
