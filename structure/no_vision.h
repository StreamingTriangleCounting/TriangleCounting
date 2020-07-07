#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<assert.h>
#include "simple_sample.h"

using namespace std;

class  nv_sample
{
public:
	Simple_Sample* ss;
	int window_size;
	long current_time;
	long land_mark;
	long last_mark;
	int edge_estimate;

	nv_sample(int size, int w)
	{
		ss = new Simple_Sample(size);
		window_size = w;
		current_time = 0;
		land_mark = 0;
		last_mark = 0;
	
	}
	~nv_sample()
	{
		delete ss;
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
		double p = double(MurmurHash((const unsigned char*)e.c_str(), e.length()) % 1000000+1) / 1000001;
		current_time = time;
		ss->update(time-window_size);
		if (time - land_mark >= window_size)
		{
			assert(time - land_mark < 2*window_size);
			last_mark = land_mark;
			land_mark = land_mark + window_size;
			ss->slice_switch(last_mark);
			cout << time << ' '<<land_mark<<" effected" << endl;
		}
		ss->insert(s, d, p, time, land_mark, last_mark);
	}
	int count()
	{
		int m = ss->size;
		double alpha = 0.7213 / (1 + (1.079 / m));
		int total_num = (double(alpha * m * m) / (ss->q_count));
		int sample_num = ss->valid_num;
		if (total_num < 2.5*m)
			total_num = -log(1 - double(ss->edge_count) / m)*m;
		total_num = total_num *(double(ss->valid_num) / ss->edge_count);
		edge_estimate = total_num;
		//cout << q << ' ' << total_num << endl;
		//cout <<"st count "<< sample_num << ' ' << total_num << ' '<<st->edge_count<<' '<<st->q_count<<endl;
		double p = (double(sample_num) / total_num) * (double(sample_num-1) / (total_num-1)) * (double(sample_num-2) / (total_num-2));
		cout <<"nc: "<<ss->trcount<<' '<< p << endl;
		return (ss->trcount) / p;
	}
};
