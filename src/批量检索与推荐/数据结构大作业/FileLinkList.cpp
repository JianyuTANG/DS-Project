#include "FileLinkList.h"

FileLinkList::FileLinkList(CharString *word)
{
	key.assign(word);
	doc_frequency=0;
	head=new linknode;
	head->filecode=-1;
	head->num=-1;
	head->occurnum=-1;
	head->next=NULL;
	head->pre=NULL;
}

linknode *FileLinkList::Search(int code)
{
	linknode *tmp=head->next;
	while(tmp)
	{
		if(tmp->filecode==code)
			return tmp;
		tmp=tmp->next;
	}
	return NULL;
}

linknode *FileLinkList::search(int code)
{
	linknode *tmp=head->next;
	linknode *last=head;
	while(tmp)
	{
		if(tmp->filecode==code)
			return last;
		last=tmp;
		tmp=tmp->next;
	}
	return last;
}

void FileLinkList::Add(int code)
{
	linknode *last=search(code);
	if(last->next)
	{
		linknode *temp=last->next;
		int num=++temp->num;
		last->next=temp->next;
		if(temp->next)
			temp->next->pre=last;
		temp->next=NULL;
		temp->pre=NULL;
		temp->occurnum=1;
		linknode *tmp=last;
		while(tmp->num<num&&tmp!=head)
			tmp=tmp->pre;
		if(tmp->next)
			tmp->next->pre=temp;
		temp->next=tmp->next;
		tmp->next=temp;
		temp->pre=tmp;
	}
	else
	{
		doc_frequency++;
		linknode *temp=new linknode;
		temp->filecode=code;
		temp->num=1;
		temp->occurnum=1;
		temp->next=NULL;
		temp->pre=last;
		last->next=temp;
	}
}

bool FileLinkList::operator==(FileLinkList a)
{
	if(key==a.key)
		return 1;
	else
		return 0;
}

bool FileLinkList::operator>(FileLinkList a)
{
	if(key>a.key)
		return 1;
	else
		return 0;
}

bool FileLinkList::operator<(FileLinkList a)
{
	if(key<a.key)
		return 1;
	else
		return 0;
}

bool linknodecmp(linknode *a,linknode *b)
{
	if(a->occurnum<b->occurnum)
		return true;
	else if(a->occurnum==b->occurnum)
		if(a->num<b->num)
			return true;
	return false;
}

void FileLinkList::Add_for_merge(linknode *x)
{
	int code=x->filecode;
	linknode *last=search(code);
	if(last->next)
	{
		linknode *temp=last->next;
		temp->num+=x->num;
		last->next=temp->next;
		if(temp->next)
			temp->next->pre=last;
		temp->next=NULL;
		temp->pre=NULL;
		temp->occurnum++;
		linknode *tmp=last;
		while(linknodecmp(tmp,temp)&&tmp!=head)
			tmp=tmp->pre;
		if(tmp->next)
			tmp->next->pre=temp;
		temp->next=tmp->next;
		tmp->next=temp;
		temp->pre=tmp;
	}
	else
	{
		linknode *temp=new linknode;
		temp->filecode=code;
		temp->num=x->num;
		temp->occurnum=1;
		temp->next=NULL;
		temp->pre=last;
		last->next=temp;
	}
}

FileLinkList::~FileLinkList()
{
	linknode *tmp=head;
	linknode *temp=head->next;
	if(tmp)
		delete[] tmp;
	while(temp)
	{
		tmp=temp;
		temp=temp->next;
		delete[] tmp;
	}
}