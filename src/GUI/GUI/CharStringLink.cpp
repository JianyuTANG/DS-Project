#include "CharStringLink.h"
#include "stdafx.h"

CharStringLink::CharStringLink()
{
	head=NULL;
	tail=NULL;
	length=0;
}

void CharStringLink::add(CharString &x)
{
	CharStringLinkNode *temp;
	temp=new CharStringLinkNode;
	temp->next=NULL;
	temp->word=x.GetData();
	if(head==NULL)
	{
		head=temp;
		tail=head;
	}
	else
	{
		tail->next=temp;
		tail=tail->next;
	}
	length++;
}

void CharStringLink::remove()
{
	if(length==0)
		return;
	if(tail==head)
	{
		delete head;
		head=NULL;
		tail=NULL;
	}
	else
	{
		CharStringLinkNode *temp;
		temp=head;
		while(temp->next!=tail)
			temp=temp->next;
		delete tail;
		tail=temp;
		length--;
	}
}

CharStringLinkNode * CharStringLink::search(CharString &x)
{
	if(length==0)
		return NULL;
	CharStringLinkNode *temp;
	temp=head;
	while(temp!=NULL)
	{
		if(temp->word==x)
			return temp;
		temp=temp->next;
	}
	return NULL;
}

void CharStringLink::print_link(FILE *output)
{
	CharStringLinkNode *cursor;
	cursor=head;
	while(cursor!=NULL)
	{
		fprintf(output,"%s",cursor->word.GetData());
		fprintf(output,"\n");
		cursor=cursor->next;
	}
}

CharStringLinkNode *CharStringLink::gethead()
{
	CharStringLinkNode *temp;
	temp=head;
	return head;
}

CharStringLink::~CharStringLink()
{
	CharStringLinkNode* tmp=head;
	CharStringLinkNode* temp=head->next;
	if(tmp)
		delete tmp;
	while(temp)
	{
		tmp=temp;
		temp=temp->next;
		delete tmp;
	}
}