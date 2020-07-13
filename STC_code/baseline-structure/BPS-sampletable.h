#include<iostream>
#include"BPS-edgetable.h"
#include<vector>
#include<assert.h>
#include<math.h>
using namespace std;

 class BPSSampleTable
 {
 	public:
 	BPSEdgeTable* edge_table;
	NodeTable* node_table;
 	int size;
	int edge_count;
	int node_count;
	int valid_num;
	int trcount;
	double q_count;

 	BPSSampleTable(int s)
 	{
 		size = s;
		edge_count = 0;
		node_count = 0;
		valid_num = 0;
		trcount = 0;
		edge_table = new BPSEdgeTable(s);
		node_table = new NodeTable(4, 2*s);
		q_count = size;

	 }
	~BPSSampleTable ()
	 {
		delete edge_table;
		delete node_table;
	 }
			
	int count_triangle(sample_node* pos_s, sample_node* pos_d)
	{
		vector<unsigned long> v1;
		vector<unsigned long> v2;
		unsigned long s_num = pos_s->nodeID;
		unsigned long d_num = pos_d->nodeID;
		int edge_s = pos_s->first_edge;
		int edge_d = pos_d->first_edge;
		while (edge_s >= 0)
		{
			unsigned long tmp;
			int next_index;
			if (edge_table->table[edge_s].s == s_num)
			{
				tmp = edge_table->table[edge_s].d;
				next_index = edge_table->table[edge_s].pointers[last_s];
			}
			else if (edge_table->table[edge_s].d == s_num)
			{
				tmp = edge_table->table[edge_s].s;
				next_index = edge_table->table[edge_s].pointers[last_d];
			}

			if (edge_table->table[edge_s].test.timestamp<0)  // only count the valid edge
				v1.push_back(tmp);
			edge_s = next_index;
		}

		while (edge_d >= 0)
		{
			unsigned long tmp;
			int next_index;
			if (edge_table->table[edge_d].d == d_num)
			{
				tmp = edge_table->table[edge_d].s;
				next_index = edge_table->table[edge_d].pointers[last_d];
			}
			else if (edge_table->table[edge_d].s == d_num)
			{
				tmp = edge_table->table[edge_d].d;
				next_index = edge_table->table[edge_d].pointers[last_s];
			}

			if (edge_table->table[edge_d].test.timestamp<0)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = next_index;
		}
		return count_join(v1, v2);
	}

	void link_list(sample_node* pos_s, sample_node* pos_d, int pos)
	{
		unsigned long s_num = pos_s->nodeID;
		unsigned long d_num = pos_d->nodeID;
				// if the node is not in the table ,insert it

		edge_table->table[pos].set_last_s(pos_s->first_edge);
		edge_table->table[pos].set_last_d(pos_d->first_edge);

		if (pos_s->first_edge>=0)
		{
			if (edge_table->table[pos_s->first_edge].s == s_num)
				edge_table->table[pos_s->first_edge].set_next_s(pos);
			else
				edge_table->table[pos_s->first_edge].set_next_d(pos);
		}

		if (pos_d->first_edge>=0)
		{
			if (edge_table->table[pos_d->first_edge].s == d_num)
				edge_table->table[pos_d->first_edge].set_next_s(pos);
			else
				edge_table->table[pos_d->first_edge].set_next_d(pos);
		}
		pos_s->set_first_edge(pos);
		pos_d->set_first_edge(pos);			// set the cross list;

	}

	void dismiss(sample_node* pos_s, sample_node* pos_d, int pos)
	{

		unsigned long old_s = pos_s->nodeID;
		unsigned long old_d = pos_d->nodeID;
		int last_edge_s = edge_table->table[pos].pointers[last_s];  // isolate this edge from the list
		int last_edge_d = edge_table->table[pos].pointers[last_d];
		int next_edge_s = edge_table->table[pos].pointers[next_s];
		int next_edge_d = edge_table->table[pos].pointers[next_d];

		if (pos_s->first_edge == pos)
		{
			//cout<<"delete source node"<<endl;
			if (last_edge_s < 0) // there are no edges left for this node
			{
				node_table->delete_via_ID(pos_s->nodeID);
				node_count--;
			}
			else
				pos_s->first_edge = last_edge_s;
		}
		if (pos_d->first_edge == pos)
		{
			//cout<<"delete destinition node"<<endl;
			if (last_edge_d < 0)
			{
				node_table->delete_via_ID(pos_d->nodeID);
				node_count--;
			}
			else
				pos_d->first_edge = last_edge_d;
		}


		if (last_edge_s>=0)
		{
			if (edge_table->table[last_edge_s].s == old_s)
				edge_table->table[last_edge_s].set_next_s(next_edge_s);
			else
				edge_table->table[last_edge_s].set_next_d(next_edge_s);
		}

		if (next_edge_s>=0)
		{
			if (edge_table->table[next_edge_s].s == old_s)
				edge_table->table[next_edge_s].set_last_s(last_edge_s);
			else
				edge_table->table[next_edge_s].set_last_d(last_edge_s);
		}

		if (last_edge_d>=0)
		{
			if (edge_table->table[last_edge_d].d == old_d)
				edge_table->table[last_edge_d].set_next_d(next_edge_d);
			else
				edge_table->table[last_edge_d].set_next_s(next_edge_d);
		}

		if (next_edge_d>=0)
		{
			if (edge_table->table[next_edge_d].d == old_d)
				edge_table->table[next_edge_d].set_last_d(last_edge_d);
			else
				edge_table->table[next_edge_d].set_last_s(last_edge_d);
		}
	}
	 void insert(unsigned long s_num, unsigned long d_num, double p, long time)
	 {
		 string s = to_string(s_num);
		 string d = to_string(d_num);
		 string edge = s + d;
	 	 unsigned int pos = (*hfunc[0])((unsigned char*)(edge.c_str()), edge.length())%size;


		 if (edge_table->table[pos].s == 0 && edge_table->table[pos].d == 0) // no sample edge, then 2 cases: the bucket is empty, or only has test item;
		 {
			 edge_table->replace_sample(s_num, d_num, p, time, pos);

			 sample_node* pos_s = node_table->ID_to_pos(s_num);
			 sample_node* pos_d = node_table->ID_to_pos(d_num);
			 if (!pos_s)
			 {
				 pos_s = node_table->insert(s_num);
				 node_count++;
			 }
			 if (!pos_d)
			 {
				 pos_d = node_table->insert(d_num);
				 node_count++;
			 }

			 link_list(pos_s, pos_d, pos);
			
			 if (edge_table->table[pos].test.timestamp < 0) // if this bucket is unused, the q_count is 1 initially;
			 {
				 q_count = q_count - 1; 
				 edge_count++;
				 valid_num++;
			 }

			 else if (p >= edge_table->table[pos].test.priority&&edge_table->table[pos].test.timestamp >= 0) // if p is larger than the test priority, the test priority can be deleted now;
			 {
				 valid_num++;
				 q_count -= 1/pow(2, int(-(log(1 - edge_table->table[pos].test.priority) / log(2)))+1); // it used to be the largest priority
				 edge_table->delete_test(pos);

			 }

			 if (edge_table->table[pos].test.timestamp < 0)
			 {
				 trcount += count_triangle(pos_s, pos_d);
				 q_count += 1/pow(2, int(-(log(1 - p) / log(2)))+1);
			 }

			return;
		}

		 // if the inserted edge has already shown up and is sampled.

		 if (edge_table->table[pos].s == s_num && edge_table->table[pos].d == d_num)
		 {
			 edge_table->update_sample(pos, time);
			 return;
		 }

		 // else if the sampled edge is in last slice

		if (p >= edge_table->table[pos].priority)// if larger than the sampled p, replace it;
		{


			// replace the sample edge
			sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
			sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);

			if (edge_table->table[pos].test.timestamp<0) // if there is no test item;
			{
				q_count = q_count-1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				trcount -= count_triangle(old_s, old_d);
				dismiss(old_s, old_d, pos);
				edge_table->replace_sample(s_num, d_num, p, time, pos);
				sample_node* pos_s = node_table->ID_to_pos(s_num);
				sample_node* pos_d = node_table->ID_to_pos(d_num);
				if (!pos_s)
				{
					pos_s = node_table->insert(s_num);
					node_count++;
				}
				if (!pos_d)
				{
					pos_d = node_table->insert(d_num);
					node_count++;
				}
				link_list(pos_s, pos_d, pos);
				trcount += count_triangle(pos_s, pos_d);
			}
			else if (p >= edge_table->table[pos].test.priority)
			{
				q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].test.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				valid_num++;
				dismiss(old_s, old_d, pos);
				edge_table->delete_test(pos);
				edge_table->replace_sample(s_num, d_num, p, time, pos);
				sample_node* pos_s = node_table->ID_to_pos(s_num);
				sample_node* pos_d = node_table->ID_to_pos(d_num);
				if (!pos_s)
				{
					pos_s = node_table->insert(s_num);
					node_count++;
				}
				if (!pos_d)
				{
					pos_d = node_table->insert(d_num);
					node_count++;
				}
				link_list(pos_s, pos_d, pos);
				trcount += count_triangle(pos_s, pos_d);
			}
			else
			{
				dismiss(old_s, old_d, pos);
				edge_table->replace_sample(s_num, d_num, p, time, pos);
				sample_node* pos_s = node_table->ID_to_pos(s_num);
				sample_node* pos_d = node_table->ID_to_pos(d_num);
				if (!pos_s)
				{
					pos_s = node_table->insert(s_num);
					node_count++;
				}
				if (!pos_d)
				{
					pos_d = node_table->insert(d_num);
					node_count++;
				}
				link_list(pos_s, pos_d, pos);
			}
		}
		   // if smaller than the sampled p£¬ nothing need to be done;
		 return;
	 }
	 void update(long ex_time, long de_time)  // when the sampled edge expires, delete it and move the candidate edge one rank upper. Before this function the cross lists including this pos should be changed, and after this function the new sampled edge (valid or not) should be 
		 // added into the curresponding cross lists;
	 {
		 if (!edge_table->expiration)
			 return;
			 while (edge_table->table[edge_table->expiration->pos].timestamp < ex_time)
		 {
			 int pos = edge_table->expiration->pos;
			 sample_node* pos_s = node_table->ID_to_pos(edge_table->table[pos].s);
			 sample_node* pos_d = node_table->ID_to_pos(edge_table->table[pos].d);

			 if (edge_table->table[pos].test.timestamp >= 0) // if the expired edge has a test item, then the item must be double expired. In this case the sampled edge is never valid, we do not need to delete triangle, 
				 // this case is very rare, only when the stream has not been updated for a long time
			 {
				 q_count -= 1/pow(2, int(-(log(1 - edge_table->table[pos].test.priority) / log(2)))+1);
				 q_count += 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1);  
				 edge_table->delete_test(pos);
			 }
			 else // otherwise 
			 {
				 trcount -= count_triangle(pos_s, pos_d);
				 valid_num--;
			 }

			 dismiss(pos_s, pos_d, pos);
			 edge_table->table[pos].test.priority = edge_table->table[pos].priority;
			 edge_table->table[pos].test.timestamp = edge_table->table[pos].timestamp;
			 edge_table->table[pos].test.window_pos = edge_table->table[pos].window_pos;

			 edge_table->table[pos].reset();
			 if (edge_table->expiration->next)
				 edge_table->expiration = edge_table->expiration->next;
			 else
				 break;
		 }
		 list_unit* cur = edge_table->tsl_head;
		 while (edge_table->table[cur->pos].test.timestamp < de_time)
		 {
			 int pos = cur->pos;
			 cur = cur->next;
			 q_count -= 1/pow(2, int(-(log(1 - edge_table->table[pos].test.priority) / log(2)))+1);
			 edge_table->delete_test(pos); // the tsl_head and the list_unit is changed in this function


			 if (edge_table->table[pos].timestamp >= 0)
			 {
				 q_count += 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1);
				 sample_node* pos_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* pos_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 trcount += count_triangle(pos_s, pos_d);
				 valid_num++;
			 }
			 else
			 {
				 q_count += 1; // if the substream has no new item since the test item expires, then the bucket is empty;
				 edge_count--;
			 }
			 if (!cur)
				 break;

		}
	}

	 
 };
 
 
