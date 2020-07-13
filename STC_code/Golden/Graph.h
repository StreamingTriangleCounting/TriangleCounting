#include<iostream>
#include<string>
#include<vector>
#include<map>
#include <math.h> 
#include<algorithm>
#include "../common_structure/count_join.cpp"
using namespace std;

struct golden_edge
{
	string Snode;
	golden_edge* next;
	int frequency;

	golden_edge(string s)
	{
		Snode = s;
		next = NULL;
		frequency = 1;
	}
};

struct golden_node
{
	string Bnode;
	golden_edge* head;

	golden_node(string s)
	{
		Bnode = s;
		head = NULL;
	}
};

class Graph
{
private:
	map<string, int> nodemap;
	vector<golden_node*> edgelists;
	int edgecount;
	int nodecount;
	int trianglecount;

public:
	Graph()
	{
		edgecount = 0;
		nodecount = 0;
		trianglecount = 0;
	}
	~Graph()
	{
		golden_edge* e_tmp1;
		golden_edge* e_tmp2;
		for (int i = 0; i < edgelists.size(); i++)
		{
			if (!edgelists[i])
				continue;
			e_tmp1 = edgelists[i]->head;
			while (e_tmp1)
			{
				e_tmp2 = e_tmp1;
				e_tmp1 = e_tmp1->next;
				delete e_tmp2;
			}
			delete edgelists[i];
		}
		edgelists.clear();
	}

	void delete_edge(string s, string d);

	void insert_edge(string s, string d);

	void increase_triangle(int v_num1, int v_num2);
	void decrease_triangle(int v_num1, int v_num2);

	int count_triangle();

	int get_edgenum()
	{
		return edgecount;
	}

};
void Graph::insert_edge(string s, string d)
{

	int v_num;
	string tmp1, tmp2;
	if (s < d)
	{
		tmp1 = d;
		tmp2 = s;
	}
	else
	{
		tmp1 = s;
		tmp2 = d;
	}


	map<string, int>::iterator it;

	it = nodemap.find(tmp1);
	if (it == nodemap.end())
	{
		nodemap[tmp1] = nodecount;
		v_num = nodecount;
		nodecount++;
		edgelists.push_back(new golden_node(tmp1));
	}
	else
	{
		v_num = it->second;
	}
	

	golden_edge* cur;

	bool inserted = false;
	cur = edgelists[v_num]->head;

	while (cur)
	{
		if (cur->Snode == tmp2)
		{
			//			cout<<"checked: "<<tmp->Snode<<' '<<d<<endl;
			cur->frequency++;
			inserted = true;
			break;
		}
		cur = cur->next;
	}

	if (!inserted){

		golden_edge* new_cur = new golden_edge(tmp2);
		new_cur->next = edgelists[v_num]->head;
		edgelists[v_num]->head = new_cur;

	}

	if (!inserted)
		edgecount++;


}

void Graph::delete_edge(string s, string d)
{
	int v_num;

	string tmp1, tmp2;
	if (s < d)
	{
		tmp1 = d;
		tmp2 = s;
	}
	else
	{
		tmp1 = s;
		tmp2 = d;
	}


	bool deleted = false;
	map<string, int>::iterator it;

	it = nodemap.find(tmp1);
	if (it == nodemap.end())
		return;
	else
		v_num = it->second;


	golden_edge* cur;
	golden_edge* parent;

	cur = edgelists[v_num]->head;
	parent = cur;


	while (cur)
	{
		if (cur->Snode == tmp2)
		{
			cur->frequency--;
			if (cur->frequency == 0)
			{
				deleted = true;
				//cout << "pause" << endl;
				if (edgelists[v_num]->head == cur) // delete edge
				{
					edgelists[v_num]->head = cur->next;
					delete cur;
				}
				else
				{
					parent->next = cur->next;
					delete cur;
				}
			}
			break;
		}
		parent = cur;
		cur = cur->next;
	}


	if (deleted)
		edgecount--;

	return;

}

int Graph::count_triangle()
{
	vector<string> v1, v2;
	golden_edge* cur;
	string node1, node2;
	int sum = 0;
//	cout << "before counting " << sum << endl;
	int edge_count = 0;
	for (int i = 0; i < edgelists.size(); i++)
	{
		cur = edgelists[i]->head;
		while (cur)
		{
			v1.push_back(cur->Snode);
			cur = cur->next;
		}
		edge_count += v1.size();
		vector<string> v3 = v1;
		for (int j = 0; j < v1.size(); j++)
		{
			int tmp;
			map<string, int>::iterator it;

			it = nodemap.find(v1[j]);
			if (it == nodemap.end())
				continue;
			else
				tmp = it->second;
			cur = edgelists[tmp]->head;
			while (cur)
			{
				v2.push_back(cur->Snode);
				cur = cur->next;
			}
			sum += count_join(v3, v2);
			v2.clear();
			vector<string>().swap(v2);
		}
		v1.clear();
		vector<string>().swap(v1);
	}
	//cout << "after counting " << sum << endl;
//	cout << "total edge num "<<edge_count << endl;
	return sum;
}

//############################################################################################### Continuous Version  #############################################################

/*void Graph::insert_edge(string s, string d)
{

	int v_num1, v_num2;


	map<string, int>::iterator it;

	it = nodemap.find(s);
	if (it == nodemap.end())
	{
		nodemap[s] = nodecount;
		v_num1 = nodecount;
		nodecount++;
		edgelists.push_back(new golden_node(s));
	}
	else
	{
		v_num1 = it->second;
	}
	it = nodemap.find(d);
	if (it == nodemap.end())
	{
		nodemap[d] = nodecount;
		v_num2 = nodecount;
		nodecount++;
		edgelists.push_back(new golden_node(d));
	}
	else
	{
		v_num2 = it->second;
	}

	golden_edge* tmp;

	bool inserted1 = false;
	tmp = edgelists[v_num1]->head;

	while (tmp)
	{
		if (tmp->Snode == d)
		{
			tmp->frequency++;
			inserted1 = true;
			break;
		}
		tmp = tmp->next;
	}

	if (!inserted1){

		golden_edge* new_tmp = new golden_edge(d);
		new_tmp->next = edgelists[v_num1]->head;
		edgelists[v_num1]->head = new_tmp;

	}



	bool inserted2 = false;
	tmp = edgelists[v_num2]->head;

	while (tmp)
	{
		if (tmp->Snode == s)
		{
			tmp->frequency++;
			inserted2 = true;
			break;
		}
		tmp = tmp->next;
	}

	if (!inserted2){

		golden_edge* tmp = new golden_edge(s);
		tmp->next = edgelists[v_num2]->head;
		edgelists[v_num2]->head = tmp;

	}

	if (!inserted1&&!inserted2)
	{
		edgecount++;
		increase_triangle(v_num1, v_num2);
	}
}

void Graph::delete_edge(string s, string d)
{

	int v_num1, v_num2;
	bool deleted = false;
	map<string, int>::iterator it;

	it = nodemap.find(s);
	if (it == nodemap.end())
		return;
	else
		v_num1 = it->second;

	it = nodemap.find(d);
	if (it == nodemap.end())
		return;
	else
		v_num2 = it->second;

	golden_edge* tmp;
	golden_edge* parent;

	tmp = edgelists[v_num1]->head;
	parent = tmp;


	while (tmp)
	{
		if (tmp->Snode == d)
		{
			tmp->frequency--;
			if (tmp->frequency == 0)
			{
				deleted = true;
				if (edgelists[v_num1]->head == tmp) // delete edge
				{
					edgelists[v_num1]->head = tmp->next;
					delete tmp;
				}
				else
				{
					parent->next = tmp->next;
					delete tmp;
				}
		
			}
			break;
		}
		parent = tmp;
		tmp = tmp->next;
	}

	tmp = edgelists[v_num2]->head;


	while (tmp)
	{
		if (tmp->Snode == s)
		{
			tmp->frequency--;
			if (tmp->frequency == 0)
			{
				deleted = true;
				if (edgelists[v_num2]->head == tmp) // delete edge
				{
					edgelists[v_num2]->head = tmp->next;
					delete tmp;
				}
				else
				{
					parent->next = tmp->next;
					delete tmp;
				}
			}
	
			break;
		}
		parent = tmp;
		tmp = tmp->next;
	}

	if (deleted){
		edgecount--;
		decrease_triangle(v_num1, v_num2);
	}
	return;

}

void Graph::increase_triangle(int v_num1, int v_num2)
{
	vector<string> list1;
	vector<string> list2;

	golden_edge* tmp = edgelists[v_num1]->head;

	while (tmp != NULL)
	{
		list1.push_back(tmp->Snode);
		tmp = tmp->next;
	}

	tmp = edgelists[v_num2]->head;

	while (tmp != NULL)
	{
		list2.push_back(tmp->Snode);
		tmp = tmp->next;
	}
	//	cout<<list1.size()<<' '<<list2.size()<<endl; 
	trianglecount += count_join(list1, list2);
}

void Graph::decrease_triangle(int v_num1, int v_num2)
{
	vector<string> list1;
	vector<string> list2;

	golden_edge* tmp = edgelists[v_num1]->head;

	while (tmp)
	{
		list1.push_back(tmp->Snode);
		tmp = tmp->next;
	}

	tmp = edgelists[v_num2]->head;

	while (tmp)
	{
		list2.push_back(tmp->Snode);
		tmp = tmp->next;
	}

	trianglecount -= count_join(list1, list2);
}
int Graph::count_triangle()
{
	return trianglecount;
}*/
