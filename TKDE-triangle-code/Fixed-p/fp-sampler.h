#include<iostream>
#include<string>
#include<vector> 
#include<assert.h>
#include<unordered_map>
#ifndef setting 
#include "../common/undirected-setting.h"
#define setting
#endif
using namespace std;

struct timed_edge // 48 bytes per edge
{
	unsigned int s, d;
	long long timestamp;
	timed_edge* src_next;
	timed_edge* dst_next;
	timed_edge* tsl_prev;
	timed_edge* tsl_next;
	timed_edge(unsigned int s_num = 0, unsigned int d_num = 0, long long time = -1)
	{
		s = s_num;
		d = d_num;
		timestamp = time;
		src_next = NULL;
		dst_next = NULL;
		tsl_prev = NULL;
		tsl_next = NULL;
	}
};
struct node_info
{
	timed_edge* first_edge;
	int local_count;
	node_info()
	{
		first_edge = NULL;
		local_count = 0;
	}
};
class fp_sampler
{
	public:
		unordered_map<unsigned int, node_info> graph; // 4+8+8 = 20 bytes per node, max node num <= 2* max edge num
		double p;
		int hashindex;
		int trcount;
		int w; 
		timed_edge* tsl_head;
		timed_edge* tsl_tail;
		fp_sampler(double probability, int window_size, int hash_index, int max_edge_num = 100)
		{
			w = window_size;
			p = probability;
			hashindex = hash_index;
			trcount = 0;
			tsl_head = NULL;
			tsl_tail = NULL;
			graph.reserve(2*max_edge_num);
		}
		~fp_sampler()
		{
			unordered_map<unsigned int, node_info>::iterator it;
			for(it = graph.begin();it!=graph.end();it++)
			{
				timed_edge* tmp = (it->second).first_edge;
				timed_edge* next = tmp;
				while(tmp)
				{
					next = tmp->src_next;
					delete tmp;
					tmp = next; 
				}
			}
			graph.clear();
		}
		
		void print()
		{
			cout<<"printing graph: "<<endl;
			unordered_map<unsigned int, node_info>::iterator it;
			for(it = graph.begin();it!=graph.end();it++)
			{
				cout<<"node "<<it->first<<endl;
				timed_edge* tmp = (it->second).first_edge;
				while(tmp)
				{
					cout<<tmp->s<<' '<<tmp->d<<endl;
					if(tmp->s==it->first)
						tmp = tmp->src_next;
					else
						tmp = tmp->dst_next;
				}
				cout<<endl;	 
			}
			
			cout<<"printing time list:"<<endl;
			timed_edge* tmp = tsl_head;
			while(tmp)
			{
				cout<<tmp->s<<' '<<tmp->d<<' '<<tmp->timestamp<<endl;
				tmp = tmp->tsl_next;
			}
			return;
		}
		
		void count_triangle(unsigned int s, unsigned int d, int op)
		{
			unordered_map<unsigned int, node_info>::iterator it1 = graph.find(s);
			if(it1==graph.end())
				return;
			timed_edge* s_cur = (it1->second).first_edge;
			
			unordered_map<unsigned int, node_info>::iterator it2 = graph.find(d);
			if(it2==graph.end())
				return;
			timed_edge* d_cur = (it2->second).first_edge;
			
			vector<unsigned int> v1;
			vector<unsigned int> v2;
			vector<unsigned int> common_neighbor;
			while(s_cur)
			{
				if(s_cur->s==s)
				{
					v1.push_back(s_cur->d);
					s_cur = s_cur->src_next;
				}
				else if(s_cur->d==s)
				{
					v1.push_back(s_cur->s);
					s_cur = s_cur->dst_next; 
				}
			}
			
			while(d_cur)
			{
				if(d_cur->s==d)
				{
					v2.push_back(d_cur->d);
					d_cur = d_cur->src_next;
				}
				else if(d_cur->d==d)
				{
					v2.push_back(d_cur->s);
					d_cur = d_cur->dst_next; 
				}
			}
			count_join(v1, v2, common_neighbor);
			for(int i=0;i<common_neighbor.size();i++)
		{
			graph[common_neighbor[i]].local_count += op;
			trcount += op;
			(it1->second).local_count += op;
			(it2->second).local_count += op;
		}
			
		}
		void insert_edge(unsigned int s, unsigned int d, long long time)
		{
			string s_string = my_to_string(s);
			string d_string = my_to_string(d);
			string edge = s_string + d_string;
			double prob = double((*hfunc[hashindex])((const unsigned char*)edge.c_str(), edge.length()) % 1000000 + 1) / 1000001;
			if(prob>p)	
				return;
				
				
			unordered_map<unsigned int, node_info>::iterator it1 = graph.find(s);
			if(it1==graph.end())
			{
				node_info new_info;
				graph[s] = new_info;
				it1 = graph.find(s);
			}
			
			unordered_map<unsigned int, node_info>::iterator it2 = graph.find(d);
			if(it2==graph.end())
			{
				node_info new_info;
				graph[d] = new_info;
				it2 = graph.find(d);
			}
			
			timed_edge* tmp = (it1->second).first_edge;
			while(tmp)
			{
				if(tmp->s==s && tmp->d==d)
				{
					tmp->timestamp = time;
					
					if(tsl_tail!=tmp)
					{
						if(tmp->tsl_prev)
							(tmp->tsl_prev)->tsl_next = tmp->tsl_next;
						if(tmp->tsl_next)
							(tmp->tsl_next)->tsl_prev = tmp->tsl_prev;
						if(tsl_head == tmp)
							tsl_head = tmp->tsl_next; 
						tsl_tail->tsl_next = tmp;
						tmp->tsl_prev = tsl_tail;
						tmp->tsl_next = NULL;
						tsl_tail = tmp;
					}
					if(!tsl_head)
						tsl_head = tmp;
					return;	
				}
				if(tmp->s==s)
					tmp = tmp->src_next;
				else if(tmp->d==s)
					tmp = tmp->dst_next;
				else
					assert(false);
			}
			timed_edge* new_edge = new timed_edge(s, d, time);
			new_edge->src_next = (it1->second).first_edge;
			(it1->second).first_edge = new_edge;
			
			new_edge->dst_next = (it2->second).first_edge;
			(it2->second).first_edge = new_edge;
			
			if(tsl_tail)
				tsl_tail->tsl_next = new_edge;
			new_edge->tsl_prev = tsl_tail;
			new_edge->tsl_next = NULL;
			tsl_tail = new_edge;
			if(!tsl_head)
				tsl_head = new_edge;
			count_triangle(s, d, 1);
		}
		void update(long long ex_time)
		{
			if(!tsl_head)
				return;
			while(tsl_head->timestamp<ex_time)
			{
				unsigned int s = tsl_head->s;
				unsigned int d = tsl_head->d;
				count_triangle(s, d, -1); // delete triangle 
				
				// split the edge from the graph
				unordered_map<unsigned int, node_info>::iterator it1 = graph.find(s);
				assert(it1!=graph.end());
				unordered_map<unsigned int, node_info>::iterator it2 = graph.find(d);
				assert(it2!=graph.end());
				
				timed_edge* tmp = (it1->second).first_edge;
				timed_edge* parent = tmp;
				while(tmp)
				{
					if(tmp == tsl_head)
					{
						if(parent!=tmp){		// if not the first edge in the edge list of s
							if(parent->s==s)
								parent->src_next = tmp->src_next;
							else if(parent->d==s)
								parent->dst_next = tmp->src_next;
							else
								assert(false);
							}
						else
						{
							if(tmp->src_next==NULL)	// no edges left in the edge list of s
								graph.erase(it1); // delete the node
							else
								(it1->second).first_edge = tmp->src_next; 
						}
						break;	
					}
					parent = tmp;
					if(tmp->s==s)
						tmp = tmp->src_next;
					else if(tmp->d==s)
						tmp = tmp->dst_next;
					else
						assert(false);
				}
				
				
				tmp = (it2->second).first_edge;
				parent = tmp;
				while(tmp)
				{
					if(tmp == tsl_head)
					{
						if(parent!=tmp){		// if not the first edge in the edge list of s
							if(parent->s==d)
								parent->src_next = tmp->dst_next;
							else if(parent->d==d)
								parent->dst_next = tmp->dst_next;
							else
								assert(false);
							}
						else
						{
							if(tmp->dst_next==NULL)	// no edges left in the edge list of s
								graph.erase(it2);
							else
								(it2->second).first_edge = tmp->dst_next; 
						}
						break;	
					}
					parent = tmp;
					if(tmp->s==d)
						tmp = tmp->src_next;
					else if(tmp->d==d)
						tmp = tmp->dst_next;
					else
						assert(false);
				}
				
				
				// delete the edge and move the tsl_head pointer
				tmp = tsl_head;
				tsl_head = tsl_head->tsl_next;
				delete tmp;
			}
		}
		void process_edge(unsigned int s, unsigned int d, long long time)
		{
			if(s<d)
			{
				unsigned int tmp =s;
				s = d;
				d = tmp;
			}
			update(time-w);
			insert_edge(s, d, time);
		}
		int count()
		{
			return trcount/(p*p*p);
		}
		int local_count(unsigned int v)
		{
			unordered_map<unsigned int, node_info>::iterator it = graph.find(v);
			if(it==graph.end())
				return 0;
			else
			 	return (it->second).local_count/(p*p*p);
		}
		void all_local(unordered_map<unsigned int, int> &cr)
		{
				unordered_map<unsigned int, node_info>::iterator it;
				for(it = graph.begin();it!=graph.end();it++)
					cr[it->first] = (it->second).local_count/(p*p*p);
				return;
		}
};
