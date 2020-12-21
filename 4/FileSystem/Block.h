#pragma once

class Block {
private:
	int id;
	char content;

public:
	static int count;
	Block();
	Block(char);
	int getId();
	char getContent();
	void setContent(char);
};
