#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<unordered_map>
#include<assert.h>
#ifndef setting
#include "../../common/undirected-setting.h"
#define setting
#endif
#include "asy_sampletable.h"

using namespace std;

class asy_sample
{
public:
	asy_sampletable* ss;
	int window_size;
	int group_num;
	long long current_time;
	long long* land_mark;
	long long* last_mark;
	int edge_estimate;
	int sample_size;
	double sample_prob;
	int hashindex;
	int gswitch_iter; // use a iterator to mark the group with largest group ID and has switched slice. This iterator increase from 0 to group_num and return to 0;

	asy_sample(int size, int w, int g, int hash_index)
	{
		ss = new asy_sampletable(size, g);
		hashindex = hash_index;
		group_num = g;
		window_size = w;
		current_time = 0;
		land_mark = new long long[g];
		last_mark = new long long[g];
		for(int i=0;i<g;i++)
		{
			land_mark[i] = -(g-i)*(window_size/g);
		}
		gswitch_iter = 0;
		sample_size = 0;
		edge_estimate = 0;
		sample_prob = 0;

	}
	~asy_sample()
	{
		delete ss;
		delete []land_mark;
		delete []last_mark;
	}

	void proceed(unsigned int s, unsigned int d, long long time)
	{
		if (s < d)
		{
			unsigned int tmp = s;
			s = d;
			d = tmp;
		}
		string s_string = my_to_string(s);
		string d_string = my_to_string(d);
		string e = s_string + d_string;
		double p = double((*hfunc[hashindex])((const unsigned char*)e.c_str(), e.length()) % 1000000 + 1) / 1000001;
		int group  = ((*hfunc[hashindex+2])((unsigned char*)(e.c_str()), e.length()) % group_num);
		current_time = time;
		ss->update(time - window_size);
		while (time - land_mark[gswitch_iter] >= window_size)
		{
			assert(time - land_mark[gswitch_iter] < 2 * window_size);
			last_mark[gswitch_iter] = land_mark[gswitch_iter];
			land_mark[gswitch_iter] = land_mark[gswitch_iter] + window_size;
			ss->slice_switch(last_mark[gswitch_iter], gswitch_iter);
			gswitch_iter++;
			if(gswitch_iter==group_num)
				gswitch_iter = 0;
		}
		ss->insert(s, d, group, p, time, land_mark[group], last_mark[group], hashindex);
	}

	void prepare()
	{
		sample_size = 0;
		for(int i=0;i<group_num;i++)
			sample_size += ss->valid_num[i];
		
		edge_estimate = 0;
		for(int i=0;i<group_num;i++)
		{
		int m = ss->group_size;
		double alpha = 0.7213 / (1 + (1.079 / m));
		int group_card = (double(alpha * m * m) / (ss->q_count[i]));
		if (group_card < 2.5*m)
			 group_card = -log(1 - double(ss->edge_count[i]) / m)*m;
		edge_estimate += group_card *(double(ss->valid_num[i]) / ss->edge_count[i]);
		}

		sample_prob = 	(double(sample_size) / edge_estimate) * (double(sample_size - 1) / (edge_estimate - 1)) * (double(sample_size - 2) / (edge_estimate - 2));

	}
	int count()	
	{
		return (ss->trcount) / sample_prob;
	}

	int local_count(unsigned int v)
	{
		sample_node* tmp = ss->node_table->ID_to_pos(v);
		if(!tmp)
			return 0;
		else
			return (tmp->local_count) / sample_prob; 
	}
	
	void all_local(unordered_map<unsigned int, int>& cr)
	{
		for(int i=0;i<ss->node_table->length;i++)
		{
			int tr_count = (ss->node_table->table[i].local_count) / sample_prob;
			cr[ss->node_table->table[i].nodeID] = (ss->node_table->table[i].local_count) / sample_prob;
			sample_node* tmp = ss->node_table->table[i].next;
			while(tmp)
			{
				cr[tmp->nodeID] = (tmp->local_count) / sample_prob;
				tmp = tmp->next; 
			}
		}
	}

	int valid_count()
	{
		int total_count = 0;
		for(int i=0;i<group_num;i++)
			total_count += ss->valid_num[i];
		return total_count;
	}
};
