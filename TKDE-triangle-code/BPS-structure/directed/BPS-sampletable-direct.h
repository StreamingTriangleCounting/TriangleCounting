#include<iostream>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<assert.h>
#include<math.h>
#define type_A 0
#define type_B 1
#ifndef triangle_type
#define triangle_type 1
#endif
using namespace std;

 class BPSSampleTable
 {
 	public:
 	EdgeTable* edge_table;
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
		edge_table = new EdgeTable(s);
		node_table = new NodeTable(4, s*2);
		q_count = size;

	 }
	~BPSSampleTable ()
	 {
		delete edge_table;
		delete node_table;
	 }
			
	void modify_triangle_B(sample_node* pos_s, sample_node* pos_d, int op)
	{ 
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->in_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			unsigned int tmp = edge_table->table[edge_s].s;
			if(edge_table->table[edge_s].vice.timestamp<0)  // only count the valid edge
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_d];			
		}
			
		while (edge_d >= 0)
		{
			unsigned int tmp = edge_table->table[edge_d].d;
			if(edge_table->table[edge_d].vice.timestamp<0)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		std::vector<unsigned int> cn;
		 count_join(v1, v2, cn);
		 for(int i=0;i<cn.size();i++)
		 {
		 	pos_s->local_count += op;
		 	pos_d->local_count += op;
		 	trcount += op;
		 	(node_table->ID_to_pos(cn[i]))->local_count += op;
		 }
		v1.clear();
		vector<unsigned int>().swap(v1);
		v2.clear();
		vector<unsigned int>().swap(v2);
		cn.clear();
		vector<unsigned int>().swap(cn);
		return;
	}
	
	void modify_triangle_A(sample_node* pos_s, sample_node* pos_d, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->out_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			unsigned int tmp = edge_table->table[edge_s].d;
			if(edge_table->table[edge_s].vice.timestamp<0)
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_s];
		}

		while (edge_d >= 0)
		{
			unsigned int tmp = edge_table->table[edge_d].d;

			if(edge_table->table[edge_d].vice.timestamp<0)
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		vector<unsigned int> common_neighbor;
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->local_count += op;
			trcount += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
		}
		
		v2.clear();	
		common_neighbor.clear();
		
		edge_d = pos_d->in_edge;
		while (edge_d >= 0)
		{
			unsigned int tmp = edge_table->table[edge_d].s;

			if(edge_table->table[edge_d].vice.timestamp<0)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_d];
		}
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->local_count += op;
			trcount += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
		}
		
		
		v1.clear();	
		common_neighbor.clear();
		
		edge_s = pos_s->in_edge;
		while (edge_s >= 0)
		{
			unsigned int tmp = edge_table->table[edge_s].s;
			if(edge_table->table[edge_s].vice.timestamp<0)  // only count the valid edge
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_d];
		}
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->local_count += op;
			trcount += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
		}
		
		v1.clear();
		vector<unsigned int>().swap(v1);
		
		v2.clear();
		vector<unsigned int>().swap(v2);
		
		common_neighbor.clear();
		vector<unsigned int>().swap(common_neighbor);	
	}

	void link_list(sample_node* pos_s, sample_node* pos_d, int pos)
	{
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
			
		edge_table->table[pos].set_last_s(pos_s->out_edge);
		edge_table->table[pos].set_last_d(pos_d->in_edge);

		if (pos_s->out_edge>=0)
			edge_table->table[pos_s->out_edge].set_next_s(pos);

		if (pos_d->in_edge>=0)
			edge_table->table[pos_d->in_edge].set_next_d(pos);
		pos_s->set_edge(pos, out);
		pos_d->set_edge(pos, in);			// set the cross list;

	}

	void dismiss(sample_node* pos_s, sample_node* pos_d, int pos)
	{
		
		unsigned int old_s = pos_s->nodeID;
		unsigned int old_d = pos_d->nodeID;
		int last_edge_s = edge_table->table[pos].pointers[last_s];  // isolate this edge from the list
		int last_edge_d = edge_table->table[pos].pointers[last_d];
		int next_edge_s = edge_table->table[pos].pointers[next_s];
		int next_edge_d = edge_table->table[pos].pointers[next_d];
		
		if (pos_s->out_edge == pos)
		{
			if (last_edge_s < 0&&pos_s->in_edge<0) // there are no edges left for this node
			{
				node_table->delete_via_ID(old_s);
				node_count--;
			}
			else
				pos_s->out_edge = last_edge_s;
		}	
		
		if(!pos_d||pos_d->nodeID!=old_d)
			pos_d = node_table->ID_to_pos(old_d);	// after delete old_s, the address of old_d in the nodetable may have changed
		
		if (pos_d->in_edge == pos)
		{
			if (last_edge_d < 0 && pos_d->out_edge<0)
			{
				node_table->delete_via_ID(pos_d->nodeID);
				node_count--;
			}
			else
				pos_d->in_edge = last_edge_d;
		}


		if (last_edge_s>=0)
			edge_table->table[last_edge_s].set_next_s(next_edge_s);

		if (next_edge_s>=0)
			edge_table->table[next_edge_s].set_last_s(last_edge_s);

		if (last_edge_d>=0)
			edge_table->table[last_edge_d].set_next_d(next_edge_d);

		if (next_edge_d>=0)
			edge_table->table[next_edge_d].set_last_d(last_edge_d);
			
		return;
	}
	 void insert(unsigned int s_num, unsigned int d_num, double p, long long time, int hashindex)
	 {
		 string s = my_to_string(s_num);
		 string d = my_to_string(d_num);
		 string edge = s + d;
	 	 unsigned int pos = ((*hfunc[hashindex+1]))((unsigned char*)(edge.c_str()), edge.length())%size;
	 	 
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
			
			 if (edge_table->table[pos].vice.timestamp < 0) // if this bucket is unused, the q_count is 1 initially;
			 {
				 q_count = q_count - 1; 
				 edge_count++;
				 valid_num++;
			 }
			 else if (p >= edge_table->table[pos].vice.priority)//&&edge_table->table[pos].test.timestamp >= 0) // if p is larger than the test priority, the test priority can be deleted now;
			 {
				 valid_num++;
				 q_count -= 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1); // it used to be the largest priority
				 edge_table->delete_vice(pos);
			 }

			 if (edge_table->table[pos].vice.timestamp < 0)
			 {
			 	if(triangle_type==type_A)
				 	modify_triangle_A(pos_s, pos_d, 1);
				else
					modify_triangle_B(pos_s, pos_d, 1); 
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

			if (edge_table->table[pos].vice.timestamp<0) // if there is no test item;
			{
				q_count = q_count-1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				if(triangle_type==type_A)
					modify_triangle_A(old_s, old_d, -1);
				else
					modify_triangle_B(old_s, old_d, -1);
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
				if(triangle_type==type_A)
					modify_triangle_A(pos_s, pos_d, 1);
				else
					modify_triangle_B(pos_s, pos_d, 1);
			}
			else if (p >= edge_table->table[pos].vice.priority)
			{
				q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				valid_num++;
				dismiss(old_s, old_d, pos);
				edge_table->delete_vice(pos);
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
				if(triangle_type==type_A)
					modify_triangle_A(pos_s, pos_d, 1);
				else
					modify_triangle_B(pos_s, pos_d, 1);
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
	 void update(long long ex_time, long long de_time)  // when the sampled edge expires, delete it and move the candidate edge one rank upper. Before this function the cross lists including this pos should be changed, and after this function the new sampled edge (valid or not) should be 
		 // added into the curresponding cross lists;
	 {
		if (edge_table->expiration==-1)
			 return;
		int tsl_pos = edge_table->expiration;
		int pos = tsl_pos % size;
		
			 while (edge_table->table[pos].timestamp < ex_time)
		 {
		 	tsl_pos = edge_table->table[pos].time_list_next;
		 	
			 sample_node* pos_s = node_table->ID_to_pos(edge_table->table[pos].s);
			 sample_node* pos_d = node_table->ID_to_pos(edge_table->table[pos].d);
		
			 if (edge_table->table[pos].vice.timestamp >= 0) // if the expired edge has a test item, then the item must be double expired. In this case the sampled edge is never valid, we do not need to delete triangle, 
				 // this case is very rare, only when the stream has not been updated for a long time
			 {
				 q_count -= 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1);
				 q_count += 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1);  
				 edge_table->delete_vice(pos);
			 }
			 else // otherwise 
			 {
			 	if(triangle_type==type_A)
					modify_triangle_A(pos_s, pos_d, -1);
				else
					modify_triangle_B(pos_s, pos_d, -1);
				 valid_num--;
			 }
			 dismiss(pos_s, pos_d, pos);
			 
			 edge_table->table[pos].vice.reset(edge_table->table[pos].s, edge_table->table[pos].d, edge_table->table[pos].priority, edge_table->table[pos].timestamp, edge_table->table[pos].time_list_prev, edge_table->table[pos].time_list_next);
			 edge_table->set_time_list(edge_table->table[pos].time_list_prev, 1, pos+size);
			 edge_table->set_time_list(edge_table->table[pos].time_list_next, 0, pos+size);
			 if(edge_table->tsl_head == pos)
			 	edge_table->tsl_head = pos + size;
			if(edge_table->tsl_tail == pos)
			 	edge_table->tsl_tail = pos + size;	 

			 edge_table->table[pos].reset();
			 
			 edge_table->expiration = tsl_pos; // move expiration to next edge in the time sequence list;
			 if(tsl_pos<0)
			 	break;
			  else
				pos = tsl_pos % size;
		 }
		 
		 
		 tsl_pos = edge_table->tsl_head;
		 if(tsl_pos<0)
			 return;
		 pos = tsl_pos % size;
		 
		 while (edge_table->table[pos].vice.timestamp < de_time)
		 {
			 tsl_pos = edge_table->table[pos].vice.time_list_next; // the double expired edge mush be a test edge;
			 
			 q_count -= 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1);
			 edge_table->delete_vice(pos); // the tsl_head and the list_unit is changed in this function

			 if (edge_table->table[pos].timestamp >= 0)
			 {
				 q_count += 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1);
				 sample_node* pos_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* pos_d = node_table->ID_to_pos(edge_table->table[pos].d);
			
				 if(triangle_type==type_A)
					modify_triangle_A(pos_s, pos_d, 1);
				else
					modify_triangle_B(pos_s, pos_d, 1);
				 valid_num++;
			 }
			 else
			 {
				 q_count += 1; // if the substream has no new item since the test item expires, then the bucket is empty;
				 edge_count--;
			 }
			 if(tsl_pos<0)
			 	break;
			else
				pos = tsl_pos % size;

		}
	}

	 
 };
 
 
