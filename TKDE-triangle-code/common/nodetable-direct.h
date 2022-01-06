#include<iostream>
#include<string>
#include<vector>
#include<map>

class NodeTable
{
public:
		sample_node* table;
		int k;
		int length;
		int slice_length;

		NodeTable(int k_num, int l_num)
		{
			k = k_num;
			length = l_num;
			slice_length = length / k;
			table = new sample_node[length];
			for (int i = 0; i<length; i++)
				table[i].init(0);
		}
		~NodeTable()
		{
			for (int i = 0; i<length; i++)
			{
				if (table[i].next)
				{
					sample_node* tmp = table[i].next;
					sample_node* next = tmp;
					while (tmp)
					{
						next = tmp->next;
						delete tmp;
						tmp = next;
					}
				}
			}
			delete[]table;
		}

		sample_node* insert(unsigned int node, int out_edge_ = -1, int in_edge_=-1);

		sample_node* set_edge(unsigned int  node, int edge, int type);
		int get_edge(unsigned int node, int type);

		sample_node* ID_to_pos(unsigned int node);

		void delete_via_ID(unsigned int  node);
		void active();

};



sample_node* NodeTable::insert(unsigned int node, int out_edge_, int in_edge_)
{
	int min_length = 0x7FFFFFFF;
	int min_pos = -1;

	string node_s = my_to_string(node);

	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
		int address = i*slice_length + pos;


		if (table[address].nodeID == 0)
		{
			table[address].nodeID = node;
			table[address].set_edge(out_edge_, out);
			table[address].set_edge(in_edge_, in);
			return &(table[address]);
		}

		if (table[address].nodeID == node)
		{
			if (out_edge_>=0)
				table[address].set_edge(out_edge_, out);
			if(in_edge_>=0)
				table[address].set_edge(in_edge_, in);
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
					if (out_edge_>=0)
						table[address].set_edge(out_edge_, out);
					if(in_edge_>=0)
						table[address].set_edge(in_edge_, in);
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

	sample_node* tmp = new sample_node(node, out_edge_, in_edge_);
	tmp->next = table[min_pos].next;
	table[min_pos].next = tmp;
	return tmp;
}

sample_node* NodeTable::set_edge(unsigned int node, int edge, int type)
{
	string node_s = my_to_string(node);
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
		int address = i*slice_length + pos;

		if (table[address].nodeID == node)
		{
				table[address].set_edge(edge, type);	
				return &(table[address]);
		}

		else
		{
			sample_node* tmp = table[address].next;
			while (tmp)
			{
				if (tmp->nodeID == node)
				{
					tmp->set_edge(edge, type);
					return tmp;
				}
				tmp = tmp->next;
			}
		}
	}

	return NULL;
}



int NodeTable::get_edge(unsigned int node, int type)
{
	string node_s = my_to_string(node);
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
		int address = i*slice_length + pos;

		if (table[address].nodeID == node)
			return table[address].get_edge(type);
		else
		{
			sample_node* tmp = table[address].next;
			while (tmp)
			{
				if (tmp->nodeID == node)
					return tmp->get_edge(type);
				tmp = tmp->next;
			}
		}
	}

	return -1;
}

sample_node* NodeTable::ID_to_pos(unsigned int node)
{
	string node_s = my_to_string(node);
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
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

void NodeTable::delete_via_ID(unsigned int  node)
{
	string node_s = my_to_string(node);
	for (int i = 0; i<k; i++)
	{
		unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
		int address = i*slice_length + pos;

		if (table[address].nodeID == node)
		{
			if (table[address].next)
			{
				sample_node* cur = table[address].next;
				table[address] = (*cur);
				delete cur;
			}
			else
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

void NodeTable::active()
{
	for (int i = 0; i < length; i++)
	{
		table[i].local_count = table[i].vision_count;
		table[i].vision_count = 0;
		sample_node* tmp = table[i].next;
		while (tmp)
		{
			tmp->local_count = tmp->vision_count;
			tmp->vision_count = 0;
			tmp = tmp->next;
		}
	}
}
