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
		int total_num = (double(alpha * m * m) / (st->q_count));
		int sample_num = st->valid_num;
		if (total_num < 2.5*m)
			total_num = -log(1 - double(st->edge_count) / m)*m;
		total_num = total_num *(double(st->valid_num) / st->edge_count);
		edge_estimate = total_num;
		cout <<"st count "<< sample_num << ' ' << total_num << ' '<<st->edge_count<<' '<<st->q_count<<endl;
		double p = (double(sample_num) / total_num) * (double(sample_num-1) / (total_num-1)) * (double(sample_num-2) / (total_num-2));
		return (st->trcount) / p;

	}
};

