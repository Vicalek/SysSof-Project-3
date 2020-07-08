#include <stdio.h>
#include <stdlib.h>
#indlude <ctype.h>


// HW2 ->  HW3  ->  HW1
// LEX    parser    VM  ?
// this might need another step somewhere

/*
For constants: you must store kind, name and value.
For variables,: you must store kind, name, L and M.
For procedures: you must store kind, name, L and M.
*/
typedef symbol
{
    int kind; 		// const = 1, var = 2, proc = 3
    char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value)
	int level; 		// L level
	int addr; 		// M address
	int mark		// to indicate unavailable or delete d
} symbol;




int main()
{
    /*
    notes from looking over the instructions and watching the TA's video, I am still not 100% confidant in what it wants so we have to go over that proto

        a big difference between project 2 and project 3 is that there are no pros in
        project 3 (thank goodness! basically, no functions)

        another difference is that the open brackets and closing brackets+period becoming
        begin and end

        for project 1 and 2 we really only made 1 c file, but here I think we should make a driver and also a parser
        c files. like this might not continue to be called "main.c"
        the driver will need main(argc, **argv) where argv is and array of strings that gets input in the command
        line and argc is the number of strings
        maybe the parser should be stuck into the end of the lexical analyzer so we don't have to pass the tokens??
    */



}
