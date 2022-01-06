#include<iostream>
#include<string>
#include<vector> 
#ifndef n_type
#define n_type unsigned int
#endif
#include "Graph-directed.h"
#define type_A 0
#define type_B 1
#ifndef triangle_type
#define triangle_type 1
#endif
using namespace std;

struct timed_edge
{
	long long timestamp;
	n_type s; 
	n_type d;
	timed_edge* next;
	timed_edge(n_type s_, n_type d_, long long t)
	{
		s = s_;
		d = d_;
		timestamp = t;
		next = NULL;
	}
};

class GoldenCounter
{
	private:
		int windowsize;
		long long current_time;
		timed_edge* tsl_head;
		timed_edge* tsl_tail;
		int duplicated_edgenum;
		Graph* graph;
	 
	public:
		GoldenCounter(int w)
		{
			windowsize = w;
			current_time = 0;
			graph = new Graph;
			duplicated_edgenum = 0;
			tsl_head = NULL;
			tsl_tail = NULL;
		}
		~GoldenCounter()
		{
			timed_edge* cur = tsl_head;
			tsl_head = NULL;
			tsl_tail = NULL;
			timed_edge* next = cur;
			while (cur)
			{
				next = cur->next;
				delete cur;
				cur = next;
			}
			delete graph; 
		}
		int get_edgenum()
		{
			return graph->get_edgenum();
		}
		void insert_edge(n_type s, n_type d, long long time)
		{
			duplicated_edgenum++;
			current_time = time;
			timed_edge* e = new timed_edge(s, d, current_time);
			if (!tsl_head)
				tsl_head = e;
			if (tsl_tail)
				tsl_tail->next = e;
			tsl_tail = e;
			graph->insert_edge(s, d);
			
			timed_edge* cur = tsl_head;
			timed_edge* next;
			while (cur->timestamp < current_time - windowsize)
			{
				duplicated_edgenum--;
				next = cur->next;
				graph->delete_edge(cur->s, cur->d);
				delete cur;
				cur = next;
				tsl_head = next;
				if(!cur)
				    return;
			}
			//cout<<"insert finished"<<endl;
			return; 
		}
		
		int duplicate_count()
		{
			return duplicated_edgenum;
		}
		int edge_count()
		{
			return graph->get_edgenum();
		}
		
		int triangle_count()
		{
			if(triangle_type==type_A)
				return graph->count_triangle_A();
			else
				return graph->count_triangle_B();
		}
		unsigned long long weighted_triangle_count()
		{
			if(triangle_type==type_A)
				return graph->weighted_count_A();
			else
				return graph->weighted_count_B();
		} 
		
		void local_count(unordered_map<unsigned int, int> &cr)
		{
			if(triangle_type==type_A)
				graph->local_count_A(cr);
			else
				graph->local_count_B(cr);
		}
		
		void weighted_local(unordered_map<unsigned int, unsigned long long> &cr)
		{
			if(triangle_type==type_A)
				graph->weighted_local_A(cr);
			else
				graph->weighted_local_B(cr);
		}
		
};
