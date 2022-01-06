#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
using namespace std;
#include <math.h> 
#include<algorithm>
#include<unordered_set>


struct golden_edge
{
	unsigned int Snode;
	int frequency;
	golden_edge* next;
	
	golden_edge(unsigned int s=0)
	{
		Snode = s;
		next = NULL;
		frequency = 1;
	}
};


struct golden_node
{
	unsigned int ID;
	golden_edge* out_edge;
	golden_edge* in_edge;

	golden_node(unsigned int s=0)
	{
		ID = s;
		out_edge = NULL;
		in_edge = NULL;
	}
};

class Graph
{
public:
	unordered_map<unsigned int, golden_node> g;
	int edgecount;
	int nodecount;
	int trianglecount;

	Graph()
	{
		edgecount = 0;
		nodecount = 0;
		trianglecount = 0;
	}
	~Graph()
	{
		unordered_map<unsigned int, golden_node>::iterator it;
		for (it = g.begin(); it != g.end(); it++)
		{
			golden_edge* tmp = (it->second).out_edge;
			golden_edge* next = tmp;
			while (tmp)
			{
				next = tmp->next;
				delete tmp;
				tmp = next;
			}
			
			tmp = (it->second).in_edge;
			while (tmp)
			{
				next = tmp->next;
				delete tmp;
				tmp = next;
			}
		}
		g.clear();
	}

	void delete_edge(unsigned int s, unsigned int d);

	void insert_edge(unsigned int s, unsigned int d);


	int count_triangle_A();
	unsigned long long weighted_count_A();
	
	int count_triangle_B();
	unsigned long long weighted_count_B();
	
	void local_count_A(unordered_map<unsigned int, int> &cr);
	void weighted_local_A(unordered_map<unsigned int, unsigned long long> &cr);	

	void local_count_B(unordered_map<unsigned int, int> &cr);
	void weighted_local_B(unordered_map<unsigned int, unsigned long long> &cr);
	
	int get_edgenum()
	{
		return edgecount;
	}

};

void Graph::insert_edge(unsigned int s, unsigned int d) // directed edge s->d
{

	bool find = false;
	unordered_map<unsigned int, golden_node>::iterator it;
	
	it = g.find(s);
	if (it ==g.end())
	{
		golden_node gn(s);
		gn.out_edge = new golden_edge(d);
		g[s] = gn;
		nodecount++;
	}
	else
	{
		golden_edge* tmp = (it->second).out_edge;
		while (tmp)
		{
			if (tmp->Snode == d)
			{
				tmp->frequency++;
				find = true;
				break;
			}
			tmp = tmp->next;
		}
		if (!find)
		{
			golden_edge* tmp = new golden_edge(d);
			tmp->next = (it->second).out_edge;
			(it->second).out_edge = tmp;
		}
	}
	
	
	it = g.find(d);
	if (it ==g.end())
	{
		golden_node gn(d);
		gn.in_edge = new golden_edge(s);
		g[d] = gn;
		nodecount++;
	}
	else
	{
		golden_edge* tmp = (it->second).in_edge;
		while (tmp)
		{
			if (tmp->Snode == s)
			{
				tmp->frequency++;
				find = true;
				break;
			}
			tmp = tmp->next;
		}
		if (!find)
		{
			golden_edge* tmp = new golden_edge(s);
			tmp->next = (it->second).in_edge;
			(it->second).in_edge = tmp;
		}
	}
	
	if (!find)
		edgecount++;

	return;
}

void Graph::delete_edge(unsigned int s, unsigned int d)
{
	bool deleted = false;
	
	unordered_map<unsigned int, golden_node>::iterator it;

	it = g.find(s);
	if (it == g.end())
		return;
	else
	{
		golden_edge* tmp = (it->second).out_edge;
		if(!tmp)
			return;
		if (tmp->Snode == d)
		{
			tmp->frequency--;
			if (tmp->frequency == 0)
			{
				(it->second).out_edge = tmp->next;
				delete tmp;
				deleted = true;
				if ((it->second).out_edge == NULL && (it->second).in_edge == NULL)
				{
					g.erase(it);
					nodecount--;
				}
			}
		}
		else
		{
			golden_edge* p = tmp;
			tmp = tmp->next;
			while (tmp)
			{
				if (tmp->Snode == d)
				{
					tmp->frequency--;
					if (tmp->frequency == 0)
					{
						p->next = tmp->next;
						delete tmp;
						deleted = true;
					}
					break;
				}
				p = tmp;
				tmp = tmp->next;
			}
		}
	}
	
	it = g.find(d);
	if (it == g.end())
		return;
	else
	{
		golden_edge* tmp = (it->second).in_edge;
		if(!tmp)
			return;
		if (tmp->Snode == s)
		{
			tmp->frequency--;
			if (tmp->frequency == 0)
			{
				(it->second).in_edge = tmp->next;
				delete tmp;
				deleted = true;
				if ((it->second).out_edge == NULL && (it->second).in_edge == NULL)
				{
					g.erase(it);
					nodecount--;
				}
			}
		}
		else
		{
			golden_edge* p = tmp;
			tmp = tmp->next;
			while (tmp)
			{
				if (tmp->Snode == s)
				{
					tmp->frequency--;
					if (tmp->frequency == 0)
					{
						p->next = tmp->next;
						delete tmp;
						deleted = true;
					}
					break;
				}
				p = tmp;
				tmp = tmp->next;
			}
		}
	}
	
	
	
	if (deleted)
		edgecount--;

	return;

}

int Graph::count_triangle_A()
{
	vector<unsigned int> v1;
	unordered_set<unsigned int> neighbor_set;
	golden_edge* cur;
	
	int sum = 0;
	unordered_map<unsigned int, golden_node>::iterator it;
	for (it = g.begin(); it != g.end();it++)
	{
		unsigned int src = it->first;
	//	cout<<"neighbor of "<<src<<endl;
		cur = (it->second).out_edge;
		while (cur)
		{
		//	cout<<cur->Snode<<' ';
			v1.push_back(cur->Snode);
			neighbor_set.insert(cur->Snode);
			cur = cur->next;
		}
	//	cout<<endl;
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned int anc = v1[j];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(neighbor_set.find(cur->Snode)!=neighbor_set.end()){
					sum++;
				//	cout<<"found triangle "<<src<<' '<<anc<<' '<<cur->Snode<<endl;
				}
				cur = cur->next;
			}
		}
		v1.clear();
		neighbor_set.clear();
	}
	return sum;
}

int Graph::count_triangle_B()
{
	vector<unsigned int> v1;
	unordered_set<unsigned int> neighbor_set;
	golden_edge* cur;
	
	int sum = 0;
	unordered_map<unsigned int, golden_node>::iterator it;
	for (it = g.begin(); it != g.end();it++)
	{
		unsigned int src = it->first;
		cur = (it->second).out_edge;
		while (cur)
		{
			if(cur->Snode<src)
				v1.push_back(cur->Snode);
			cur = cur->next;
		}
		
		cur = (it->second).in_edge;
		while (cur)
		{
			if(cur->Snode<src)
				neighbor_set.insert(cur->Snode);
			cur = cur->next;
		}
		
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned int anc = v1[j];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(neighbor_set.find(cur->Snode)!=neighbor_set.end())
					sum++;
				cur = cur->next;
			}
		}
		v1.clear();
		neighbor_set.clear();
	}
	return sum;
}



unsigned long long Graph::weighted_count_A()
{
	vector<unsigned int> v1;
	unordered_map<unsigned int, int> neighbor_freq;
	golden_edge* cur;
	
	unsigned long long sum = 0;
	unordered_map<unsigned int, golden_node>::iterator it;
	for (it = g.begin(); it != g.end();it++)
	{
		unsigned int src = it->first;
		cur = (it->second).out_edge;
		while (cur)
		{
			v1.push_back(cur->Snode);
			neighbor_freq[cur->Snode] = cur->frequency;
			cur = cur->next;
		}
		
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned int anc = v1[j];
			int fre1 = neighbor_freq[anc];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(neighbor_freq.find(cur->Snode)!=neighbor_freq.end())
				{
					int fre2 = cur->frequency;
					int fre3 = neighbor_freq[cur->Snode];
					sum+= ((unsigned long long)fre1*fre2*fre3);
				}
				cur = cur->next;
			}
		}
		v1.clear();
		neighbor_freq.clear();
	}
	return sum;
}

unsigned long long Graph::weighted_count_B()
{
	vector<unsigned int> v1;
	unordered_map<unsigned int, int> out_freq;
	unordered_map<unsigned int, int> in_freq;
	golden_edge* cur;
	
	unsigned long long sum = 0;
	unordered_map<unsigned int, golden_node>::iterator it;
	for (it = g.begin(); it != g.end();it++)
	{
		unsigned int src = it->first;
		cur = (it->second).out_edge;
		while (cur)
		{
			v1.push_back(cur->Snode);
			out_freq[cur->Snode] = cur->frequency;
			cur = cur->next;
		}
		
		cur = (it->second).in_edge;
		while (cur)
		{
			in_freq[cur->Snode] = cur->frequency;
			cur = cur->next;
		}
		
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned int anc = v1[j];
			int fre1 = out_freq[anc];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(in_freq.find(cur->Snode)!=in_freq.end())
				{
					int fre2 = cur->frequency;
					int fre3 = in_freq[cur->Snode];
					sum+= ((unsigned long long)fre1*fre2*fre3);
				}
				cur = cur->next;
			}
		}
		v1.clear();
		in_freq.clear();
		out_freq.clear();
	}
	return sum;
}

void Graph::local_count_A(unordered_map<unsigned int, int> &cr)
{
	vector<unsigned int> v1;
	golden_edge* cur;
	unordered_map<unsigned int, golden_node>::iterator it;
	unordered_set<unsigned int> neighbor_set;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		unsigned int src = it->first;
		cur = (it->second).out_edge;
		while (cur)
		{
			v1.push_back(cur->Snode);
			neighbor_set.insert(cur->Snode);
			cur = cur->next;
		}
		int total_count = 0;
		
		for (int j = 0; j < v1.size(); j++)
		{
			int sum = 0;
			unsigned int anc = v1[j];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(neighbor_set.find(cur->Snode)!=neighbor_set.end())
				{
					sum++;
					if(cr.find(cur->Snode)!=cr.end())
						cr[cur->Snode]++;
					else
						cr[cur->Snode] = 1;
				}
				cur = cur->next;
			}
			if(cr.find(anc)!=cr.end())
				cr[anc]+= sum;
			else
				cr[anc] = sum;	
			total_count += sum;
		}
		v1.clear();
		neighbor_set.clear();
		if(cr.find(src)!=cr.end())
			cr[src] += total_count;
		else
			cr[src] = total_count;
	}
	
	for (it = g.begin(); it != g.end(); it++)
	{
		if(cr.find(it->first)==cr.end())
			cr[it->first] = 0;
	} 
}

void Graph::weighted_local_A(unordered_map<unsigned int, unsigned long long> &cr)
{
	vector<unsigned int> v1;
	golden_edge* cur;
	unordered_map<unsigned int, golden_node>::iterator it;
	unordered_map<unsigned int, int> neighbor_freq;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		unsigned int src = it->first;
		cur = (it->second).out_edge;
		while (cur)
		{
			v1.push_back(cur->Snode);
			neighbor_freq[cur->Snode] = cur->frequency;
			cur = cur->next;
		}
		unsigned long long total_count = 0;
		
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned long long sum = 0;
			unsigned int anc = v1[j];
			int fre1 = neighbor_freq[anc];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(neighbor_freq.find(cur->Snode)!=neighbor_freq.end())
				{
					int fre2 = cur->frequency;
					int fre3 = neighbor_freq[cur->Snode];
					unsigned long long triangle_fre = ((unsigned long long)fre1*fre2*fre3);
					
					sum += triangle_fre;
					
					if(cr.find(cur->Snode)!=cr.end())
						cr[cur->Snode] += triangle_fre;
					else
						cr[cur->Snode] = triangle_fre;
				}
				cur = cur->next;
			}
			if(cr.find(anc)!=cr.end())
				cr[anc]+= sum;
			else
				cr[anc] = sum;	
			total_count += sum;
		}
		v1.clear();
		neighbor_freq.clear();
		if(cr.find(src)!=cr.end())
			cr[src] += total_count;
		else
			cr[src] = total_count;
	}
	
	for (it = g.begin(); it != g.end(); it++)
	{
		if(cr.find(it->first)==cr.end())
			cr[it->first] = 0;
	} 
	return;
}

void Graph::local_count_B(unordered_map<unsigned int, int> &cr)
{
	vector<unsigned int> v1;
	golden_edge* cur;
	unordered_map<unsigned int, golden_node>::iterator it;
	unordered_set<unsigned int> neighbor_set;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		unsigned int src = it->first;
		
		cur = (it->second).out_edge;
		while (cur)
		{
			if(cur->Snode<src) 
				v1.push_back(cur->Snode);
			cur = cur->next;
		}
		
		cur = (it->second).in_edge;
		while (cur)
		{
			if(cur->Snode<src)
				neighbor_set.insert(cur->Snode);
			cur = cur->next;
		}
		
		int total_count = 0;
		
		for (int j = 0; j < v1.size(); j++)
		{
			int sum = 0;
			unsigned int anc = v1[j];
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(neighbor_set.find(cur->Snode)!=neighbor_set.end())
				{
					sum++;
					if(cr.find(cur->Snode)!=cr.end())
						cr[cur->Snode]++;
					else
						cr[cur->Snode] = 1;
				}
				cur = cur->next;
			}
			if(cr.find(anc)!=cr.end())
				cr[anc]+= sum;
			else
				cr[anc] = sum;	
			total_count += sum;
		}
		v1.clear();
		neighbor_set.clear();
		if(cr.find(src)!=cr.end())
			cr[src] += total_count;
		else
			cr[src] = total_count;
	}
	
	for (it = g.begin(); it != g.end(); it++)
	{
		if(cr.find(it->first)==cr.end())
			cr[it->first] = 0;
	} 
}

void Graph::weighted_local_B(unordered_map<unsigned int, unsigned long long> &cr)
{
	vector<unsigned int> v1;
	golden_edge* cur;
	unordered_map<unsigned int, golden_node>::iterator it;
	unordered_map<unsigned int, int> out_neighbor_freq;
	unordered_map<unsigned int, int> in_neighbor_freq;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		unsigned int src = it->first;
		
		cur = (it->second).out_edge;
		while (cur)
		{
			if(cur->Snode<src) 
			{
				v1.push_back(cur->Snode);
				out_neighbor_freq[cur->Snode] = cur->frequency;
			}
			cur = cur->next;
		}
		
		cur = (it->second).in_edge;
		while (cur)
		{
			if(cur->Snode<src)
				in_neighbor_freq[cur->Snode] = cur->frequency;
			cur = cur->next;
		}
		
		unsigned long long total_count = 0;
		
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned long long sum = 0;
			unsigned int anc = v1[j];
			int fre1 = out_neighbor_freq[anc];
			
			unordered_map<unsigned int, golden_node>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = (it2->second).out_edge;
			while (cur)
			{
				if(in_neighbor_freq.find(cur->Snode)!=in_neighbor_freq.end())
				{
					int fre2 = cur->frequency;
					int fre3 = in_neighbor_freq[cur->Snode];
					unsigned long long triangle_fre = ((unsigned long long)fre1*fre2*fre3);
					
					sum += triangle_fre;
					if(cr.find(cur->Snode)!=cr.end())
						cr[cur->Snode]+= triangle_fre;
					else
						cr[cur->Snode] = triangle_fre;;
				}
				cur = cur->next;
			}
			if(cr.find(anc)!=cr.end())
				cr[anc]+= sum;
			else
				cr[anc] = sum;	
			total_count += sum;
		}
		v1.clear();
		out_neighbor_freq.clear();
		in_neighbor_freq.clear();
		if(cr.find(src)!=cr.end())
			cr[src] += total_count;
		else
			cr[src] = total_count;
	}
	
	for (it = g.begin(); it != g.end(); it++)
	{
		if(cr.find(it->first)==cr.end())
			cr[it->first] = 0;
	} 
}
