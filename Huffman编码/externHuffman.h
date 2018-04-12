#pragma once
#ifndef externHuffman
#define externHuffman

#include"Huffman.h"
#include<cstring>

template<typename E>
class Code
{
public:
	E* vals;
	char** outcome;
	int length;

	Code(int number);
	~Code();
	void produceSpaceOutcome(int size);
};

template<typename E>
Code<E>::Code(int number)
{
	vals = new E[number];
	length = number;
	outcome = new char*[number];
}
template<typename E>
Code<E>::~Code(){}

template<typename E>
void Code<E>::produceSpaceOutcome(int size)
{
	outcome[length] = new char[size];
}

template<class E>
void huffmanCode(HuffTree<E>* hTree, int numOfLeaf,Code<E> &code)
{
	char* temp = new char[numOfLeaf];
	code.length = 0;

	//hTree->traverse(hTree->root());

	codeHelp(hTree->root(), temp, code, 0);

	/*for (int i = 0; i < numOfLeaf; i++)
	{
		cout << "字符：" << code.vals[i];
		cout << "编码：" << code.outcome[i] << endl;
		
	}*/
	
	delete[] temp;

	return;
}

template<class E>
void codeHelp(HuffNode<E>* root , char* temp,Code<E> &code,int level)
{
	if (NULL==root)
	{
		return;
	}

	if (root->isLeaf())
	{
		code.vals[code.length] = ((LeafNode<E>*)root)->val();
		temp[level] = '\0';
		code.produceSpaceOutcome(level + 1);
		//outcome[length] = new char[level+1];
		memcpy(code.outcome[code.length], temp, level+1);
		code.length++;
		return;
	}
	
	IntlNode<E>* inNode = (IntlNode<E>*)root;

	//左子节点
	inNode->setWeight(1);
	temp[level] = '0';
	codeHelp(inNode->left(), temp, code, level+1);
		
	//右子节点
	inNode->setWeight(2);
	temp[level] = '1';
	codeHelp(inNode->right(), temp, code, level+1);	

	return;
}

//将给定树内部节点的权值初始化为0.
#endif // externHuffman
