#pragma once
#include <iostream>
#include <string>
using namespace std;

namespace world
{
	class square
	{
	protected:
		int square;
	public:
		virtual void  putSquare() = 0;
		void getSquare(int);

	};

	class leader
	{
	protected:
		string name;
		void changeLeader(string);
	};

	class parlament
	{
	protected:
		void craeteParlament();
	};

	class  repablick : public virtual square, protected parlament
	{
	private:
		string nameOfCountry;

	public:
		void putSquare();
		void newParlament();
	};

	class  kindom : public virtual square, private leader
	{
	public:
		void putSquare();
		void newLeader(string name);
	};

	class Union : public repablick, public kindom
	{
	public:
		void putSquare();
	};

	class question
	{
	public:
		void sqare (square *);
	};
}