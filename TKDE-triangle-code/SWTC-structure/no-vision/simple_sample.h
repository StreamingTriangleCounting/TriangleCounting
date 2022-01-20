#include<iostream>
#include<vector>
#include<assert.h>
#include<math.h>
using namespace std;

 class Simple_Sample
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
	
	Simple_Sample(int s)
	{
		size = s;
		edge_count = 0;
		node_count = 0;
		valid_num = 0;
		trcount = 0;
		edge_table = new EdgeTable(s);
		node_table = new NodeTable(4, 2*s);
		q_count = size;
	}
	~Simple_Sample()
	{
		delete edge_table;
		delete node_table;
	}
	
	int count_triangle(sample_node* pos_s, sample_node* pos_d)
	{
		vector<unsigned int > v1;
		vector<unsigned int > v2;
		unsigned int  s_num = pos_s->nodeID;
		unsigned int  d_num = pos_d->nodeID;
		int edge_s = pos_s->first_edge;
		int edge_d = pos_d->first_edge;
		while (edge_s >= 0)
		{
			unsigned int  tmp;
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

			if (edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp||edge_table->table[edge_s].vice.timestamp<0)  // only count the valid edge
				v1.push_back(tmp);
			edge_s = next_index;
		}

		while (edge_d >= 0)
		{
			unsigned int  tmp;
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

			if (edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp||edge_table->table[edge_d].vice.timestamp<0)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = next_index;
		}
		return count_join(v1, v2);
	}
		void link_list(sample_node* pos_s, sample_node* pos_d, int pos, unsigned int  s_num, unsigned int  d_num)
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
    	void dismiss(unsigned int  s, unsigned int  d, int pos) // isolate this edge from the list, conducted before delete an edge.
	{

		int last_edge_s = edge_table->table[pos].pointers[last_s];  
		int last_edge_d = edge_table->table[pos].pointers[last_d];
		int next_edge_s = edge_table->table[pos].pointers[next_s];
		int next_edge_d = edge_table->table[pos].pointers[next_d];
		
		sample_node* pos_s = node_table->ID_to_pos(s);

		if (pos_s->first_edge == pos)
		{
			//cout<<"delete source node"<<endl;
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
			//cout<<"delete destinition node"<<endl;
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
		//	if (last_edge_s == 14042)
		//		cout << "pause" << endl;
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
		//	if (last_edge_d == 14042)
		//		cout << "pause" << endl;
			if (edge_table->table[last_edge_d].d == d)
				edge_table->table[last_edge_d].set_next_d(next_edge_d);
			else
				edge_table->table[last_edge_d].set_next_s(next_edge_d);
		}

		if (next_edge_d>=0)
		{
			//if (pos == 14042)
		//		cout << "pause" << endl;
			if (edge_table->table[next_edge_d].d == d)
				edge_table->table[next_edge_d].set_last_d(last_edge_d);
			else
				edge_table->table[next_edge_d].set_last_s(last_edge_d);
		}
	}

	 void insert(unsigned int  s_num, unsigned int  d_num, double p, long long time, long long land_mark,long long last_mark, int hashindex)
	 {
		 string s = to_string(s_num);
		 string d = to_string(d_num);
		 string edge = s + d;
	 	 unsigned int pos = (*hfunc[hashindex+1])((unsigned char*)(edge.c_str()), edge.length())%size;
	


		 if (edge_table->table[pos].s == 0 && edge_table->table[pos].d == 0)
		 {
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



			 if (edge_table->table[pos].vice.timestamp >= 0)// there may be this case: the substream has not received a new item for a long long time, and the old sample become test item, but no sample;
			 {
				 assert(edge_table->table[pos].vice.timestamp < land_mark && edge_table->table[pos].vice.timestamp >= last_mark);

				 if (p >= edge_table->table[pos].vice.priority)
				 {
					 valid_num++;
					 edge_table->replace_sample(s_num, d_num, p, time, pos);
					 link_list(pos_s, pos_d, pos, s_num, d_num);
					 int cr = count_triangle(pos_s, pos_d);
					 trcount += cr;
					 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
					 edge_table->delete_vice(pos);
				 }
				 else
				 {
					 edge_table->replace_sample(s_num, d_num, p, time, pos);
					 link_list(pos_s, pos_d, pos, s_num, d_num); // remain invalid
				 }

			 }
			 else{
				 edge_count++;
				 valid_num++;
				 edge_table->replace_sample(s_num, d_num, p, time, pos);	// if there is no sampled edge in this substream, this is the first one
				 link_list(pos_s, pos_d, pos, s_num, d_num);
				 int cr = count_triangle(pos_s, pos_d);
				 trcount += cr;

				 q_count = q_count - 1 + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				// cout <<p<<' '<< int((log(1 - p) / log(2)))<< endl;
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
				int cr = count_triangle(old_s, old_d);
				trcount -= cr;
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
				cr = count_triangle(pos_s, pos_d);
				trcount += cr;
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

							 edge_table->delete_vice(pos); // the test edge is not needed any more;
				
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
							 int cr = count_triangle(pos_s, pos_d);
							 trcount += cr;
							 valid_num++;
						 }
						 else
						 {
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
						 }
					 }
					 else //else there should be no vice edge, we replace the sampled edge.
					 {
					 	 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
					     sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
						 if(edge_table->table[pos].vice.timestamp >= 0)
						  {
							cout<<edge_table->table[pos].timestamp<<' '<<time<<' '<<edge_table->table[pos].vice.timestamp<<' '<<land_mark<<' '<< last_mark<<endl;
							assert(false);
						  }
						 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
					
						 int cr = count_triangle(old_s, old_d);
						 trcount -= cr;
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
						 cr = count_triangle(pos_s, pos_d);
						 trcount += cr;

					 }
				
				}

			}

		//	if (time == 40597)
		//		cout << "pause 1" << endl;
		 return;
	 }
	 
	 
	  void update(long long time)  // when the sampled edge expires, delete it and move the candidate edge one rank upper. Before this function the cross lists including this pos should be changed, and after this function the new sampled edge (valid or not) should be 
		 // added into the curresponding cross lists;
		 //time = T-N
	 {
		 list_unit* cur = edge_table->tsl_head;
		 if (!cur) return;
		 while (edge_table->table[cur->pos].timestamp < time)
		 {
			 int pos = cur->pos;
			 cur = cur->next;


			 if(edge_table->table[pos].vice.timestamp>=time)
			 {
				 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 int cr = count_triangle(old_s, old_d);
				 trcount -= cr;
				 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
				 sample_unit tmp = edge_table->table[pos];
		
				 edge_table->delete_sample(pos); // delete the expired sample;


				 valid_num--;  // the valid num decreases;


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

				 edge_table->table[pos].vice.s = tmp.s;
				 edge_table->table[pos].vice.d = tmp.d;
				 edge_table->table[pos].vice.timestamp = tmp.timestamp;
				 edge_table->table[pos].vice.priority = tmp.priority; // the old edge is a test edge now;
				 edge_table->table[pos].vice.window_pos = NULL;// the test edge does not need a linked list unit
			 }
			 else  // if there is no vice edge
			 {
				 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 int cr = count_triangle(old_s, old_d);
				 trcount -= cr;
				 dismiss(edge_table->table[pos].s, edge_table->table[pos].d, pos);
				 valid_num--;

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
	 
	 void slice_switch(int land_mark) // the window arrives at a landmark, any test edge behind this landmark loses effect
	 {
	 	for(int i=0;i<size;i++)
		 {
		 	if(edge_table->table[i].vice.timestamp>=0 && edge_table->table[i].vice.timestamp<land_mark) // a out-dated test edge
		 {
		 	q_count -=  1/pow(2, int(-(log(1 - edge_table->table[i].vice.priority) / log(2)))+1);
		 	edge_table->delete_vice(i); 
		 	if(edge_table->table[i].timestamp>=0) // if there is an invalid sample edge
		 	{
		 		 sample_node* pos_s = node_table->ID_to_pos(edge_table->table[i].s);
				 sample_node* pos_d = node_table->ID_to_pos(edge_table->table[i].d);
				 int cr = count_triangle(pos_s, pos_d);
				 trcount += cr;
				 q_count+=  1/pow(2, int(-(log(1 - edge_table->table[i].priority) / log(2)))+1);
				 valid_num++;  // a new valid edge
			}
			else // if not this position is empty
			{
				q_count+=1;
				edge_count--;
			}
		 }
		}	
	}  
};

