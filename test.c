#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>  
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

typedef enum token_type {
	periodsym = 1, varsym, lbracketsym, procsym, rbracketsym, 
	callsym, commasym, beginsym, semicolonsym, endsym, 
	assignsym, ifsym, questionsym, dosym, colonsym, 
	readsym, lparenthesissym, writesym, rparenthesissym, identsym, 
	eqlsym, numbersym, neqsym, modsym, lsssym, 
	divsym, leqsym, multsym, gtrsym, subsym, 
	geqsym, addsym, whilesym
} token_type;

typedef struct lexeme {
	char name[12];
	int value;
	token_type type;
} lexeme;

// Must be global. 
lexeme *list;
int lex_index;
int runningInts = 0;
int in_index;

lexeme *lexanalyzer(char *input, int printFlag);

void alphatoken(char* input);
void numbertoken(char* input);
int symboltoken(char* input);
int issymbol(char c);
int isspecialsymbol(char* symbol);
void comment(char* input);
int reservedcheck(char *buffer);
void printlexerror(int type);
void printtokens();
int pow(int b, int exp);

// When goin thru lex, check if char is a letter, symbol, and number. 

// Here lies our project overview.
	// Required error handling.
		// 1. Invalid Identifier (identifiers should not begin with digits) 
		// 2. Number Length (at most 5 digits) 
		// 3. Identifier Length (at most 11 letters or digits) 
		// 4. Invalid Symbol 
	// Lexeme returns NULL if there is an error.
	// . The  primary  function marks  the  end  of  the  lexeme  list  before  returning. 
	

// Implement this function. Char *input will have the character input string. We will 
// probably loop through this until we reach a pointer to NULL. 
int main() {
  char* input = "var x, [3]; \n procedure func \n// this program should generate a parser error \nif <> ? problem : bueno \ny(2) := 4 * 8 % 11 \nend \nbegin \ncall \ndo \nwhile > < == candy : /**/ \nread write <===>=-+ \n.\0";
  lexeme* list = lexanalyzer(input,1);
}


lexeme *lexanalyzer(char *input, int printFlag)
{
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	lex_index = 0;
  in_index = 0;

  while (input[in_index] != '\0'){
    if(!iscntrl(input[in_index])){
      if(isalpha(input[in_index])){
        alphatoken(input);
      } else
      if(isdigit(input[in_index])){
        numbertoken(input); 
      } else
      if(issymbol(input[in_index])){ 
        if(symboltoken(input))
          comment(input);
      }
    }
    in_index++;
  }
  
  if (printFlag)
		printtokens();
	list[lex_index++].type = -1;
  
	return list;
}

void alphatoken(char* input){
  int index_copy = in_index;
  bool error = false;
  char buffer[12] = "";
    // loop through input until a non-alphanumeric character is found OR the 12th alphanumeric character is reached
    while(isalpha(input[index_copy]) || isdigit(input[index_copy])){
      // if the 12th alphanumeric character is reached, print identifier length error
      if(index_copy - in_index >= 11){
        // IDENTIFIER LENGTH ERROR
        error = true;
        printlexerror(3);
        break;
      }
      buffer[index_copy-in_index] = input[index_copy];
      index_copy+=1;
    }
  // if non-alphanumeric character is reached, check for reserved word then move on to next token
  if(error==false){
    int res = reservedcheck(buffer);
    if(res!=-1){
      token_type t = res;
      list[lex_index].type = t;
      strcpy(list[lex_index].name,buffer);
      
    } else {
      list[lex_index].type = identsym;
      strcpy(list[lex_index].name,buffer);
    }
    lex_index++;
    in_index = index_copy-1;
  }
        
}

// Check for reserved words
int reservedcheck(char *buffer){
  char *reserved[10] = 
  {  
    "var",
    "procedure",
    "call",
    "begin",
    "end",
    "if",
    "do",
    "while",
    "read",
    "write"
  };
  int reservedvalue = -1;

    if(strcmp(buffer, reserved[0]) == 0)
    {
      reservedvalue = 2;
    }
    else if(strcmp(buffer, reserved[1]) == 0)
    {
      reservedvalue = 4;
    }
    else if(strcmp(buffer, reserved[2]) == 0)
    {
      reservedvalue = 6;
    }
    else if(strcmp(buffer, reserved[3]) == 0)
    {
      reservedvalue = 8;
    }
    else if(strcmp(buffer, reserved[4]) == 0)
    {
      reservedvalue = 10;
    }
    else if(strcmp(buffer, reserved[5]) == 0)
    {
      reservedvalue = 12;
    }
    else if(strcmp(buffer, reserved[6]) == 0)
    {
      reservedvalue = 14;
    }
    else if(strcmp(buffer, reserved[7]) == 0)
    {
      reservedvalue = 33;
    }
    else if(strcmp(buffer, reserved[8]) == 0)
    {
      reservedvalue = 16;
    }
    else if(strcmp(buffer, reserved[9]) == 0)
    {
      reservedvalue = 18;
    }
  
  return reservedvalue;
}

// Code ref matt
// void fetch_cycle(instruction code, IR *IR){
//     IR->opCode = code.opcode; 
//     IR->r = code.r; 
//     IR->l = code.l;
//     IR->m = code.m;
// }
// Reference
// 	typedef struct lexeme {
// 	char name[12];
// 	int value;
// 	token_type type;
// } lexeme;

// Check if the first index of the input is an integer. If so return error. 
// Might have ot check if 5 consecutive digits. 
void numbertoken(char* input)
{
	if(in_index == 0)
	{
		//printf(stderr);
		printf("Code: Input begins with integer"); 
    list = NULL; 
		exit(0);     //terminates program
	}
  int digit[5];
  int i=0;
  while (isdigit(input[in_index])) {
    if(i==5) {
      //exceeded max number of digits
      printlexerror(2);
      list = NULL;
      exit(0);
    }
    digit[i] = input[in_index]-'0';
    in_index++;
    i++;
  }
  if (isalpha(input[in_index])) {
    //number followed by letter no whitespace
    printlexerror(1);
    list = NULL;
    exit(0);
  }
  in_index--;
  
  int num = 0;
  for (int j=0;j<i;j++) {
    num += digit[j] * pow(10,i-j-1);
  }
	
	//*list[in_index].name = ;
	list[lex_index].type = numbersym; 
	list[lex_index].value = num;
  lex_index++; 
}

int pow(int b, int exp) {
  if (exp==0) return 1;
  return b * pow(b,exp-1);
}

int symboltoken(char* input) {
  //create temp index for char array
  int tempidx = 0;
  //create char array to store symbol token
  char buffer[3];
  //if looking at a symbol or white space, look for next symbol
  while (issymbol(input[in_index])||iscntrl(input[in_index])||isspace(input[in_index])) {
    //if looking at symbol, add to buffer
    if (issymbol(input[in_index])) {
      buffer[tempidx++] = input[in_index];
    }
    //increment input index
    in_index++;
    //if tempidx is 2, break since longest symbol is 2 characters
    if (tempidx==2) 
      break;
  }
  //save next non space character
  tempidx = in_index;
  //decrement in_index to just before next character
  in_index--;
  //turn buffer into a string
  buffer[2] = '\0';
  //find out if buffer is a special symbol
  int type = isspecialsymbol(buffer);
  //check for comment
  if (type==-1) {
    //return 1 signifiying there is a comment
    return 1;
  }
  //check for <= >= edgecases
  if ((type==27||type==31)&&input[tempidx]=='=') {
    //increment tempidx to look for next nonspace character
    tempidx++;
    while(iscntrl(input[tempidx])||isspace(input[tempidx]))
      tempidx++;
    //check if character is a = symbol
    if (input[tempidx]!='=') //set type to 0 if not to only use first char in buffer
      type=0;
  }
  //check if valid 2 character symbol
  if (!type) {
    //change to one character symbol if not valid
    char newbuffer[3];
    strncpy(newbuffer,buffer,1);
    strcpy(buffer,newbuffer);
    type = isspecialsymbol(buffer);
    //decrement in_index to not point at 2nd character
    in_index--;
  }
  //check if buffer is now valid
  if (type) {
    list[lex_index].type = type;
    lex_index++;
    }
  else {
    //print an error if buffer is still not valid
    printlexerror(4);
    list = NULL;
    exit(0);
  }
  //return 0 signifying no comment
  return 0;
}

int issymbol(char c) {
  //switch case for different symbols
  switch (c) {
    case '.':
      return 1;
    case '[':
      return 1;
    case ']':
      return 1;
    case ',':
      return 1;
    case ';':
      return 1;
    case '?':
      return 1;
    case ':':
      return 1;
    case '(':
      return 1;
    case ')':
      return 1;
    case '<':
      return 1;
    case '>':
      return 1;
    case '+':
      return 1;
    case '-':
      return 1;
    case '*':
      return 1;
    case '/':
      return 1;
    case '%':
      return 1;
    case '=':
      return 1;
    default:
      return 0;
  }
}
int isspecialsymbol(char* symbol) {
  //return type number if special symbol is valid
  //return -1 if comment marker is recognized
  //return 0 if special symbol not found
  if (strcmp(symbol,".")==0) {
    return 1;
  } else if (strcmp(symbol,"[")==0) {
    return 3;
  } else if (strcmp(symbol,"]")==0) {
    return 5;
  } else if (strcmp(symbol,",")==0) {
    return 7;
  } else if (strcmp(symbol,";")==0) {
    return 9;
  } else if (strcmp(symbol,":=")==0) {
    return 11;
  } else if (strcmp(symbol,"?")==0) {
    return 13;
  } else if (strcmp(symbol,":")==0) {
    return 15;
  } else if (strcmp(symbol,"(")==0) {
    return 17;
  } else if (strcmp(symbol,")")==0) {
    return 19;
  } else if (strcmp(symbol,"==")==0) {
    return 21;
  } else if (strcmp(symbol,"<>")==0) {
    return 23;
  } else if (strcmp(symbol,"<")==0) {
    return 25;
  } else if (strcmp(symbol,"<=")==0) {
    return 27;
  } else if (strcmp(symbol,">")==0) {
    return 29;
  } else if (strcmp(symbol,">=")==0) {
    return 31;
  } else if (strcmp(symbol,"+")==0) {
    return 32;
  } else if (strcmp(symbol,"-")==0) {
    return 30;
  } else if (strcmp(symbol,"*")==0) {
    return 28;
  } else if (strcmp(symbol,"/")==0) {
    return 26;
  } else if (strcmp(symbol,"%")==0) {
    return 24;
  } else if (strcmp(symbol,"//")==0) {
    return -1;
  }
  return 0;
}

void comment(char* input) {
  //reads all character until it finds a newline
  while (input[in_index]!='\n'&&input[in_index++]!='\0');
  //returns to new line character
  in_index--;
}

// Will print the lexeme table and the token list
void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case lbracketsym:
				printf("%11s\t%d", "[", lbracketsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case rbracketsym:
				printf("%11s\t%d", "]", rbracketsym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case questionsym:
				printf("%11s\t%d", "?", questionsym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case colonsym:
				printf("%11s\t%d", ":", colonsym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case lparenthesissym:
				printf("%11s\t%d", "(", lparenthesissym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case rparenthesissym:
				printf("%11s\t%d", ")", rparenthesissym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
			case neqsym:
				printf("%11s\t%d", "<>", neqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			default:
				printf("%11s\t%s", "err", "err");
				break;
		}
		printf("\n");
	}
	printf("\n");
} 

// Prints error message if error detected in *lexeme function, and frees the list
// allocated in the *lexeme function. 
void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Number Length\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Identifier Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}