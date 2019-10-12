

//          Course: CS3820-01 Programming Languages
//            Name: Sajan, Kefin
//      Assignment: Programming Assignment 3
//   Date assigned: 12/13/18
//        Date due: 12/20/18
//  Date handed in: 12/20/18
//          Remark: This is a program created to simulate a recursive descent parser

#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

using namespace std;

//Buffer definition//
#define BUFFERSIZE 80
char tokenBuffer[BUFFERSIZE];
bool needToken = true;

enum tokenType { AND, BEGIN, END, FOR, IF, NOT, OR, READ, WHILE, WRITE, COMMENT, ID, REAL, STRING, PLUS, MULTIPLICATION, ASSIGNMENT, EQUAL, GREATERTHAN, LESSTHAN, LEFTP, COMMA, RIGHTP, SEMICOLON, INVALID, DIVISION, INTEGER };

//Scanner Functions Headers//
tokenType lexical_error(void);
//  Return Invaild if the input is not in the system

void skipSpaces();
//  Skips over space characters in the input stream.

void clearBuffer(void);
//  Sets all the elements of the buffer tokenBuffer[] to the null character.

tokenType getId(void);
//  Looks for identifiers.

tokenType getComment(void);
//  Looks for comment, or division char depending on the next character.

tokenType getReal(void);
//  Looks for a real const, or integer.

tokenType getStrings(void);
//  Looks for a string const

void displayToken(tokenType code);
//  Get an argument as a token code, then displays the appropriate message, also prints the contents of the buffer.

//New function implemented for Assignment 3//

tokenType getnexttoken(void);
//  Calls the function int scanner (void) from the scanner assignment
//  to get the code of the next token from the input source file

tokenType peektoken(void);
//  Calls the function getnexttoken(void)  and then returns the code
//  returned by getnexttoken(void)

tokenType readtoken(void);
// Calls the function getnexttoken(void), sets the global variable
// needToken to true and returns the code returned by getnexttoken(void)

void match(tokenType token);
// Receives as argument a token code (to be matched with the next input token)

void syntaxerror(tokenType token);
// Receives as argument a token code and then prints an appropriate
// error message and then skip to the next line.This also make sures
// that errors in your input source program are only missing tokens.

//Parcer Function Headers//
void program(void);
void stmntlist(void);
void stmnt(void);

//Scanner Functions//

tokenType lexical_error(void)
// Return Invaild if the input is not in the system
{
	tokenType userInput;
	int num = cin.get();

	tokenBuffer[0] = num;
    
	return INVALID;
}
void skipSpaces()
// Skips over space characters in the input stream.
{
	int usrInp;
	usrInp = cin.get();

	while (isspace(usrInp))
		usrInp = cin.get();

	cin.putback(usrInp);
}
void clear_buf(void)
{
	for (int i = 0; i < BUFFERSIZE; i++)
		tokenBuffer[i] = '\0';
}
tokenType getId(void)
// Looks for identifiers.
{
	static char reservedWords[][10] =
	{ "AND", "BEGIN", "END", "FOR", "IF", "NOT", "OR", "READ", "WHILE", "WRITE" };
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();

	if (isalpha(usrInp))
	{
		tokenBuffer[i++] = usrInp;
		usrInp = cin.get();
		while (isalnum(usrInp))
		{
			tokenBuffer[i++] = usrInp;
			usrInp = cin.get();
		}
		cin.putback(usrInp);


		int first = 0, mid, last = 9;
		bool ntFd = true;
		while (first <= last && ntFd)
		{
			mid = (first + last) / 2;
			int answer = strcmp(tokenBuffer, reservedWords[mid]);
			if (answer == 0)
				ntFd = false;
			else if (answer > 0)
				first = mid + 1;
			else
				last = mid - 1;
		}
		if (ntFd)
			userInput = ID;
		else
			userInput = (tokenType)mid;
	}
	else
	{
		cin.putback(usrInp);
        return INVALID;
	}
	return userInput;
}

tokenType getComment(void)
// Finds comment, or divide char depending on the next character.
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();

	if (cin.peek() == '*' && usrInp == '/')
	{
		tokenBuffer[i++] = usrInp;

		usrInp = cin.get();
		//tokenBuffer[i++] = usrInp;

		usrInp = cin.get();

		while (!(usrInp == '*' && cin.peek() == '/') && cin.peek() != EOF)
		{
			tokenBuffer[i++] = usrInp;
			usrInp = cin.get();
			//Buffer Increase
		}

		if (cin.peek() == EOF)
			userInput = INVALID;
		else
		{
			tokenBuffer[i++] = usrInp;
			usrInp = cin.get();
			tokenBuffer[i++] = usrInp;
			return COMMENT;
		}
	}
	else
	{
		cin.putback(usrInp);
        return INVALID;
	}
	return userInput;
}

tokenType getReal(void)
// Looks for a real constant, or integer.
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	if (isdigit(usrInp))
	{
		tokenBuffer[i++] = usrInp;
		usrInp = cin.get();

		while (isdigit(usrInp))
		{
			tokenBuffer[i++] = usrInp;
			usrInp = cin.get();
			//Buffer Increase
		}

		if (usrInp == '.')
		{
			tokenBuffer[i++] = usrInp;
			usrInp = cin.get();
			if (isdigit(usrInp))
			{
				tokenBuffer[i++] = usrInp;
				usrInp = cin.get();
				while (isdigit(usrInp))
				{
					tokenBuffer[i++] = usrInp;
					usrInp = cin.get();
				}
				cin.putback(usrInp);
				return REAL;
			}
			else
			{
				cin.putback(usrInp);
				userInput = INVALID;
			}
		}
		else
		{
			cin.putback(usrInp);
			userInput = INVALID;
		}
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
	return userInput;
}

tokenType getStrings(void)
// Looks for a string constant.
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '\"')
	{
		tokenBuffer[i++] = usrInp;

		usrInp = cin.get();
		while (!(usrInp == '\"') && usrInp != EOF)
		{
			tokenBuffer[i++] = usrInp;

			usrInp = cin.get();
		}
		if (usrInp == EOF)
			userInput = INVALID;
		else
		{
			tokenBuffer[i++] = usrInp;
			userInput = STRING;
		}
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
    return userInput;
}

tokenType getPlus(void)
{
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '+')
	{
		tokenBuffer[i++] = usrInp;
		return PLUS;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}

tokenType getMul(void)
{
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '*')
	{
		tokenBuffer[i++] = usrInp;
		return MULTIPLICATION;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}

tokenType getAssign(void)
{
	int usrInp;
	int usrInp2;
	int i = 0;

	usrInp = cin.get();
	usrInp2 = cin.peek();
	if (usrInp == ':' && usrInp2 == '=')
	{
		tokenBuffer[i++] = usrInp;
		usrInp = cin.get();
		tokenBuffer[i] = usrInp;
		return ASSIGNMENT;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}

tokenType getEqual(void)
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '=')
	{
		tokenBuffer[i++] = usrInp;
		userInput= EQUAL;
	}
	else
	{
		cin.putback(usrInp);
		userInput= INVALID;
	}
    return userInput;
}

tokenType getGreater(void)
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '>')
	{
		tokenBuffer[i++] = usrInp;
		return GREATERTHAN;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}

tokenType getLess(void)
{
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '<')
	{
		tokenBuffer[i++] = usrInp;
		return LESSTHAN;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}


tokenType getLP(void)
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == '(')
	{
		tokenBuffer[i++] = usrInp;
		return LEFTP;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}


tokenType getRP(void)
{
	tokenType userInput;
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == ')')
	{
		tokenBuffer[i++] = usrInp;
		return RIGHTP;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}


tokenType getComma(void)
{
	int usrInp;
	int i = 0;

	usrInp = cin.get();
	if (usrInp == ',')
	{
		tokenBuffer[i++] = usrInp;
		return COMMA;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}


tokenType getSColon(void)
{
	int usrInp;
	int i = 0;
    
    usrInp = cin.get();
	if (usrInp == ';')
	{
		tokenBuffer[i++] = ';';
		return SEMICOLON;
	}
	else
	{
		cin.putback(usrInp);
		return INVALID;
	}
}

tokenType scanner(void)
{
    skipSpaces();
    int usrInp = cin.get();
    
    if(usrInp == EOF)
        return(tokenType)EOF;
    
    if (usrInp == '/')
    {
        cin.putback(usrInp);
        return getComment();
    }
    
    if (isalpha(usrInp))
    {
        cin.putback(usrInp);
        return getId();
    }
    
    if (isdigit(usrInp))
    {
        cin.putback(usrInp);
        return getReal();
    }
    
    if (usrInp == '\"')
    {
        cin.putback(usrInp);
        return getStrings();
    }
    
    if (usrInp == '+')
    {
        cin.putback(usrInp);
        return getPlus();
    }
    
    if (usrInp == '*')
    {
        cin.putback(usrInp);
        return getMul();
    }
    
    if (usrInp == '=')
    {
        cin.putback(usrInp);
        return getEqual();
    }
    
    if (usrInp == ':')
    {
        cin.putback(usrInp);
        return getAssign();
    }
    
    if(usrInp == '>')
    {
        cin.putback(usrInp);
        return getGreater();
    }
    
    if (usrInp == '<')
    {
        cin.putback(usrInp);
        return getLess();
    }
    
    if (usrInp == '(')
    {
        cin.putback(usrInp);
        return getLP();
    }
    
    if (usrInp == ')')
    {
        cin.putback(usrInp);
        return getRP();
    }
    
    if (usrInp == ',')
    {
        cin.putback(usrInp);
        return getComma();
    }

    if (usrInp == ';')
    {
        cin.putback(usrInp);
        return getSColon();
    }
    
    // Can be only a lexical error //
    
    cin.putback(usrInp);
    return lexical_error();
    
}

void display_token(tokenType code)
// Get an argument as a token code, then displays the appropriate message, also prints the contents of the buffer.
{
	const char *MESS[] = { "and", "begin", "end", "for", "if", "not", "or", "read", "while", "write", "comment", "identifier", "real constant", "string", "plus", "multiplication", "assignment", "equal", "greater than", "less than", "left parenthesis", "comma", "right parenthesis", "semicolon", "invalid", "division", "integer" };
	cout << " \n\t\t" << MESS[(int)code] << "\t" << tokenBuffer;
}

//New function implemented for Assignment 3
tokenType getnexttoken(void)
{
    tokenType nexttoken;
    if (needToken)
    {
        nexttoken = scanner();
        needToken = false;
    }
    return nexttoken;
}

tokenType peektoken(void)
{
    return getnexttoken();
}

tokenType readtoken(void)
{
    tokenType tok = getnexttoken();
    needToken = true;
    return tok;
}

void syntaxerror(tokenType token)
{
    static char message[][20] =  { "AND", "BEGIN", "END", "FOR", "IF", "NOT","OR", "READ", "WHILE", "WRITE", "COMMENT", "IDENTIFIER",  "REAL CONSTANT","STRING", "PLUS", "MULTIPLICATION", "ASSIGNMENT", "EQUAL", "GREATER THAN",
        "LESS THAN", "LEFT PARENTHESIS", "COMMA", "RIGHT PARENTHESIS", "SEMICOLON","INVALID", "DIVISION", "INTEGER"
    };
    
        cout<< " token \"" << message[(int)token] << "\"\t"<< " # "
            << (int)token << endl;
    
}

void match(tokenType token)
{
    int tok = readtoken();
    
    if (token == tok)
    { if (tok != SEMICOLON)
        {
            cout << tokenBuffer << endl;
            clear_buf();
        }
    }
        else
            syntaxerror(token);
}

//End of Scanner Functions//

//Parser Functions//
void program(void)
{
    match(BEGIN);
    stmntlist();
    match(END);
}

void stmntlist(void)
{
    tokenType nexttoken = peektoken();
    
    if(nexttoken){

        stmnt();
        match(SEMICOLON);
        stmntlist();
    }
    else
        syntaxerror(nexttoken);
}

void stmnt(void)
{
    tokenType nexttoken = peektoken();
    
    if (nexttoken)
    {
        match(peektoken());
        match(ASSIGNMENT);
        stmntlist();
        match(SEMICOLON);
    }
    else
        syntaxerror(nexttoken);
}
//End of Parser Functions//

//Main Function//
int main()
{
    program();
	return 0;
}
