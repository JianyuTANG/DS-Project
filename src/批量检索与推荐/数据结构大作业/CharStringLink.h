#pragma once
#include "stdio.h"
#include "CharString.h"

struct CharStringLinkNode
{
	CharString word;
	CharStringLinkNode *next;
};

class CharStringLink
{
public:
	CharStringLink();
	void add(CharString &x);
	CharStringLinkNode * search(CharString &x);
	CharStringLinkNode *gethead();
	void remove();
	void print_link(FILE *output);
	~CharStringLink();
private:
	CharStringLinkNode *head;
	CharStringLinkNode *tail;
	int length;
};
