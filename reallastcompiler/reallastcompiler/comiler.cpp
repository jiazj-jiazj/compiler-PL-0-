/*
	Created by 021830122¼ÖÖ¾¾ü on 2020/11/29
*/
#include"head.h"
fstream intxt;
fstream outtxt;
fstream readLexi;
fstream writeGram;

const string key[] = { "program","const","var","procedure","begin","end","if","then","while","do","call","read","write","odd" };
int line;
int column;
Unit  unit;

//Lexical analysis
void LA();
bool isBC(char cc);
bool isDigit(char cc);
bool isLetter(char cc);
string concat(char cc, string loken);
int reserve(string loken);
void Retract();
//Grammar analysis
void GA();
void readLine();
void throwError(string error, int ismissing);
void prog();
void block();
void condecl();
void const_();
void vardecl();
void proc();
void body();
void statement();
void lexp();
void exp();
void term();
void factor();

int main()
{
	LA();
	GA();
	
	return  0;
}
void GA() //Grammar analysis
{
	readLexi.open("write.txt", ios::in);
	readLine();
	prog();
	string line;
	if (!readLexi.eof())   //if prog is over ,but words still exit
	{
		throwError(unit.value, 0);
	}
	else
	{
		exit(0);
	}
	while (!readLexi.eof())
	{
		getline(readLexi, line);
		if (readLexi.eof())
		{
			exit(0);
		}
		istringstream iss(line);
		iss >> unit.value;
		iss >> unit.key;
		iss >> unit.line;
		iss >> unit.column;
		throwError(unit.value, 0);
	}
	readLexi.close();
}
void LA()    //Lexical analysis
{
	char ch;
	string strToken;
	intxt.open("read.txt", ios::in);
	outtxt.open("write.txt", ios::out | ios::trunc);
	while ((ch = intxt.get()) != -1)
	{
		if (!line)
			line++;
		if (isBC(ch))
		{
			strToken = "";
		}
		else if (isLetter(ch))
		{
			while (isLetter(ch) || isDigit(ch))
			{
				strToken = concat(ch, strToken);
				column++;
				ch = intxt.get();
			}
			if (reserve(strToken))
			{
				cout << strToken << " " << "keyword" << " " << line << " " << column << " " << endl;
				outtxt << strToken << " " << "keyword" << " " << line << " " << column << " " << endl;
			}
			else {
				cout << strToken << " " << "ID" << " " << line << " " << column << " " << endl;
				outtxt << strToken << " " << "ID" << " " << line << " " << column << " " << endl;
			}
			Retract();
			strToken = "";
		}
		else if (isDigit(ch))
		{
			while (isDigit(ch))
			{
				strToken = concat(ch, strToken);
				column++;
				ch = intxt.get();
			}

			if (isLetter(ch))  //  char behaind int is wrong
			{
				cout << "[Lexical error]" << " " << line << " " << column << " " << endl;
				outtxt << "[Lexical error]" << " " << line << " " << column << " " << endl;

				while (isLetter(ch) || isDigit(ch))  //¼ÌÐø¶ÁID
				{
					strToken = concat(ch, strToken);
					column++;
					ch = intxt.get();
				}
				cout << strToken << " " << " Invalid ID" << " " << line << " " << column << " " << endl;
				outtxt << strToken << " " << " Invalid ID" << " " << line << " " << column << " " << endl;

				cout << "[Lexical error]" << " " << line << " " << column << " " << endl;
				outtxt << "[Lexical error]" << " " << line << " " << column << " " << endl;

			}
			else {
				cout << strToken << " " << " INT" << " " << line << " " << column << " " << endl;
				outtxt << strToken << " " << " INT" << " " << line << " " << column << " " << endl;
			}
			Retract();
			strToken = "";

		}
		else switch (ch)
		{
		case '=':
			column++;
			cout << ch << " " << "lop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "lop" << " " << line << " " << column << " " << endl;
			break;
		case ':':
			column++;
			strToken = concat(ch, strToken);
			ch = intxt.get();

			strToken = concat(ch, strToken);
			if (ch == '=')
			{
				column++;
				cout << strToken << " " << "aop" << " " << line << " " << column << " " << endl;
				outtxt << strToken << " " << "aop" << " " << line << " " << column << " " << endl;

			}
			else {
				cout << "Lexical error" << " " << "£º=AOP" << line << " " << column << " " << "/.missing =./" << endl;
				outtxt << "Lexical error" << " " << "£º=AOP" << line << " " << column << " " << "/.missing =./" << endl;
				Retract();
			}
			strToken = "";
			break;
		case '>':
			column++;
			ch = intxt.get();
			if (ch == '=')
			{
				column++;
				cout << ">=" << " " << "lop" << " " << line << " " << column << " " << endl;
				outtxt << ">=" << " " << "lop" << " " << line << " " << column << " " << endl;
			}
			else {
				cout << ">" << " " << "lop" << " " << line << " " << column << " " << endl;

				outtxt << ">" << " " << "lop" << " " << line << " " << column << " " << endl;
				Retract();
			}
			break;
		case '<':
			column++;
			ch = intxt.get();
			if (ch == '>')
			{
				column++;
				cout << "<>" << " " << "lop" << " " << line << " " << column << " " << endl;
				outtxt << "<>" << " " << "lop" << " " << line << " " << column << " " << endl;

			}
			else if (ch == '=')
			{
				column++;
				cout << "<=" << " " << "lop" << " " << line << " " << column << " " << endl;
				outtxt << "<=" << " " << "lop" << " " << line << " " << column << " " << endl;
			}
			else {
				cout << "<" << " " << "cop" << " " << line << " " << column << " " << endl;
				outtxt << "<" << " " << "cop" << " " << line << " " << column << " " << endl;
				Retract();
			}
			break;
		case '+':
			column++;
			cout << ch << " " << "aop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "aop" << " " << line << " " << column << " " << endl;
			break;

		case '-':
			column++;
			cout << ch << " " << "aop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "aop" << " " << line << " " << column << " " << endl;
			break;

		case '*':
			column++;
			cout << ch << " " << "mop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "mop" << " " << line << " " << column << " " << endl;
			break;
		case '/':
			column++;
			cout << ch << " " << "mop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "mop" << " " << line << " " << column << " " << endl;
			break;
		case ',':
			column++;
			cout << ch << " " << "sop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "sop" << " " << line << " " << column << " " << endl;
			break;
		case ';':
			column++;
			outtxt << ch << " " << "eop" << " " << line << " " << column << " " << endl;
			cout << ch << " " << "eop" << " " << line << " " << column << " " << endl;
			break;
		case '(':

		case ')':
			column++;
			cout << ch << " " << "sop" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "sop" << " " << line << " " << column << " " << endl;
			break;



		default:
			column++;
			cout << ch << " " << "UNKNOW" << " " << line << " " << column << " " << endl;
			outtxt << ch << " " << "UNKNOW" << " " << line << " " << column << " " << endl;
			break;
		}

	}
	intxt.close();
	outtxt.close();
}


//Lexical
bool isBC(char cc)
{
	if (cc == '\n' || cc == '\t' || cc == ' ' || cc == '\r')
	{
		switch (cc)
		{
		case ' ':
			column++;
			break;
		case '\n':
			line++;
			column = 0;
			break;
		case '\t':
			column += 4;
			break;
		case '\r':
			column = 1;
		default:
			break;
		}
		return 1;
	}
	return 0;
}

bool isDigit(char cc)
{
	if (cc >= '0' && cc <= '9')
		return 1;
	return 0;
}

bool isLetter(char cc)
{
	if ((cc >= 'a' && cc <= 'z') || (cc >= 'A' && cc <= 'Z'))
	{
		return 1;

	}
	return 0;
}

string concat(char cc, string loken)
{
	return loken + cc;
}

int reserve(string loken)
{
	int i;
	for (int i = 0; i < 15; i++)
	{
		if (loken == key[i])
			return 1;
	}
	return 0;
}


void Retract()
{
	if (!intxt.eof())
		intxt.seekg(-1, ios::cur);

}

//Grammar  
void readLine()
{
	string line;
	if (!readLexi.eof())
	{
		getline(readLexi, line);
		if (readLexi.eof())
		{
			exit(0);
		}
		istringstream iss(line);
		iss >> unit.value;
		iss >> unit.key;
		iss >> unit.line;
		iss >> unit.column;
	}
	else {
		exit(0);
	}
}

void throwError(string error, int ismissing)
{
	if (ismissing == 1)
	{
		cout << error + " is missing ";
		unit.print();
	}
	if (ismissing == 0)
	{
		cout << error + " is extra ";
		unit.print();
	}
}
void prog()
{

	while (unit.value != "program")
	{
		throwError(unit.value, 0);
		readLine();
	}
	readLine();
	if (unit.key != "ID")
	{
		throwError("ID", 1);
	}
	else {
		readLine();
	}
	if (unit.value != ";")
	{
		throwError(";", 1);
	}
	else {
		readLine();
	}
	block();
}
void block()
{
	while ((unit.value != "const") && (unit.value != "var") && (unit.value != "procedure") && (unit.value != "begin"))
	{
		throwError(unit.value, 0);
		readLine();
	}
	if (unit.value == "const")
	{
		condecl();
	}
	if (unit.value == "var")
	{
		vardecl();
	}
	if (unit.value == "procedure")
	{
		proc();
	}
	body();
}

void condecl()
{
	while (unit.value != "const")
	{
		throwError(unit.value, 0);
		readLine();
	}
	readLine();
	const_();
	while (unit.value == ",")
	{
		readLine();
		const_();
	}
	if (unit.value != ";")
	{
		throwError(";", 1);
	}
	else
	{
		readLine();
	}
}

void const_()
{
	while (unit.key != "ID")
	{
		throwError(unit.value, 0);
		readLine();
	}
	readLine();
	if (unit.value != ":=")
	{
		throwError(":=", 1);
	}
	else {
		readLine();
	}
	if (unit.key != "INT")
	{
		throwError("INT", 1);
	}
	else {
		readLine();
	}
}
void vardecl()
{
	while (unit.value != "var")
	{
		throwError(unit.value, 0);
		readLine();
	}
	readLine();
	if (unit.key != "ID")
	{
		throwError("ID", 1);
	}
	else {
		readLine();
	}
	while (unit.value == ",")
	{
		readLine();
		if (unit.key != "ID")
		{
			throwError("ID", 1);
		}
		else {
			readLine();
		}

	}
	if (unit.value != ";")
	{
		throwError(";", 1);
	}
	else {
		readLine();
	}
}
void proc()
{
	while (unit.value != "procedure")
	{
		throwError(unit.value, 0);
		readLine();
	}
	readLine();
	if (unit.key != "ID")
	{
		throwError("ID", 1);
	}
	else {
		readLine();
	}
	if (unit.value != "(")
	{
		throwError("(", 1);
	}
	else {
		readLine();
	}
	if (unit.key == "ID")
	{
		readLine();
		while (unit.value == ",")
		{
			readLine();
			if (unit.key != "ID")
			{
				throwError("ID", 1);
			}
			else {
				readLine();
			}
		}
	}
	if (unit.value != ")")
	{
		throwError(")", 1);
	}
	else {
		readLine();
	}
	if (unit.value != ";")
	{
		throwError(";", 1);
	}
	else {
		readLine();
	}
	block();
	while (unit.value == ";")
	{
		readLine();
		proc();
	}
}
void body()
{
	while (unit.value != "begin")
	{
		throwError(unit.value, 0);
		readLine();
	}
	readLine();
	statement();
	while (unit.value == ";")
	{
		readLine();
		statement();
	}
	if (unit.value != "end")
	{
		throwError("end", 1);
	}else {
		readLine();
	}
}
void statement()
{
		while (unit.key != "ID"&&unit.value != "if"&&unit.value != "while"&&unit.value != "call"&&unit.value != "begin"&&unit.value != "read"&&unit.value != "write")
		{
			throwError(unit.value, 0);
			readLine();
		}
		if (unit.key == "ID")
		{
			readLine();
			if (unit.value != ":=")
			{
				throwError(":=", 1);
			}
			else {
				readLine();
			}
			exp();
		}
		else if (unit.value == "if")
		{
			readLine();
			lexp();
			if (unit.value != "then")
			{
				throwError("then", 1);
			}
			else {
				readLine();
			}
			statement();
			if (unit.value == "else")
			{
				statement();
			}
		}
		else if (unit.value == "while")
		{
			readLine();
			lexp();
			if (unit.value != "do")
			{
				throwError("do", 1);
			}
			else {
				readLine();
			}
			statement();
		}
		else if (unit.value == "call")
		{
			readLine();
			if (unit.key != "ID")
			{
				throwError("ID", 1);
			}
			else
			{
				readLine();
			}
			if (unit.value != "(")
			{
				throwError("(", 1);
			}
			else {
				readLine();
			}
			if (unit.value == "+" || unit.value == "-" || unit.key == "ID" || unit.key == "INT" || unit.value == "(")
			{
				exp();
				while (unit.value == ",")
				{
					exp();
				}
			}
			if (unit.value != ")")
			{
				throwError(")", 1);
			}
			else {
				readLine();
			}
		}
		else if (unit.value == "begin")
		{
			body();
		}
		else if (unit.value == "read")
		{
			readLine();
			if (unit.value != "(")
			{
				throwError("(", 1);
			}
			else {
				readLine();
			}
			if (unit.key != "ID")
			{
				throwError("ID", 1);
			}
			else {
				readLine();
			}
			while (unit.value == ",")
			{
				readLine();
				exp();
			}
			if (unit.value != ")")
			{
				throwError(")", 1);
			}
			else {
				readLine();
			}
		}
		else if (unit.value == "write")
		{
			readLine();
			if (unit.value != "(")
			{
				throwError("(", 1);
			}
			else readLine();
			exp();
			while (unit.value == ",")
			{
				readLine();
				exp();
			}
			if (unit.value != ")")
			{
				throwError(")", 1);
			}
			else {
				readLine();
			}
		}
}
void lexp() {
	if (unit.value == "odd")
	{
		readLine();
		exp();
	}
	else {
		exp();
		if (unit.key != "lop")
		{
			throwError("lop", 1);
		}
		else {
			readLine();
		}
		exp();
	}
}
void exp()
{
	while (unit.value != "+"&&unit.value != "-"&&unit.key != "ID"&&unit.key != "INT"&&unit.value != "(")
	{
		throwError(unit.value, 0);
		readLine();
	}
	if (unit.value == "+" || unit.value == "-")
	{
		readLine();
	}
	term();
	while (unit.key == "aop")
	{
		readLine();
		term();
	}
}
void term()
{
	factor();
	while (unit.key == "mop")
	{
		readLine();
		factor();
	}
}
void factor()
{
	while (unit.key != "ID"&&unit.key != "INT"&&unit.value != "(")
	{
		throwError(unit.value, 0);
		readLine();
	}
	if (unit.key == "ID")
	{
		readLine();
	}
	else if (unit.key == "INT")
	{
		readLine();
	}
	else if (unit.value == "(")
	{
		readLine();
		exp();
		if (unit.value != ")")
		{
			throwError(")", 1);
		}
		else {
			readLine();
		}
	}
}
