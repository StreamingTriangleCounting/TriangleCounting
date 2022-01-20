#include<iostream>
#include<vector>
#include<assert.h>
#include<math.h>
#define type_A 0
#define type_B 1
#ifndef triangle_type
#define triangle_type 1
#endif
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
		
	
		void modify_triangle_B(sample_node* pos_s, sample_node* pos_d, long long last_mark, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		bool print = false;
//		if(s_num==6&&d_num==5)
	//		print = true; 
		int edge_s = pos_s->in_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].d == s_num);
			unsigned int tmp = edge_table->table[edge_s].s;
			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)   // only count the valid edge
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_d];
		}

		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].s == d_num);
			unsigned int tmp = edge_table->table[edge_d].d;
			if(print)
				cout<<"child of dst "<<tmp<<endl;
			if (edge_table->table[edge_d].vice.timestamp<last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		if(print)
			cout<<v1.size()<<' '<<v2.size()<<endl;
		vector<unsigned int> common_neighbor;
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			if(print)
				cout<<"triangle found"<<endl;
			node_table->ID_to_pos(common_neighbor[i])->local_count += op;
			trcount += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
		}
		v1.clear();
		vector<unsigned int>().swap(v1);
		
		v2.clear();
		vector<unsigned int>().swap(v2);
	}
	
	void modify_triangle_A(sample_node* pos_s, sample_node* pos_d, long long last_mark, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->out_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].s == s_num);
			unsigned int tmp = edge_table->table[edge_s].d;
			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_s];
		}

		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].s == d_num);
			unsigned int tmp = edge_table->table[edge_d].d;

			if (edge_table->table[edge_d].vice.timestamp<last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp) // only count the valid edge
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
			assert(edge_table->table[edge_d].d == d_num);
			unsigned int tmp = edge_table->table[edge_d].s;

			if (edge_table->table[edge_d].vice.timestamp<last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge
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
			assert(edge_table->table[edge_s].d == s_num);
			unsigned int tmp = edge_table->table[edge_s].s;
			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)  // only count the valid edge
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
	}
	
	void modify_triangle(sample_node* pos_s, sample_node* pos_d, long long last_mark, int op, int type)
	{
		if(type==type_A)
			modify_triangle_A(pos_s, pos_d, last_mark, op);
		else
			modify_triangle_B(pos_s, pos_d, last_mark, op);
	} 
	void modify_illusion_B(sample_node* pos_s, sample_node* pos_d, long long land_mark, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->in_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].d == s_num);
			unsigned int tmp = edge_table->table[edge_s].s;
			if(edge_table->table[edge_s].timestamp>=land_mark)   // only count the valid edge
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_d];
		}

		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].s == d_num);
			unsigned int tmp = edge_table->table[edge_d].d;
			if(edge_table->table[edge_d].timestamp>=land_mark)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		vector<unsigned int> common_neighbor;
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->vision_count += op;
			illusion += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
		}
		v1.clear();
		vector<unsigned int>().swap(v1);
		
		v2.clear();
		vector<unsigned int>().swap(v2);
	}
	
	void modify_illusion_A(sample_node* pos_s, sample_node* pos_d, long long land_mark, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->out_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].s == s_num);
			unsigned int tmp = edge_table->table[edge_s].d;
			if(edge_table->table[edge_s].timestamp>=land_mark)
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_s];
		}

		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].s == d_num);
			unsigned int tmp = edge_table->table[edge_d].d;

			if(edge_table->table[edge_d].timestamp>=land_mark)// only count the valid edge
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		vector<unsigned int> common_neighbor;
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->vision_count += op;
			illusion += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
		}
		
		v2.clear();	
		common_neighbor.clear();
		
		edge_d = pos_d->in_edge;
		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].d == d_num);
			unsigned int tmp = edge_table->table[edge_d].s;

			if(edge_table->table[edge_d].timestamp>=land_mark)  // only count the valid edge
				v2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_d];
		}
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->vision_count += op;
			illusion += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
		}	
		
		
		
		v1.clear();	
		common_neighbor.clear();
		
		edge_s = pos_s->in_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].d == s_num);
			unsigned int tmp = edge_table->table[edge_s].s;
			if(edge_table->table[edge_s].timestamp>=land_mark)  // only count the valid edge
				v1.push_back(tmp);
			edge_s = edge_table->table[edge_s].pointers[last_d];
		}
		count_join(v1, v2, common_neighbor);
		for(int i=0;i<common_neighbor.size();i++)
		{
			node_table->ID_to_pos(common_neighbor[i])->vision_count += op;
			illusion += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
		}	
	}
	
	void modify_illusion(sample_node* pos_s, sample_node* pos_d, long long land_mark, int op, int type)
	{
		if(type==type_A)
			modify_illusion_A(pos_s, pos_d, land_mark, op);
		else
			modify_illusion_B(pos_s, pos_d, land_mark, op);
	} 
	
	void modify_both_B(sample_node* pos_s, sample_node* pos_d, long long last_mark, long long land_mark, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		vector<unsigned int> vi1;
		vector<unsigned int> vi2;
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->in_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].d == s_num);
			unsigned int tmp = edge_table->table[edge_s].s;
			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)  // only count the valid edge in triangle count 
				v1.push_back(tmp);
			if(edge_table->table[edge_s].timestamp>=land_mark)	
				vi1.push_back(tmp); // only count edges in current slice, as at next landmark only these edges will be active.
			edge_s = edge_table->table[edge_s].pointers[last_d];
		}

		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].s == d_num);
			unsigned int tmp = edge_table->table[edge_d].d;
			if (edge_table->table[edge_d].vice.timestamp < last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge in triangle count
				v2.push_back(tmp);
			if(edge_table->table[edge_d].timestamp>=land_mark)
				vi2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		vector<unsigned int> cn;
		vector<unsigned int> vcn;

		count_join(v1, v2, cn);
		count_join(vi1, vi2, vcn);
		for(int i=0;i<cn.size();i++)
		{
			int u = cn[i];
			(node_table->ID_to_pos(u))->local_count += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
			trcount += op;
		}
			for(int i=0;i<vcn.size();i++)
		{
			int u = vcn[i];
			(node_table->ID_to_pos(u))->vision_count += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
			illusion += op;
		}
		v1.clear();
		vector<unsigned int>().swap(v1);
		v2.clear();
		vector<unsigned int>().swap(v2);
		vi1.clear();
		vector<unsigned int>().swap(vi1);
		vi2.clear();
		vector<unsigned int>().swap(vi2);
		cn.clear();
		vector<unsigned int>().swap(cn);
		vcn.clear();
		vector<unsigned int>().swap(vcn);
		return;
	}
	
	void modify_both_A(sample_node* pos_s, sample_node* pos_d, long long last_mark, long long land_mark, int op) //op = 1 means to add, op = -1 means to minus.
	{
		vector<unsigned int> v1;
		vector<unsigned int> v2;
		vector<unsigned int> vi1;
		vector<unsigned int> vi2;
		
		unsigned int s_num = pos_s->nodeID;
		unsigned int d_num = pos_d->nodeID;
		int edge_s = pos_s->out_edge;
		int edge_d = pos_d->out_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].s == s_num);
			unsigned int tmp = edge_table->table[edge_s].d;
			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)  // only count the valid edge in triangle count 
				v1.push_back(tmp);
			if(edge_table->table[edge_s].timestamp>=land_mark)	
				vi1.push_back(tmp); // only count edges in current slice, as at next landmark only these edges will be active.
			edge_s = edge_table->table[edge_s].pointers[last_s];
		}

		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].s == d_num);
			unsigned int tmp = edge_table->table[edge_d].d;

			if (edge_table->table[edge_d].vice.timestamp < last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge in triangle count
				v2.push_back(tmp);
			if(edge_table->table[edge_d].timestamp>=land_mark)
				vi2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_s];
		}
		vector<unsigned int> cn;
		vector<unsigned int> vcn;

		count_join(v1, v2, cn);
		count_join(vi1, vi2, vcn);
		for(int i=0;i<cn.size();i++)
		{
			int u = cn[i];
			(node_table->ID_to_pos(u))->local_count += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
			trcount += op;
		}
			for(int i=0;i<vcn.size();i++)
		{
			int u = vcn[i];
			(node_table->ID_to_pos(u))->vision_count += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
			illusion += op;
		}
		
		v2.clear();	
		vi2.clear();
		cn.clear();
		vcn.clear();
		
		
		edge_d = pos_d->in_edge;
		while (edge_d >= 0)
		{
			assert(edge_table->table[edge_d].d == d_num);
			unsigned int tmp = edge_table->table[edge_d].s;

			if (edge_table->table[edge_d].vice.timestamp < last_mark || edge_table->table[edge_d].vice.timestamp>edge_table->table[edge_d].timestamp)  // only count the valid edge in triangle count
				v2.push_back(tmp);
			if(edge_table->table[edge_d].timestamp>=land_mark)
				vi2.push_back(tmp);
			edge_d = edge_table->table[edge_d].pointers[last_d];
		}
		count_join(v1, v2, cn);
		count_join(vi1, vi2, vcn);
		for(int i=0;i<cn.size();i++)
		{
			int u = cn[i];
			(node_table->ID_to_pos(u))->local_count += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
			trcount += op;
		}
			for(int i=0;i<vcn.size();i++)
		{
			int u = vcn[i];
			(node_table->ID_to_pos(u))->vision_count += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
			illusion += op;
		}
		
		
		v1.clear();	
		vi1.clear();
		cn.clear();
		vcn.clear();
		
		edge_s = pos_s->in_edge;
		while (edge_s >= 0)
		{
			assert(edge_table->table[edge_s].d == s_num);
			unsigned int tmp = edge_table->table[edge_s].s;
			if (edge_table->table[edge_s].vice.timestamp<last_mark || edge_table->table[edge_s].vice.timestamp>edge_table->table[edge_s].timestamp)  // only count the valid edge in triangle count 
				v1.push_back(tmp);
			if(edge_table->table[edge_s].timestamp>=land_mark)	
				vi1.push_back(tmp); // only count edges in current slice, as at next landmark only these edges will be active.
			edge_s = edge_table->table[edge_s].pointers[last_d];
		}
		count_join(v1, v2, cn);
		count_join(vi1, vi2, vcn);
		for(int i=0;i<cn.size();i++)
		{
			int u = cn[i];
			(node_table->ID_to_pos(u))->local_count += op;
			pos_s->local_count += op;
			pos_d->local_count += op;
			trcount += op;
		}
			for(int i=0;i<vcn.size();i++)
		{
			int u = vcn[i];
			(node_table->ID_to_pos(u))->vision_count += op;
			pos_s->vision_count += op;
			pos_d->vision_count += op;
			illusion += op;
		}
		v1.clear();
		vector<unsigned int>().swap(v1);
		v2.clear();
		vector<unsigned int>().swap(v2);
		vi1.clear();
		vector<unsigned int>().swap(vi1);
		vi2.clear();
		vector<unsigned int>().swap(vi2);
		cn.clear();
		vector<unsigned int>().swap(cn);
		vcn.clear();
		vector<unsigned int>().swap(vcn);
		return;
	}
	
	void modify_both(sample_node* pos_s, sample_node* pos_d, long long last_mark, long long land_mark, int op, int type)
	{
		if(type==type_A)
			modify_both_A(pos_s, pos_d, last_mark, land_mark, op);
		else
			modify_both_B(pos_s, pos_d, last_mark, land_mark, op);
	}
	void link_list(sample_node* pos_s, sample_node* pos_d, int pos, unsigned int s_num, unsigned int d_num)
	{

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
		unsigned int s = pos_s->nodeID;
		unsigned int d = pos_d->nodeID;
		
		int last_edge_s = edge_table->table[pos].pointers[last_s];  // isolate this edge from the list
		int last_edge_d = edge_table->table[pos].pointers[last_d];
		int next_edge_s = edge_table->table[pos].pointers[next_s];
		int next_edge_d = edge_table->table[pos].pointers[next_d];

		if (pos_s->out_edge == pos)
		{
			if (last_edge_s < 0 && pos_s->in_edge < 0) // there are no edges left for this node
			{
				pos_s = NULL;
				node_table->delete_via_ID(s);
				node_count--;
			}
			else
				pos_s->out_edge = last_edge_s;
		}
		if(!pos_d||pos_d->nodeID!=d)
			pos_d = node_table->ID_to_pos(d);
		
		if (pos_d->in_edge == pos)
		{
			if (last_edge_d < 0 && pos_d->out_edge < 0)
			{
				pos_d = NULL;
				node_table->delete_via_ID(d);
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

	void dismiss(unsigned int s, unsigned int d, int pos)
	{	
		int last_edge_s = edge_table->table[pos].pointers[last_s];  // isolate this edge from the list
		int last_edge_d = edge_table->table[pos].pointers[last_d];
		int next_edge_s = edge_table->table[pos].pointers[next_s];
		int next_edge_d = edge_table->table[pos].pointers[next_d];
		
		sample_node* pos_s = node_table->ID_to_pos(s); 
		if (pos_s->out_edge == pos)
		{
			if (last_edge_s < 0 && pos_s->in_edge < 0) // there are no edges left for this node
			{
				pos_s = NULL;
				node_table->delete_via_ID(s);
				node_count--;
			}
			else
				pos_s->out_edge = last_edge_s;
		}
			
		sample_node* pos_d = node_table->ID_to_pos(d);
		
		if (pos_d->in_edge == pos)
		{
			//cout<<"delete destinition node"<<endl;
			if (last_edge_d < 0 && pos_d->out_edge < 0)
			{
				pos_d = NULL;
				node_table->delete_via_ID(d);
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
	 void insert(unsigned int s_num, unsigned int d_num, double p, long long time, long long land_mark,long long last_mark, int hashindex)
	 {
		 string s = my_to_string(s_num);
		 string d = my_to_string(d_num);
		 string edge = s + d;
	 	 unsigned int pos = (*hfunc[hashindex+1])((unsigned char*)(edge.c_str()), edge.length())%size;
	
	
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
					 modify_both(pos_s, pos_d, last_mark, land_mark, 1, triangle_type);
					 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
					 edge_table->table[pos].vice.reset();
				 }
				 else
				 {
					 edge_table->replace_sample(s_num, d_num, p, time, pos);
					 link_list(pos_s, pos_d, pos, s_num, d_num);
					 modify_illusion(pos_s, pos_d, land_mark, 1, triangle_type);
				 }

			 }
			 else{
				 edge_count++;
				 valid_num++;
				 edge_table->replace_sample(s_num, d_num, p, time, pos);	// if there is no sampled edge in this substream, this is the first one
				 link_list(pos_s, pos_d, pos, s_num, d_num);
				 modify_both(pos_s, pos_d, last_mark, land_mark, 1, triangle_type);

				 q_count = q_count - 1 + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
			 }
			return;
		}

		 // if the inserted edge has already shown up and is sampled.

		 if (edge_table->table[pos].s == s_num && edge_table->table[pos].d == d_num)
		 {
		 	 if(edge_table->table[pos].timestamp<land_mark){
			 illusion_q = illusion_q- 1 + 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1);
			 illusion_valid++;
			 sample_node* pos_s = node_table->ID_to_pos(s_num);
			 sample_node* pos_d = node_table->ID_to_pos(d_num);
			 modify_illusion(pos_s, pos_d, land_mark, 1, triangle_type);
			}
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
				// in this case, the vice edge is not needed anyway;
				edge_table->delete_vice(pos);
				// reset the vice edge

				// replace the sample edge
				sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				q_count += -1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				illusion_q += -1 + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
				illusion_valid++;
				modify_triangle(old_s, old_d, last_mark, -1, triangle_type);
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
				}					// if the node is not in the table ,insert it
				link_list(pos_s, pos_d, pos, s_num, d_num);
				modify_both(pos_s, pos_d, last_mark, land_mark, 1, triangle_type);

			}
			else   // if smaller than the sampled p, check the vice edge;
			{
				if (p >= edge_table->table[pos].vice.priority) // can replace it;
				{
					edge_table->replace_vice(s_num, d_num, p, time, pos);
				}
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
						 if (p >= edge_table->table[pos].vice.priority) // the new edge can replace the test edge
						 {
							 q_count = q_count - 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);
							 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);

							 edge_table->delete_vice(pos); // the test edge is not needed any more;
							 modify_illusion(old_s, old_d, land_mark, -1, triangle_type); // in this case the sampled edge is only an illusion

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
							 }					// if the node is not in the table ,insert it

							 link_list(pos_s, pos_d, pos, s_num, d_num);
							 modify_both(pos_s, pos_d, last_mark, land_mark, 1, triangle_type);
							 valid_num++;
						 }
						 else
						 {
							 illusion_q = illusion_q - 1/pow(2, int(-(log(1 - edge_table->table[pos].priority) / log(2)))+1) + 1/pow(2, int(-(log(1 - p) / log(2)))+1);

							 modify_illusion(old_s, old_d, land_mark, -1, triangle_type); // in this case the sampled edge is only an illusion
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
							 }					// if the node is not in the table ,insert it
							 link_list(pos_s, pos_d, pos,s_num, d_num);
							 modify_illusion(pos_s, pos_d, land_mark, 1, triangle_type); // the new edge is still an illusion


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

						 modify_both(old_s, old_d, last_mark, land_mark, -1, triangle_type);
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
						 }					// if the node is not in the table ,insert it
						 link_list(pos_s, pos_d, pos, s_num, d_num);
						 modify_both(pos_s, pos_d, last_mark, land_mark, 1, triangle_type);
					 }
					 // reset the vice edge

			
				}

			}

		 return;
	 }
	 void update(long long time, long long land_mark, long long last_mark)  // when the sampled edge expires, delete it and move the candidate edge one rank upper. Before this function the cross lists including this pos should be changed, and after this function the new sampled edge (valid or not) should be 
		 // added into the curresponding cross lists;
	 {
		int tsl_pos = edge_table->tsl_head;
		if(tsl_pos < 0)
			return;
		int pos = tsl_pos % size;
		 
		 
		 while (edge_table->table[pos].timestamp < time)
		 {
			  tsl_pos = edge_table->table[pos].time_list_next;

			 if (edge_table->table[pos].vice.timestamp < last_mark && edge_table->table[pos].vice.timestamp >= 0)  // if the vice edge is elder than the last_mark, then it is a left test edge and need to be cleaned. 
				 edge_table->table[pos].vice.reset();
			
			 if(edge_table->table[pos].vice.timestamp>=time)
			 {
				 illusion_q = illusion_q - 1 + 1/pow(2, int(-(log(1 - edge_table->table[pos].vice.priority) / log(2)))+1);

				 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 modify_triangle(old_s, old_d, last_mark, -1, triangle_type);
				 dismiss(old_s, old_d, pos);
				 sample_unit tmp = edge_table->table[pos];
		
				 edge_table->delete_sample(pos); // delete the expired sample;


				 valid_num--;  // the valid num decreases, but the illusion_valid do not decrease, as after the land mark this bucket will still be valid;


				 edge_table->table[pos].reset(tmp.vice.s, tmp.vice.d, tmp.vice.priority, tmp.vice.timestamp, tmp.vice.time_list_prev, tmp.vice.time_list_next); // the vice edge is an invalid sample now
			     edge_table->set_time_list(tmp.vice.time_list_prev, 1, pos);
				 edge_table->set_time_list(tmp.vice.time_list_next, 0, pos); // the pointer used to be pos + total_size (candidate unit), now updated to pos (sample unit); 
				 if(edge_table->tsl_tail == pos + size)
					edge_table->tsl_tail = pos;
				 if(edge_table->tsl_head == pos + size)
					edge_table->tsl_head = pos; 
			    
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
				 modify_illusion(pos_s, pos_d, land_mark, 1, triangle_type); // the inserted is invalid, therefore only the illusion is increased;
				 illusion_valid++;

				 
				 edge_table->table[pos].vice.reset(tmp.s, tmp.d, tmp.priority, tmp.timestamp);
			 }
			 else  // if there is no vice edge
			 {
				 sample_node* old_s = node_table->ID_to_pos(edge_table->table[pos].s);
				 sample_node* old_d = node_table->ID_to_pos(edge_table->table[pos].d);
				 modify_triangle(old_s, old_d, last_mark, -1, triangle_type);
				 dismiss(old_s, old_d, pos);
				 valid_num--;
					
				 edge_table->table[pos].vice.reset(edge_table->table[pos].s, edge_table->table[pos].d, edge_table->table[pos].priority, edge_table->table[pos].timestamp);


				 edge_table->delete_sample(pos);

			 }

			 if(tsl_pos<0)
				break;
			pos = tsl_pos % size;

		 }
	 }

	 void ective()
	 {
		 trcount = illusion;
		 valid_num = illusion_valid;
		 q_count = illusion_q;
		 edge_count = illusion_valid; 
		 illusion_q = size;
		 illusion_valid = 0;
		 illusion = 0;
		 node_table->active();
	 }
	 
 };
 
 
