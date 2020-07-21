#include<iostream>
#include<string>
#include<vector> 
#include "Graph.h"
using namespace std;

struct timed_edge
{
	long timestamp;
	string s; 
	string d;
	timed_edge* next;
	timed_edge(string s_, string d_, int t)
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
		long current_time;
		int edgenum;
		timed_edge* tsl_head;
		timed_edge* tsl_tail;
		Graph* graph;
	 
	public:
		GoldenCounter(int w)
		{
			windowsize = w;
			current_time = 0;
			graph = new Graph;
			edgenum = 0;
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
		void insert_edge(string s, string d, long time)
		{
		/*	if(s == d)
				return;*/
			edgenum++;
			if(s < d)
			{
				string tmp = s;
				s = d;
				d = tmp;
			}
			///cout<<"inserting : "<<s<<' '<<d<<' ';
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
				edgenum--;
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
			return edgenum;
		}
		int edge_count()
		{
			return graph->get_edgenum();
		}
		
		int triangle_count()
		{
			return graph->count_triangle();
		 } 	
	
		
};
