#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<assert.h>
#include "sampletable.h"

using namespace std;

class sample
{
public:
	SampleTable* st;
	int window_size;
	long current_time;
	long land_mark;
	long last_mark;
	int edge_estimate;

	sample(int size, int w)
	{
		st = new SampleTable(size);
		window_size = w;
		current_time = 0;
		land_mark = 0;
		last_mark = 0;
	
	}
	~sample()
	{
		delete st;
	}

	void proceed(unsigned long s, unsigned long d, long time)
	{
		if (s < d)
		{
			unsigned long tmp = s;
			s = d;
			d = tmp;
		}
		string s_string = to_string(s);
		string d_string = to_string(d);
		string e = s_string + d_string;
		//double p = double(MurmurHash((const unsigned char*)e.c_str(), e.length()) % 1000000+1) / 1000001;
		double p = (rand()+1)/double(RAND_MAX+2); // the priority is randomly generated.
	//	cout<<p<<endl;
		current_time = time;
		st->update(time-window_size, last_mark );
		if (time - land_mark >= window_size)
		{
			assert(time - land_mark < 2*window_size);
			last_mark = land_mark;
			land_mark = land_mark + window_size;
			st->ective();
		//	cout << time << ' '<<land_mark<<" effected" << endl;
		}
		st->insert(s, d, p, time, land_mark, last_mark);
	}
	unsigned long long count()
	{
		int m = st->size;
		double alpha = 0.7213 / (1 + (1.079 / m));
		double q = 0;
		for (int i = 0; i < m; i++)
		{
			if (st->edge_table->table[i].vice.timestamp>last_mark && st->edge_table->table[i].vice.timestamp <= land_mark)
				q = q + 1 / pow(2, int(-(log(1 - st->edge_table->table[i].vice.priority) / log(2))) + 1);
			else if (st->edge_table->table[i].priority <= 0)
				q = q + 1;
			else
			q = q + 1 / pow(2, int(-(log(1 - st->edge_table->table[i].priority) / log(2))) + 1);
			if(i%1000==0)
				cout<<"current q "<<q<<endl;
		}
		int total_num = (double(alpha * m * m) / (st->q_count));
		int sample_num = st->valid_num;
		if (total_num < 2.5*m)
			total_num = -log(1 - double(st->edge_count) / m)*m;
		total_num = total_num *(double(st->valid_num) / st->edge_count);
		edge_estimate = total_num;
	//	cout << q << ' ' << total_num << endl;
		cout <<"st count "<< sample_num << ' ' << total_num << ' '<<st->edge_count<<' '<<st->q_count<<endl;
		double p = (double(sample_num) / total_num) * (double(sample_num-1) / (total_num-1)) * (double(sample_num-2) / (total_num-2));
	//	cout <<"tc: "<<st->trcount<<' '<< p << endl;
		return (st->trcount) / p;

	}
	int count_tr(unsigned long s, unsigned long d)
	{

		if (d>s)
			cout << "error" << endl;
		sample_node* tmp1 = st->node_table->ID_to_pos(s);
		sample_node* tmp2 = st->node_table->ID_to_pos(d);
		int loc1 = tmp1->first_edge;
		int loc2 = tmp2->first_edge;
		vector<int> v1;
		vector<int> v2;
		while (loc1 > 0)
		{
			if (st->edge_table->table[loc1].s == s)
			{
				if (st->edge_table->table[loc1].d < s)
					v1.push_back(st->edge_table->table[loc1].d);
				loc1 = st->edge_table->table[loc1].pointers[last_s];
			}
			else if (st->edge_table->table[loc1].d == s)
			{
				if (st->edge_table->table[loc1].s < s)
					v1.push_back(st->edge_table->table[loc1].s);
				loc1 = st->edge_table->table[loc1].pointers[last_d];
			}
			else
				cout << "error case 2" << endl;

		}
		while (loc2 > 0)
		{
			if (st->edge_table->table[loc2].s == d)
			{
				if (st->edge_table->table[loc2].d < d)
					v2.push_back(st->edge_table->table[loc2].d);
				loc2 = st->edge_table->table[loc2].pointers[last_s];
			}
			else if (st->edge_table->table[loc2].d == d)
			{
				if (st->edge_table->table[loc2].s < d)
					v2.push_back(st->edge_table->table[loc2].s);
				loc2 = st->edge_table->table[loc2].pointers[last_d];
			}
			else
				cout << "error case 2" << endl;

		}

		return count_join(v1, v2);
	}

	int total_count()
	{
		int tr_count = 0;
		map<unsigned long, vector<unsigned long> > list;
	//	cout << "counting begin" << endl;
		for (int i = 0; i < st->node_table->length; i++)
		{
			if (st->node_table->table[i].nodeID > 0){
				unsigned long s = st->node_table->table[i].nodeID;
			//	cout << s << endl;
				int tmp = st->node_table->table[i].first_edge;
				while (tmp >=0)
				{
				//	if (s == 59)
					//	cout << "visiting " << tmp << endl;
					if (st->edge_table->table[tmp].s == s)
					{
						if (st->edge_table->table[tmp].d < s)
						{
							if (st->edge_table->table[tmp].vice.timestamp>st->edge_table->table[tmp].timestamp || st->edge_table->table[tmp].vice.timestamp < last_mark)
								list[s].push_back(st->edge_table->table[tmp].d);
						}
						tmp = st->edge_table->table[tmp].pointers[last_s];
					}
					else if (st->edge_table->table[tmp].d == s)
					{
						if (st->edge_table->table[tmp].s < s)
						{
							if (st->edge_table->table[tmp].vice.timestamp>st->edge_table->table[tmp].timestamp || st->edge_table->table[tmp].vice.timestamp < last_mark)
								list[s].push_back(st->edge_table->table[tmp].s);
						}
						tmp = st->edge_table->table[tmp].pointers[last_d];
					}
					else
						cout << "error case 3" << endl;
				}
				sample_node* next = st->node_table->table[i].next;
				while (next)
				{
					tmp = next->first_edge;
					s = next->nodeID;
				//	cout << s <<' '<<tmp<< endl;
					while (tmp >=0)
					{
						//if (s == 59)
							//cout << "visiting "<<tmp << endl;
						if (st->edge_table->table[tmp].s == s)
						{
							if (st->edge_table->table[tmp].d < s)
							{
								if (st->edge_table->table[tmp].vice.timestamp>st->edge_table->table[tmp].timestamp || st->edge_table->table[tmp].vice.timestamp < last_mark)
									list[s].push_back(st->edge_table->table[tmp].d);
							}
							tmp = st->edge_table->table[tmp].pointers[last_s];
						}
						else if (st->edge_table->table[tmp].d == s)
						{
							if (st->edge_table->table[tmp].s < s)
							{
								if (st->edge_table->table[tmp].vice.timestamp>st->edge_table->table[tmp].timestamp || st->edge_table->table[tmp].vice.timestamp < last_mark)
									list[s].push_back(st->edge_table->table[tmp].s);
							}
							tmp = st->edge_table->table[tmp].pointers[last_d];
						}
						else
							cout << "error case 3" << endl;
					}
					next = next->next;
				}
			}
			//cout << endl;
		}

		map<unsigned long, vector<unsigned long> > ::iterator it;
		for (it = list.begin(); it != list.end(); it++)
		{
			unsigned long s = it->first;
		//	cout << s << " the neighbors are: ";
			for (int i = 0; i < it->second.size(); i++)
			{
			//	cout << it->second[i] << " ";
				vector<unsigned long> v1 = it->second;
				vector<unsigned long> v2 = list[it->second[i]];
				tr_count += count_join(v1, v2);
			}
		//	cout << endl;
		}
		return tr_count;

	}
};

