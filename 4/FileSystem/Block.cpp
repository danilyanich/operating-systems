
#include "Block.h"
#include "Memory.h"

int Block::count = 0;

Block::Block()
{
}

Block::Block(char c)
{
	id = count++;
	content = c;
	Memory::blocks.push_back(* this);
}

int Block::getId()
{
	return id;
}

char Block::getContent()
{
	return content;
}

void Block::setContent(char c) {
	content = c;
}

