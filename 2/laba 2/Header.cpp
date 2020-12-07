#include "pch.h"
#include<stdlib.h>
#include<string>
#include"Header.h"
using namespace world;

void square:: getSquare(int square)
{
	this->square = square;
}

void leader:: changeLeader(string nameOfNewLeader)
{
	name = nameOfNewLeader;
}

void parlament:: craeteParlament()
{
	cout << "parlament has created" << endl;
}

void repablick :: putSquare()
{
	cout << "square of repablick more then " << this->square << "km" << endl;
}

void repablick ::newParlament()
{
	craeteParlament();
}

void kindom:: putSquare()
{
	cout << "square of kindom more then " << this->square << "km" << endl;
}

void kindom:: newLeader(string name)
{
	changeLeader(name);
}

void Union:: putSquare()
{

	cout << "square of Union more then " << this->square << "km" << endl;
}

 void  question::sqare(square *answer)
{
	answer->putSquare();
}