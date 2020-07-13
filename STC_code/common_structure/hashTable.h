#include<string>
#include<vector>
#include<algorithm>
#include "hashfunction.h"
using namespace std;
unsigned int (*hfunc[14])(const unsigned char*, unsigned int)=
{
    BOB1,
    BOB2,
    BOB3,
    BOB4,
    PJW,
    DJB,
    CRC32,
    OCaml,
    SML,
    STL,
    FNV32,
    Hsieh,
    RSHash	
};

template<class T>
class hashTableNode
{
public:
	T value;
	string key;
	hashTableNode<T> *next;
};
template<class T>
class hashTable
{
public:
	hashTableNode<T> **table;
	int tableSize;
	hashTable(int s):tableSize(s)
	{
		table = new hashTableNode<T>*[s];
		memset(table, NULL, tableSize * sizeof(hashTableNode<T>*));
	}
	hashTable()
	{
	}
	void init(int s)
	{
		tableSize = s;
		table = new hashTableNode<T>*[s];
		for(int i=0;i<s;i++)
			table[i]=NULL;
	//	memset(table, NULL, tableSize * sizeof(hashTableNode<T>*));
	}
	~hashTable()
	{
		cleanupHashTable();
		delete [] table;
	}
	void cleanupHashTable()
	{
		hashTableNode<T>*np, *tmp;
		for (int i = 0; i < tableSize; ++i)
		{
			if (table[i] != NULL)
			{
				np = table[i];
				while (np != NULL)
				{
					tmp = np->next;
					delete np;
					np = tmp;
				}
			}
		}
	}
	void insert(string key, T value)
	{
		hashTableNode<T> *np;
		bool inTable;
		unsigned int hash= (*hfunc[0])((unsigned char*)(key.c_str()), key.length());
		np = table[hash%tableSize];
		inTable = false;
		for (; np != NULL; np = np->next)
		{
			if (np->key == key && np->value == value)
			{
				inTable = true;
				break;
			}
		}
		if (!inTable)
		{
			hashTableNode<T>* newNode = new hashTableNode<T>;
			newNode->key = key;
			newNode->value = value;
			newNode->next = table[hash%tableSize];
			table[hash%tableSize] = newNode;
		}
	}
	
	void delete_item(string key)
	{
	hashTableNode<T> *np, *last;
	unsigned int hash= (*hfunc[0])((unsigned char*)(key.c_str()), key.length());
	np = table[hash%tableSize];
	last = np;
	for (; np != NULL; np = np->next)
	{
		if (np->key == key)
		{
			if(last == np)
			{
				table[hash%tableSize] = np->next;
				delete np;
				return;
			}
			else
			{
				last->next = np->next;
				delete np;
				return;
			}
		}
		last = np;	
	}
	
	return;
	}
 T find_value(string key)
 {
 	hashTableNode<T> *np;
	unsigned int hash= (*hfunc[0])((unsigned char*)(key.c_str()), key.length());
	np = table[hash%tableSize];
	for (; np != NULL; np = np->next)
	{
		if (np->key == key)
		{
			return np->value;
		}
	}
	return NULL;
 }
};

