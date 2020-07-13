#include<iostream>
#include<vector>
#include<assert.h>
#include<math.h>
#include"edgetable.h"
using namespace std;

struct count_result
{
	int trcount;
	int illusion;
};
 class SampleTable
 {
 	public:
 	EdgeTable* edge_table;
	NodeTable* node_table;
 	int size;
	int edge_count;
	int node_count;
	int valid_num;
	int illusion_valid;
	int trcount;
	int illusion;
	double q_count;
	double illusion_q;

 	SampleTable(int s)
 	{
 		size = s;
		edge_count = 0;
		node_count = 0;
		valid_num = 0;
		trcount = 0;
		illusion = 0;
		edge_table = new EdgeTable(s);
		node_table = new NodeTable(4, 2*s);
		q_count = size;
		illusion_q = q_count;
		illusion_valid = 0;

	 }
	~SampleTable ()
	 {
		delete edge_table;
		delete node_table;
	 }
			
	int count_triangle(sample_node* pos_s, sample_node* pos_d, long last_mark)
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

			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)  // only count the valid edge
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

			if (edge_table->table[edge_d].vice.timestamp<last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = next_index;
		}
		return count_join(v1, v2);
	}

	int count_illusion(sample_node* pos_s, sample_node* pos_d)
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

			v2.push_back(tmp);
			edge_d = next_index;
		}
		return count_join(v1, v2);
	}
	count_result count_both(sample_node* pos_s, sample_node* pos_d, long last_mark)
	{
		vector<unsigned long> v1;
		vector<unsigned long> v2;
		vector<unsigned long> vi1;
		vector<unsigned long> vi2;
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
			else
			{
				cout << "wrong!!!" << endl;
				assert(false); 
			}

			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)  // only count the valid edge in triangle count 
				v1.push_back(tmp);
			else
			vi1.push_back(tmp); // count all the edges, valid or invalid in illusion count
			edge_s = next_index;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     			edge_s = next_index;
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

			if (edge_table->table[edge_d].vice.timestamp < last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge in triangle count
				v2.push_back(tmp);
			else
			vi2.push_back(tmp);
			edge_d = next_index;
		}
		count_result cr;
		cr.trcount = count_join(v1, v2);
		cr.illusion = cr.trcount + count_join(vi1, vi2) + count_join(vi1, v2) + count_join(v1, vi2);
		return cr;
	}
	void link_list(sample_node* pos_s, sample_node* pos_d, int pos, unsigned long s_num, unsigned long d_num)
	{

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

	void dismiss(unsigned long s, unsigned long d, int pos)
	{

		int last_edge_s = edge_table->table[pos].pointers[last_s];  // isolate this edge from the list
		int last_edge_d = edge_table->table[pos].pointers[last_d];
		int next_edge_s = edge_table->table[pos].pointers[next_s];
		int next_edge_d = edge_table->table[pos].pointers[next_d];
		
		sample_node* pos_s = node_table->ID_to_pos(s);

		if (pos_s->first_edge == pos)
		{
			if (last_edge_s < 0) // there are no edges left for this node
			{
				pos_s = NULL;
				node_table->delete_via_ID(s);
				node_count--;
			}
			else
				pos_s->first_edge = last_edge_s;
		}

		sample_node* pos_d = node_table->ID_to_pos(d);
		if (pos_d->first_edge == pos)
		{
			if (last_edge_d < 0)
			{
				pos_d = NULL;
				node_table->delete_via_ID(d);
				node_count--;
			}
			else
				pos_d->first_edge = last_edge_d;
		}


		if (last_edge_s>=0)
		{
			if (edge_table->table[last_edge_s].s == s)
				edge_table->table[last_edge_s].set_next_s(next_edge_s);
			else
				edge_table->table[last_edge_s].set_next_d(next_edge_s);
		}

		if (next_edge_s>=0)
		{
			if (edge_table->table[next_edge_s].s == s)
				edge_table->table[next_edge_s].set_last_s(last_edge_s);
			else
				edge_table->table[next_edge_s].set_last_d(last_edge_s);
		}

		if (last_edge_d>=0)
		{
			if (edge_table->table[last_edge_d].d == d)
				edge_table->table[last_edge_d].set_next_d(next_edge_d);
			else
				edge_table->table[last_edge_d].set_next_s(next_edge_d);
		}

		if (next_edge_d>=0)
		{
			if (edge_table->table[next_edge_d].d == d)
				edge_table->table[next_edge_d].set_last_d(last_edge_d);
			else
				edge_table->table[next_edge_d].set_last_s(last_edge_d);
		}
	}
	 void insert(unsigned long s_num, unsigned long d_num, double p, long time, long land_mark,long last_mark)
	 {
		 string s = to_string(s_num);
		 string d = to_string(d_num);
		 string edge = s + d;
	 	 unsigned int pos = (*hfunc[0])((unsigned char*)(edge.c_str()), edge.length())%size;
	
		 if (edge_table->table[pos].vice.timestamp < last_mark && edge_table->table[pos].vice.timestamp>=0)  // if the vice edge is elder than the last_mark, then it is a left test edge and need to be cleaned. 
			 edge_table->table[pos].vice.reset();


		 if (edge_table->table[pos].s == 0 && edge_table->table[pos].d == 0)
		 {
			 illusion_valid++;
			 illusion_q = illusion_q - 1 + 1/pow(2, int(-(log(1 - p) / log(2)))+1);  // whatever the case, in the illusion a new edge is inserted.

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
			 }					// if the node is not in the table ,insert it



			 if (edge_table->table[pos].vice.timestamp >= 0)// there may be this case: the substream has not received a new item for a long time, and the old sample become test item, but no sample;
			 {
				 assert(edge_table->table[pos].vice.timestamp < land_mark && edge_table->table[pos].vice.timestamp >= last_mark);

				 if (p >= edge_table->table[pos].vice.priority)
				 {
					 valid_num++;
					 edge_table->replace_sample(s_num, d_num, p, time, pos);
					 link_list(pos_s, pos_d, pos, s_num, d_num);
					 count_result cr = count_both(pos_s, pos_d, last_mark);
					 trcount += cr.trcount;
					 illusion += cr.illusion;
					 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
					 edge_table->table[pos].vice.reset();
				 }
				 else
				 {
					 edge_table->replace_sample(s_num, d_num, p, time, pos);
					 link_list(pos_s, pos_d, pos, s_num, d_num);
					 illusion += count_illusion(pos_s, pos_d);
				 }

			 }
			 else{
				 edge_count++;
				 valid_num++;
				 edge_table->replace_sample(s_num, d_num, p, time, pos);	// if there is no sampled edge in this substream, this is the first one
				 link_list(pos_s, pos_d, pos, s_num, d_num);
				 count_result cr = count_both(pos_s, pos_d, last_mark);
				 trcount += cr.trcount;
				 illusion += cr.illusion;

				 q_count = q_count - 1 + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
			 }
			return;
		}

		 // if the inserted edge has already shown up and is sampled.

		 if (edge_table->table[pos].s == s_num && edge_table->table[pos].d == d_num)
		 {
			 edge_table->update_sample(pos, time);
			 if (edge_table->table[pos].vice.priority < edge_table->table[pos].priority && edge_table->table[pos].vice.timestamp <= edge_table->table[pos].timestamp)
				 edge_table->delete_vice(pos);
			 return;
		 }

		 // else if the sampled edge is in last slice

		 if (edge_table->table[pos].timestamp<land_mark)
		{
			if (p >= edge_table->table[pos].priority)// if larger than the sampled p, replace it;
			{
				assert(edge_table->table[pos].vice.timestamp >= land_mark|| edge_table->table[pos].vice.timestamp <0);
				// in this case, the vice edge is not needed any way;
				edge_table->delete_vice(pos);
				// reset the vice edge

				// replace the sample edge
				sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				double var = -1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				q_count += var;
				illusion_q += var;
				count_result cr = count_both(old_s, old_d, last_mark);
				trcount -= cr.trcount;
				illusion -= cr.illusion;
				dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
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
				}					// if the node is not in the table ,insert it
				link_list(pos_s, pos_d, pos, s_num, d_num);
				cr = count_both(pos_s, pos_d, last_mark);
				trcount += cr.trcount;
				illusion += cr.illusion;


			}
			else   // if smaller than the sampled p, check the vice edge;
			{
				if (p >= edge_table->table[pos].vice.priority) // can replace it;
					edge_table->replace_vice(s_num, d_num, p, time, pos);
			}
		}
		 else  //the sample edge is larger than the landmark
		 {
			
				 if (p >= edge_table->table[pos].priority)// if larger than the sampled p, replace it;
				 {
					 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
					 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
					 // in this case, we need to check the vice edge,

					 if (edge_table->table[pos].vice.timestamp < land_mark &&edge_table->table[pos].vice.timestamp >= 0) // then this is a test edge
					  {

						  assert(edge_table->table[pos].vice.timestamp >= last_mark);
 						  if (edge_table->table[pos].vice.priority <= edge_table->table[pos].priority)
						  {
							  cout << "pause" << endl;
						  }
						 if (p >= edge_table->table[pos].vice.priority) // the new edge can replace the test edge
						 {
							 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
							 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);

							 edge_table->delete_vice(pos); // the test edge is not needed any more;
							 illusion -= count_illusion(old_s, old_d); // in this case the sampled edge is only an illusion

							 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
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
							 }					// if the node is not in the table ,insert it

							 link_list(pos_s, pos_d, pos, s_num, d_num);
							 count_result cr = count_both(pos_s, pos_d, last_mark);
							 trcount += cr.trcount; // the new edge is valid
							 illusion += cr.illusion;


							 valid_num++;
						 }
						 else
						 {
							 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);

							 illusion -= count_illusion(old_s, old_d); // in this case the sampled edge is only an illusion
							 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
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
							 }					// if the node is not in the table ,insert it
							 link_list(pos_s, pos_d, pos,s_num, d_num);
							 illusion += count_illusion(pos_s, pos_d); // the new edge is still an illusion


						 }
					 }
					 else //else there should be no vice edge, we replace the sampled edge.
					 {
						 if(edge_table->table[pos].vice.timestamp >= 0)
						  {
							cout<<edge_table->table[pos].timestamp<<' '<<time<<' '<<edge_table->table[pos].vice.timestamp<<' '<<land_mark<<' '<< last_mark<<endl;
							assert(false);
						  }
						 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
						 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);

						 count_result cr = count_both(old_s, old_d, last_mark);
						 trcount -= cr.trcount;
						 illusion -= cr.illusion;
						 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
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
						 }					// if the node is not in the table ,insert it
						 link_list(pos_s, pos_d, pos, s_num, d_num);
						 cr = count_both(pos_s, pos_d, last_mark);
						 trcount += cr.trcount;
						 illusion += cr.illusion;


					 }
					 // reset the vice edge

				}

			}

		 return;
	 }
	 void update(long time, long last_mark)  // when the sampled edge expires, delete it and move the candidate edge one rank upper. Before this function the cross lists including this pos should be changed, and after this function the new sampled edge (valid or not) should be 
		 // added into the curresponding cross lists;
	 {
		 list_unit* cur = edge_table->tsl_head;
		 if (!cur) return;
		 while (edge_table->table[cur->pos].timestamp < time)
		 {
			 int pos = cur->pos;
			 cur = cur->next;



			 if (edge_table->table[pos].vice.timestamp < last_mark && edge_table->table[pos].vice.timestamp >= 0)  // if the vice edge is elder than the last_mark, then it is a left test edge and need to be cleaned. 
				 edge_table->table[pos].vice.reset();
	
			 if(edge_table->table[pos].vice.timestamp>=time)
			 {
				 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1);

				 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 count_result cr = count_both(old_s, old_d, last_mark);
				 trcount -= cr.trcount;
				 illusion -= cr.illusion;
				 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
				 sample_unit tmp = edge_table->table[pos];
		
				 edge_table->delete_sample(pos); // delete the expired sample;


				 valid_num--;  // the valid num decreases, but the illusion_valid do not decrease, as after the land mark this bucket will still be valid;


				 edge_table->table[pos].reset(tmp.vice.s, tmp.vice.d, tmp.vice.priority, tmp.vice.timestamp); // the vice edge is an invalid sample now
				 edge_table->table[pos].window_pos = tmp.vice.window_pos; // no need to insert a new list unit, the old list unit of the vice edge can be used;
				 sample_node* pos_s = node_table->ID_to_pos(tmp.vice.s);
				 sample_node* pos_d = node_table->ID_to_pos(tmp.vice.d);
				 if (!pos_s)
				 {
					 pos_s = node_table->insert(tmp.vice.s);
					 node_count++;
				 }
				 if (!pos_d)
				 {
					 pos_d = node_table->insert(tmp.vice.d);
					 node_count++;
				 }					// if the node is not in the table ,insert it

				 link_list(pos_s, pos_d, pos, tmp.vice.s, tmp.vice.d); // link the cross list;
				 illusion += count_illusion(pos_s, pos_d); // the inserted is invalid, therefore only the illusion is increased;


				 edge_table->table[pos].vice.s = tmp.s;
				 edge_table->table[pos].vice.d = tmp.d;
				 edge_table->table[pos].vice.timestamp = tmp.timestamp;
				 edge_table->table[pos].vice.priority = tmp.priority; // the old edge is a test edge now;
				 edge_table->table[pos].vice.window_pos = NULL;
			 }
			 else  // if there is no vice edge
			 {
				 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1)+1; // in the current circumstances, we can not forsee any edge in this bucket after the landmark;
				 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 count_result cr = count_both(old_s, old_d, last_mark);
				 trcount -= cr.trcount;
				 illusion -= cr.illusion;
				 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
				 valid_num--;
				 illusion_valid--; // the illusion valid also decrease;

				 edge_table->table[pos].vice.s = edge_table->table[pos].s;
				 edge_table->table[pos].vice.d = edge_table->table[pos].d;
				 edge_table->table[pos].vice.timestamp = edge_table->table[pos].timestamp;
				 edge_table->table[pos].vice.priority = edge_table->table[pos].priority; // the old edge is a test edge now;
				 edge_table->table[pos].vice.window_pos = NULL;


				 edge_table->delete_sample(pos);

			 }

			 if (!cur)
				break;

		 }
	 }

	 void ective()
	 {
		 trcount = illusion;
		 valid_num = illusion_valid;
		 q_count = illusion_q;
		 edge_count = illusion_valid; // in the landmark case, the 
	 }
	 
 };
 
 
