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
	golden_edge* next;
	int frequency;

	golden_edge(unsigned int s = 0)
	{
		Snode = s;
		next = NULL;
		frequency = 1;
	}
};

class Graph
{
public:
	unordered_map<unsigned int, golden_edge*> g;
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
		unordered_map<unsigned int, golden_edge*>::iterator it;
		for (it = g.begin(); it != g.end(); it++)
		{
			golden_edge* tmp = it->second;
			golden_edge* next = tmp;
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


	int count_triangle();
	void local_count(unordered_map<unsigned int, int> &cr);
	unsigned long long count_wedge(); 
	unsigned long long weighted_count_triangle();
	void weighted_local_count(unordered_map<unsigned int, unsigned long long> &cr);



	int get_edgenum()
	{
		return edgecount;
	}

};


void Graph::insert_edge(unsigned int s, unsigned int d)
{

	bool find = false;
	unordered_map<unsigned int, golden_edge*>::iterator it;
	
	unsigned int tmp;
	if(s<d)
	{
		tmp = s;
		s = d;
		d = tmp;
	}

	it = g.find(s);
	if (it ==g.end())
	{
		g[s] = new golden_edge(d);
		nodecount++;
	}
	else
	{
		golden_edge* tmp = it->second;
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
			tmp->next = it->second;
			g[s] = tmp;
		}
	}
	if (!find)
		edgecount++;

	return;
}

void Graph::delete_edge(unsigned int s, unsigned int d)
{
	bool deleted = false;
	
	unsigned int tmp;
	if(s<d)
	{
		tmp = s;
		s = d;
		d = tmp;
	}
	
	unordered_map<unsigned int, golden_edge*>::iterator it;

	it = g.find(s);
	if (it == g.end())
		return;
	else
	{
		golden_edge* tmp = it->second;
		if (tmp->Snode == d)
		{
			tmp->frequency--;
			if (tmp->frequency == 0)
			{
				g[s] = tmp->next;
				delete tmp;
				deleted = true;
				if (g[s] == NULL)
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
	if (deleted)
		edgecount--;

	return;

}

int Graph::count_triangle()
{
	vector<unsigned int> v1;
	unordered_set<unsigned int> neighbor_set;
	golden_edge* cur;
	
	int sum = 0;
	unordered_map<unsigned int, golden_edge*>::iterator it;
	for (it = g.begin(); it != g.end();it++)
	{
		unsigned int src = it->first;
		cur = it->second;
		while (cur)
		{
			v1.push_back(cur->Snode);
			neighbor_set.insert(cur->Snode);
			cur = cur->next;
		}
		for (int j = 0; j < v1.size(); j++)
		{
			unsigned int anc = v1[j];
			unordered_map<unsigned int, golden_edge*>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = it2->second;
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

void Graph::local_count(unordered_map<unsigned int, int> &cr)
{
	vector<unsigned int> v1, v2;
	golden_edge* cur;
	unordered_map<unsigned int, golden_edge*>::iterator it;
	unordered_set<unsigned int> neighbor_set;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		unsigned int src = it->first;
		cur = it->second;
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
			unordered_map<unsigned int, golden_edge*>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = it2->second;
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
	//cout << "after counting " << sum << endl;
	//	cout << "total edge num "<<edge_count << endl;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		cur = it->second;
		while (cur)
		{
			if(cr.find(cur->Snode)==cr.end())
				cr[cur->Snode] = 0;
			cur = cur->next; 
		} 
	} 
	return;
}

unsigned long long Graph::weighted_count_triangle()
{
	vector<unsigned int> v1;
	unordered_map<unsigned int, int> neighbor_freq;
	golden_edge* cur;
	
	unsigned long long sum = 0;
	unordered_map<unsigned int, golden_edge*>::iterator it;
	for (it = g.begin(); it != g.end();it++)
	{
		unsigned int src = it->first;
		cur = it->second;
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
			unordered_map<unsigned int, golden_edge*>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = it2->second;
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

void Graph::weighted_local_count(unordered_map<unsigned int, unsigned long long> &cr)
{
	vector<unsigned int> v1;
	golden_edge* cur;
	unordered_map<unsigned int, golden_edge*>::iterator it;
	unordered_map<unsigned int, int> neighbor_freq;
	
	for (it = g.begin(); it != g.end(); it++)
	{
		unsigned int src = it->first;
		cur = it->second;
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
			unordered_map<unsigned int, golden_edge*>::iterator it2 = g.find(anc);
			if (it2 == g.end())
				continue;
			else
				cur = it2->second;
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
		cur = it->second;
		while (cur)
		{
			if(cr.find(cur->Snode)==cr.end())
				cr[cur->Snode] = 0;
			cur = cur->next; 
		} 
	} 
	return;
}
