#ifndef HEAD_H
#define HEAD_H
#include<iostream>
#include<ctime>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;
class Unit
{
public:
	string value;
	string key;
	int line;
	int column;
	void print();
};

void Unit::print()
{
	cout << "Position: [" << line << "," << column << "]" << endl;
}
#endif //