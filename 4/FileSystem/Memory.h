#pragma once

#include <vector>

#include "Block.h"

using namespace std;

class Memory {
public:
	static vector<Block> blocks;
	static char getValueAt(int);
	static void clearValueAt(int);
};