#pragma once
#include <cstdio>
#include <cstdlib>

template <class T>
struct treenode          //树节点
{
	T data;                  //关键字
	int bf;                  //balance factor
	treenode<T> *left,*right;//左右孩子
};

template <class T>
class AVL_Tree
{
public:
	AVL_Tree();
	void Insert(T x);                            //向AVL树中插入关键字x
	void Adjust(treenode<T> *&currentnode);      
	treenode<T> *Search(T x);                    //搜索关键字为x的节点
	void Remove(T x);                            //删除AVL树种关键字为x的结点
	void Edit(T currentkey,T newkey);            //将AVL树中已有的关键字currentkey修改为newkey
private:
	treenode<T> *root;                           //树根节点
	int insert(treenode<T> *&p,T x);
	void adjust(treenode<T> *&p);                //插入时的调整
	int adjust2(treenode<T> *&p);                //删除时的调整
	int remove(treenode<T> *&p,treenode<T> *&last,T x);
	void R_rotate(treenode<T> *&p);
	void L_rotate(treenode<T> *&p);
};

template <class T>
AVL_Tree<T>::AVL_Tree()
{
	root=NULL;
}

template <class T>
void AVL_Tree<T>::Insert(T x)
{
	insert(root,x);
}

template <class T>
int AVL_Tree<T>::insert(treenode<T> *&p,T x)    //返回值为p节点深度的变化（增加量）
{
	if(p==NULL)
	{
		p=new treenode<T>;
		p->data=x;
		p->bf=0;
		p->right=NULL;
		p->left=NULL;
		return 1;
	}
	if(p->data==x)
	{
		return 0;
	}
	int change;
	if(p->data>x)
	{
		change=insert(p->left,x);
		p->bf+=change;
	}
	else if(p->data<x)
	{
		change=insert(p->right,x);
		p->bf-=change;
	}
	if(p->bf==-2||p->bf==2)
		adjust(p);
	if(p->bf==0||change==0)
		return 0;
	else
		return 1;

}

template <class T>
void AVL_Tree<T>::R_rotate(treenode<T> *&p)
{
	treenode<T> *lc;
	lc=p->left;
	p->left=lc->right;
	lc->right=p;
	p=lc;
}

template <class T>
void AVL_Tree<T>::L_rotate(treenode<T> *&p)
{
	treenode<T> *rc;
	rc=p->right;
	p->right=rc->left;
	rc->left=p;
	p=rc;
}

template <class T>
void AVL_Tree<T>::adjust(treenode<T> *&p)
{
	int bf=p->bf;
	if(bf==2)
	{
		treenode<T> *ls=p->left;
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
		treenode<T> *rs=p->right;
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

template <class T>
treenode<T> *AVL_Tree<T>::Search(T x)
{
	treenode<T> *temp=root;
	while(temp!=NULL)
	{
		if(temp->data==x)
			return temp;
		if(temp->data>x)
			temp=temp->left;
		else
			temp=temp->right;
	}
	return temp;
}

template <class T>
void AVL_Tree<T>::Remove(T x)
{
	remove(root,x);
}

template <class T>
int AVL_Tree<T>::remove(treenode<T> *&p,treenode<T> *&last,T x)    //返回值为p节点深度改变量（减小量）的绝对值
{
	if(p==NULL)
		return 0;
	if(p->data>x)
	{
		int change;
		change=remove(p->left,x);
		p->bf+=change;
		if(p->bf==2||p->bf==-2)
			return adjust2(p);
		if(p->bf==0&&change!=0)
			return 1;
		else
			return 0;
	}
	else if(p->data<x)
	{
		remove(p->right,x);
		int change;
		change=remove(p->right,x);
		p->bf-=change;
		if(p->bf==2||p->bf==-2)
			return adjust2(p);
		if(p->bf==0&&change!=0)
			return 1;
		else
			return 0;
	}
	else
	{
		if(p->left==NULL&&p->right==NULL)
		{
			delete[] p;
			p=NULL;
			return 1;
		}
		else if(p->left==NULL)
		{
			treenode<T> *tmp;
			tmp=p->right;
			delete[] p;
			p=tmp;
			return 1;
		}
		else if(p->right==NULL)
		{
			treenode<T> *tmp;
			tmp=p->left;
			delete[] p;
			p=tmp;
			return 1;
		}
		else
		{
			treenode<T> *target;
			target=p->left;
			while(target->right!=NULL)
				target=target->right;
			T newkey=target->data;
			int change;
			change=remove(p,newkey);
			p->data=newkey;
			p->bf+=change;
			if(p->bf==2||p->bf==-2)
				return adjust2(p);
			if(p->bf==0&&change!=0)
				return 1;
			else
				return 0;
		}
	}
}

template <class T>
int AVL_Tree<T>::adjust2(treenode<T> *&p)
{
	if(p->bf==-2)
	{
		treenode<T> *q=p->right;
		int bf=q->bf;
		switch (bf)
		{
		case -1:
			L_rotate(p);
			p->bf=0;
			p->left->bf=0;
			break;
		case 0:
			L_rotate(p);
			p->bf=1;
			p->left->bf=-1;
			break;
		case 1:
			bf=q->left->bf;
			R_rotate(p->left);
			L_rotate(p);
			p->bf=0;
			switch (bf)
			{
			case 1:
				p->left->bf=0;
				p->right->bf=-1;
				break;
			case 0:
				p->left->bf=0;
				p->right->bf=0;
				break;
			case -1:
				p->left->bf=1;
				p->right->bf=0;
				break;
			}
			return 1;
			break;
		}
	}
	else if(p->bf==2)
	{
		treenode<T> *q=p->left;
		int bf=q->bf;
		switch (bf)
		{
		case 1:
			R_rotate(p);
			p->bf=0;
			p->right->bf=0;
			break;
		case 0:
			R_rotate(p);
			p->bf=-1;
			p->right->bf=1;
			break;
		case -1:
			bf=q->right->bf;
			L_rotate(p->left);
			R_rotate(p);
			p->bf=0;
			switch (bf)
			{
			case 1:
				p->left->bf=0;
				p->right->bf=-1;
				break;
			case 0:
				p->left->bf=0;
				p->right->bf=0;
				break;
			case -1:
				p->left->bf=1;
				p->right->bf=0;
				break;
			}
			return 1;
			break;
		}
	}
	return 0;
}

template <class T>
void AVL_Tree<T>::Edit(T currentkey,T newkey)
{
	remove(root,currentkey);
	insert(root,currentkey);
}