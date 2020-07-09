#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// HW2 ->  HW3  ->  HW1
// LEX    parser    VM  ?
// this might need another step somewhere

/*
For constants: you must store kind, name and value.
For variables,: you must store kind, name, L and M.
For procedures: you must store kind, name, L and M.
*/
struct symbol
{
    int kind; 		// const = 1, var = 2, proc = 3
    char name[10];	// name up to 11 chars
	int val; 		// number (ASCII value)
	int level; 		// L level, in this project will basically always be 0
	int addr; 		// M address
	int mark;		// to indicate unavailable or delete d
}symbol;
int currAddress=4; // addresses here start at 4 in this project because we have 3 things in the AR already
int sizeOfSymbolTable = 1; // size of the symbol table currently, not the max size that's gonna be like 100 or something
                           // it starts at 1 because symbolTable starts at 1, if we ever it symbolTable[0] that means there
                           // is no match when we search
struct symbol symbolTable [100];


//----->here is some stuff for the code generation part of the project<-----//
typedef enum {
    LIT = 1, OPR = 2, LOD = 3, STO = 4, CAL = 5, INC = 6, JMP = 7, JPC = 8,
    STO1 = 9, STO2 = 10, STO3 = 11
}opcodes;
typedef enum{ // for when op=2
    RET = 0, NEG = 1, ADD = 2, SUB = 3, MUL = 4, DIV = 5, ODD = 6,
    MOD = 7, EQL = 8, NEQ = 9, LSS = 10, LEQ = 11, GTR = 12, GEQ = 13
}OPRCodes;
// given default values from HW1
#define MAX_DATA_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500
//struct of instructions
struct instruct{
    int OP; // opcode
    int  L; // L lexigraphical level
    int  M; // M
}instruct;

struct instruct Code[MAX_CODE_LENGTH];
int currentCodeIndex=0;
void emit(int op, int level, int address)
{
    if (currentCodeIndex > MAX_CODE_LENGTH)
    {
        //error(too much code);
    }
    else
    {
        Code[currentCodeIndex].OP = op;
        Code[currentCodeIndex].L = level;
        Code[currentCodeIndex].M = address;

        currentCodeIndex++
    }
}
//---->end of stuff for code generation part of the project<----//

//---->stuff from project 2<----//
#define MAX_DIGITS 5
#define MAX_ID_LENGTH 11
struct token{
    int ID;
    int value;
    char name[MAX_ID_LENGTH];
}token;

char * RESERVED_WORDS[] = {"const", "var", "procedure", "call", "if", "then", "else", "while", "do", "read", "write", "odd"};
char SPECIAL_SYMBOLS[] = {'+', '-', '*', '/', '(' , ')', '=', ',' , '.', '<', '>',  ';' , ':' };
char *LITERAL_ID[] = {"", " ", "id", "number", "+", "-", "*", "/", "odd", "=", "<>", "<", "<=", ">", ">=",
"(", ")", ",", ";", ".", ":=", "{", "}", "if", "then", "while", "do", "call", "const", "var", "procedure",
"write", "read", "else", ":"};

typedef enum {
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
multsym = 6,  slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
periodsym = 19, becomessym = 20, lbracesym = 21, rbracesym = 22, ifsym = 23, thensym = 24,
whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
readsym = 32, elsesym = 33, colonsym = 34
}token_type;
/* TEST:
var a, b;
{
	a:=b+1;
}.

Lexeme List
29  2 a  17  2 b  18  21  2 a  20  2 b  4  3  18  22  19

length: 15 (14 tokens + 1 ID=0)
*/
//----->end of stuff from project 2<-----//

// countTokens();
int numTokens; //probably need to run the token struct through a function that does counter++ until it hits ID=0 to get this number
int tokenIndex = 0; // this is the index for the token struct
struct token tokens[numTokens];
struct token currToken;

void getToken()
{
    if ( tokenIndex == numTokens )
        printf("no more tokens to get\n"); // this could be an error?
    else
    {
        currToken.ID = tokens[tokenIndex].ID;
        currToken.value = tokens[tokenIndex].value;
        strcpy(currToken.name, tokens[tokenIndex].name);
        tokenIndex++;
    }
}
// searches if an identifier is in the symbol table already, returns index if match found
int checkTable(struct token token)
{
    if (sizeOfSymbolTable == 1)
        return 0; // there isn't even anything in the table yet

    // search the symbol table for token.name
    for (int i = sizeOfSymbolTable ; i > 0 ; i-- )
    {
        if ( strcmp(token.name, symbolTable[i].name) == 0 ) // if a match is found
        {
            return i;
        }
    }
    return 0; // no match has been found
}
void block()
{
    // all the inputs from HW1 start with Jump to instruction 0
    emit(JMP, 0, 0);

    // TA says "keep track of the number of variables"
    int numVars = 0;

    if ( currToken.ID == constsym ) // check for a constant declaration
        do  // we have at lest 1 constant declaration, so do it at lest once
        {
            getToken(); // *if theres a problem around here we, I think could try moving getToken outside of the do/while loop*
            // this next token must be an identifier
            if ( currToken.ID != identsym )
            {
                // error();
            }
            // we check if this identifier is in the symbol table already

            // checkTable will take in the current token and check it against the symbol table, returns index if it is found,
            // returns 0 if no match found
            if ( checkTable(currToken) != 0 )
            {
                 //error(); // duplicate identifier!
            }
            else
            {
                // this is a new identifier!
                strcpy( symbolTable[sizeOfSymbolTable].name, currToken.name );
                symbolTable[sizeOfSymbolTable].kind = 1; // (kind 1 = const)
            }

            // ok now that the table knows the name of this identifier, we update token again
            getToken();

            // the next token *has* to be an equals sign
            if ( currToken.ID != eqsym )
            {
                // error();
            }

            // continue if it was an equal sign
            getToken();

            // this next token *has* to be a digit
            if ( currToken.ID != numbersym )
            {
                // error();
            }
            // if it is a digit, we can input our new const's value into the symbol table
            symbolTable[sizeOfSymbolTable].value = currToken.value;
            // there is no level, address, or mark to store in the symbol table for this one so
            // we can just go ahead and officially say the symbol table is bigger
            sizeOfSymbolTable++;

            // ok, next token
            getToken();
            // if its a comma it'll start this loop again, if not we must check for a semicolon
        }
        while (currToken.ID == commasym);//there could be multiple declared
        if ( currToken != semicolonsym )// constant declarations *have* to end with a semicolon
        {
            //error();
        }
        // if it does end in a semicolon, we can move on
        getToken();
//-----------------VARSYM------------------VARSYM--------------------VARSYM---------------------
    if ( currToken.ID == varsym ) // check for a variable declaration
        numVars++;
        do
        {
            getToken();
            // following a var, we must have an identifier symbol
            if ( currToken != identsym )
            {
                // error();
            }
            // if it is an identifier, we check if one such exists in the symbol table already
            if ( checkTable(currToken) != 0 )
            {
                // error(); // variable with that name already exists!
            }
            // if no variable with that name exists, we add it to the table:
            strcpy( symbolTable[sizeOfSymbolTable], currToken.name );
            symbolTable[sizeOfSymbolTable].kind = 2; // (kind 2 = variable)
            symbolTable[sizeOfSymbolTable].address = currAddress;
            currAddress++; // increment current address
            // there is no value to input just yet, there is no level to input, no mark either so
            // we can officially grow the symbol table
            sizeOfSymbolTable++;

        }
        while (currToken.ID == commasym)// similarly to above, there could be multiple variables declared
        if ( currToken != semicolonsym )// variable declarations *have* to end with a semicolon
        {
            //error();
        }
        // if it does end in a semicolon, we can move on
        getToken();

    // after const and vars we increment the stack pointer depending on how many vars we put i think?
    emit(INC, 0, currAddress);


    statement();
}

void statement()
{

}

int main()
{
    /*
    notes from looking over the instructions and watching the TA's video:

        *a big difference between project 2 and project 3 is that there are no pros in
            project 3 (thank goodness! basically, no functions)

        *another difference is that the open brackets and closing brackets+period becoming
            begin and end

        *for project 1 and 2 we really only made 1 c file, but here I think we should make a driver and also a parser
            c files. like this might not continue to be called "main.c"
        *the driver will need main(argc, **argv) where argv is and array of strings that gets input in the command
            line and argc is the number of strings
        *maybe the parser should be stuck into the end of the lexical analyzer so we don't have to pass the tokens??


    more notes from reviewing the 7/6 lecture:
        *Inside const-declaration  and/or var-declaration is where we populate the symbol table
        *Once the symbol table has been populated, you can move on to the "statement" part of the grammar
        *Once again: no procedures! 50 pts off if we have procedures! even if commented out?
        *It's "if and then" not "if and else"
        *He says to have an error function that takes in an int that corresponds to which error it is
        *One of the test cases they're gonna use is   procedure := else * call   to check that our program must
            take these are identifiers not actual procedure calls!
        *Every time you search a symbol table, keep the thing you're searching for in i=0
        *I think the addresses in the symbol table start at 4 because we had 3 things in the AR
        *We don't do any arithmetic in the symbol table
        *math is done in the code generation ie
            a:=b+c ==> LOD 0,5
                       LOD 0,6
                       ADD
                       STO 0,4
        *in this project, L is always 0 because we have no procedures/
        *
    */



}
