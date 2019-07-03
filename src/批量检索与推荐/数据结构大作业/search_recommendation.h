#pragma once
#include "AVL_Tree.h"
#include "CharStringLink.h"
#include "HashTable.h"
#include "memory.h"

void bulk_search(AVL_Tree *InvertedFileIndex);  //批量检索

void bulk_recommendation(CharString **title_list,AVL_Tree *InvertedFileIndex,AVL_Tree *InvertedFileIndex_title,character *root,int *word_num,int total_doc_num);
//批量推荐