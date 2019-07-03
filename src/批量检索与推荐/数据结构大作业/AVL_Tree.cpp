#include "AVL_Tree.h"

AVL_Tree::AVL_Tree()
{
	root=NULL;
}

void AVL_Tree::Insert(CharString *word,int urlcode)
{
	insert(root,word,urlcode);
}

int AVL_Tree::insert(treenode *&p,CharString *word,int urlcode)    //返回值为p节点深度的变化（增加量）
{
	if(p==NULL)
	{
		p=new treenode;
		p->data=new FileLinkList(word);
		p->data->Add(urlcode);
		p->bf=0;
		p->right=NULL;
		p->left=NULL;
		return 1;
	}
	if(p->data->key==*word)
	{
		p->data->Add(urlcode);
		return 0;
	}
	int change;
	if(p->data->key>*word)
	{
		change=insert(p->left,word,urlcode);
		p->bf+=change;
	}
	else if(p->data->key<*word)
	{
		change=insert(p->right,word,urlcode);
		p->bf-=change;
	}
	if(p->bf==-2||p->bf==2)
		adjust(p);
	if(p->bf==0||change==0)
		return 0;
	else
		return 1;

}

void AVL_Tree::R_rotate(treenode *&p)
{
	treenode *lc;
	lc=p->left;
	p->left=lc->right;
	lc->right=p;
	p=lc;
}

void AVL_Tree::L_rotate(treenode *&p)
{
	treenode *rc;
	rc=p->right;
	p->right=rc->left;
	rc->left=p;
	p=rc;
}


void AVL_Tree::adjust(treenode *&p)
{
	int bf=p->bf;
	if(bf==2)
	{
		treenode *ls=p->left;
		if(ls->bf==1)
		{
			R_rotate(p);
			p->bf=0;
			p->right->bf=0;
		}
		else if(ls->bf==-1)
		{
			int n_bf=ls->right->bf;
			L_rotate(p->left);
			R_rotate(p);
			p->bf=0;
			if(n_bf==1)
			{
				p->left->bf=0;
				p->right->bf=-1;
			}
			else
			{
				p->left->bf=1;
				p->right->bf=0;
			}
		}
	}
	else if(bf==-2)
	{
		treenode *rs=p->right;
		if(rs->bf==-1)
		{
			L_rotate(p);
			p->bf=0;
			p->left->bf=0;
		}
		else if(rs->bf==1)
		{
			int n_bf=rs->left->bf;
			R_rotate(p->right);
			L_rotate(p);
			p->bf=0;
			if(n_bf==1)
			{
				p->left->bf=0;
				p->right->bf=-1;
			}
			else
			{
				p->left->bf=1;
				p->right->bf=0;
			}
		}
	}
}

FileLinkList *AVL_Tree::Search(CharString &word)
{
	treenode *temp=root;
	while(temp!=NULL)
	{
		if(temp->data)
		{
			if(temp->data->key==word)
				return temp->data;
			if(temp->data->key>word)
				temp=temp->left;
			else
				temp=temp->right;
		}
	}
	temp=NULL;
	return NULL;
}

void init_InvertedFileIndex(AVL_Tree *InvertedFileIndex,FILE *fp,int urlcode,int *word_num)
{
	char buffer[256];
	while(fgets(buffer,255,fp))
	{
		CharString *word=new CharString;
		*word=buffer;
		word->remove2();
		//printf("%s\n",word->GetData());
		InvertedFileIndex->Insert(word,urlcode);
		word_num[urlcode]++;
		delete word;
	}
}