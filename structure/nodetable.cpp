#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"hashTable.h"
#include"nodetable.h"

using namespace std;

sample_node* NodeTable::insert(string node, int edge)
{
	int min_length = 0x7FFFFFFF;
	int min_pos = -1;
	int empty_pos = -1;

	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node.c_str()), node.length()) % slice_length;
		int address = i*slice_length + pos;


		if (table[address].nodeID == "")
		{
			if (empty_pos<0)
				empty_pos = address;
		}

		if (table[address].nodeID == node)
		{
			if (edge>0)
				table[address].set_first_edge(edge);
			return &(table[address]);
		}
		else
		{
			int l = 0;
			sample_node* tmp = table[address].next;
			while (tmp)
			{
				l++;
				if (tmp->nodeID == node)
				{
					if (edge>0)
						tmp->set_first_edge(edge);
					return tmp;
				}
				tmp = tmp->next;
			}
			if (l<min_length)
			{
				min_length = l;
				min_pos = address;
			}
		}
	}

	if (empty_pos>0)
	{
		table[empty_pos].nodeID = node;
		table[empty_pos].set_first_edge(edge);
		return &(table[empty_pos]);
	}
	sample_node* tmp = table[min_pos].next;
	if (!tmp)
	{
		tmp = new sample_node(node, edge);
		table[min_pos].next = tmp;
	}
	else
	{
		sample_node* last = tmp;
		while (tmp)
		{
			last = tmp;
			tmp = tmp->next;
		}
		tmp = new sample_node(node, edge);
		last->next = tmp;
	}
	return tmp;
}

sample_node* NodeTable::set_edge(string node, int edge)
{
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node.c_str()), node.length()) % slice_length;
		int address = i*slice_length + pos;

		if (table[address].nodeID == node)
		{
			table[address].set_first_edge(edge);
			return &(table[address]);
		}

		else
		{
			sample_node* tmp = table[address].next;
			while (tmp)
			{
				if (tmp->nodeID == node)
				{
					tmp->set_first_edge(edge);
					return tmp;
				}
				tmp = tmp->next;
			}
		}
	}

	return NULL;
}



int NodeTable::get_edge(string node)
{
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node.c_str()), node.length()) % slice_length;
		int address = i*slice_length + pos;

		if (table[address].nodeID == node)
			return table[address].first_edge;
		else
		{
			sample_node* tmp = table[address].next;
			while (tmp)
			{
				if (tmp->nodeID == node)
					return tmp->first_edge;
				tmp = tmp->next;
			}
		}
	}

	return -1;
}

sample_node* NodeTable::ID_to_pos(string node)
{
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node.c_str()), node.length()) % slice_length;
		int address = i*slice_length + pos;
		if (table[address].nodeID == node)
			return &(table[address]);
		else
		{
			sample_node* tmp = table[address].next;
			while (tmp)
			{
				if (tmp->nodeID == node)
					return tmp;
				tmp = tmp->next;
			}
		}
	}
	return NULL;
}

void NodeTable::delete_via_ID(string node)
{
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node.c_str()), node.length()) % slice_length;
		int address = i*slice_length + pos;

		if (table[address].nodeID == node)
		{
			table[address].reset();
			return;
		}

		else
		{
			sample_node* tmp = table[address].next;
			sample_node* last = tmp;
			while (tmp)
			{
				if (tmp->nodeID == node)
				{
					if (last == tmp)
					{
						table[address].next = tmp->next;
						delete tmp;
					}
					else
					{
						last->next = tmp->next;
						delete tmp;
					}
					return;
				}
				last = tmp;
				tmp = tmp->next;
			}
		}
	}
	return;
}

