#pragma once
#include "AVL_Tree.h"
#include "CharStringLink.h"
#include "HashTable.h"
#include "memory.h"

//extern int total_doc_num;

//extern TitleLink TitleLinkList;

//extern CharStringLink *wordlist;

void TF_IDF(int code,CharString &x,CharString **top_three,double *point_top_three);

void bulk_search(AVL_Tree *InvertedFileIndex);

void bulk_recommendation(CharString **title_list,AVL_Tree *InvertedFileIndex,AVL_Tree *InvertedFileIndex_title,character *root,int *word_num,int total_doc_num);