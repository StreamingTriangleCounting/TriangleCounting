#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "BPS-sampletable.h"

using namespace std;

class BPSsample
{
public:
	BPSSampleTable* st;
	int window_size;
	int current_time;
	int edge_estimate;

	BPSsample(int size, int w)
	{
		st = new BPSSampleTable(size);
		window_size = w;
		current_time = 0;
	
	}
	~BPSsample()
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
		double p = double(MurmurHash((const unsigned char*)e.c_str(), e.length()) % 1000000 + 1) / 1000001;
		current_time = time;
		st->update(time-window_size, time-2*window_size);
		st->insert(s, d, p, time);
	}
	int count()
	{
		int m = st->size;
		double alpha = 0.7213 / (1 + (1.079 / m));
		int total_num = (double(alpha * m * m) /(st->q_count));
		int sample_num = st->valid_num;
		if (total_num < 2.5*m)
			total_num = -log(1 - double(st->edge_count) / m)*m;
		total_num = total_num *(double(st->valid_num) / st->edge_count);
		edge_estimate = total_num;
		cout <<"BPS valid count "<< sample_num << "total edge" << total_num << "total sample " << st->edge_count << "q count" << st->q_count << endl;
		double p = (double(sample_num) / total_num) * (double(sample_num - 1) / (total_num - 1)) * (double(sample_num - 2) / (total_num - 2));
		return (st->trcount) / p;

	}
};

