#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<assert.h>
#ifndef setting 
#include "../../common/undirected-setting.h"
#define setting
#endif
#include "sampletable-weighted.h"


using namespace std;

class sample
{
public:
	SampleTable* st;
	int window_size;
	long long current_time;
	long long land_mark;
	long long last_mark;
	int edge_estimate;
	double sample_prob;
	int hashindex;

	sample(int size, int w, int hi)
	{
		st = new SampleTable(size);
		window_size = w;
		current_time = 0;
		land_mark = 0;
		last_mark = 0;
		hashindex = hi;
	}
	~sample()
	{
		delete st;
	}
	void proceed(unsigned int s, unsigned int d, long long time)
	{
		if (s < d)
		{
			unsigned int tmp = s;
			s = d;
			d = tmp;
		}
		double p = (rand()+1)/double(RAND_MAX+2);
		current_time = time;
		st->update(time-window_size, land_mark, last_mark);
		if (time - land_mark >= window_size)
		{
			assert(time - land_mark < 2*window_size);
			last_mark = land_mark;
			land_mark = land_mark + window_size;
			st->ective();
		}
		st->insert(s, d, p, time, land_mark, last_mark, hashindex);
	}

	void prepare()
	{
		int m = st->size;
		double alpha = 0.7213 / (1 + (1.079 / m));
		int total_num = (double(alpha * m * m) / (st->q_count));
		int sample_num = st->valid_num;
		if (total_num < 2.5*m)
			total_num = -log(1 - double(st->edge_count) / m)*m;
		total_num = total_num *(double(st->valid_num) / st->edge_count);
		edge_estimate = total_num;;
		sample_prob = (double(sample_num) / total_num) * (double(sample_num - 1) / (total_num - 1)) * (double(sample_num - 2) / (total_num - 2));

	}
	unsigned long long count()
	{
		return (st->trcount) / sample_prob;
	}
	unsigned int local_count(unsigned int v)
	{
		sample_node* tmp = st->node_table->ID_to_pos(v);
		if (!tmp)
			return 0;
		else
			return (tmp->local_count) / sample_prob;
	}
	void all_local(unordered_map<unsigned int, unsigned int>& cr)
	{
		for(int i=0;i<st->node_table->length;i++)
		{
			cr[st->node_table->table[i].nodeID] = (st->node_table->table[i].local_count) / sample_prob;
			sample_node* tmp = st->node_table->table[i].next;
			while(tmp)
			{
				cr[tmp->nodeID] = (tmp->local_count) / sample_prob;
				tmp = tmp->next; 
			}
		}
	}

};

