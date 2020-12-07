#include "pch.h"
#include <iostream>
#include<stdlib.h>
#include<string>
#include "Header.h"
using namespace std;
using namespace world;



int main()
{
	repablick RB;
	Union EA;
	kindom UK;
	question answer;
	EA.getSquare(4500000);
	RB.getSquare(200000);
	UK.getSquare(250000);

	UK.newLeader("Charlz");
	EA.newLeader("Jhon");
	RB.newParlament();
	
	answer.sqare(&RB);
	answer.sqare(&EA);
	answer.sqare(&UK);
}


