#include<iostream>
#include"Golden/GoldenCounter.h"
#include"SWTC-structure/sample.h"
#include"baseline-structure/BPS-sample.h"
#include<string>
#include<sstream>
#include<fstream>
#include<time.h>
#include<stdlib.h>
#include<sstream>

bool print = false;
using namespace std;
int main()
{

	unsigned long s, d;
	long long t;
	double time_unit=3.775;
	for (int gap = 4000000; gap <5000000; gap += 100000)
	{
		int count = 0;
		int wsize = gap*time_unit;
		int sample_size = gap*0.04;
		ifstream fin("stackoverflow.txt");
		string index = "stack_4%_";
		stringstream ss;
		ss<<(gap/100000);
		string str;
		ss>>str;
		ss.clear();
		index.append(str);
		ofstream fout(index.c_str());
		sample* tc = new sample(sample_size, wsize);
		GoldenCounter* gc = new GoldenCounter(wsize);
		BPSsample* bc = new BPSsample(sample_size, wsize);
		long t0 = -1;
		int tmp = 0;
		int checkpoint = wsize/10;
		while(fin>>s>>d>>t)
		{
			if(t0<0) t0 = t;
			s = s + 1;
			d = d + 1;
			if (s != d)	count = t-t0;
			else continue;
			if (s < d)
			{
				unsigned long tmp_e = s;
				s = d;
				d = tmp_e;
			}
			tc->proceed(s, d, count);
			bc->proceed(s, d, count);
			string str_s, str_d;
			ss<<s;
			ss>>str_s;
			ss.clear();
			ss<<d;
			ss>>str_d;
			ss.clear();
			gc->insert_edge(str_s, str_d, count);
			if (count>=2*wsize && int(count/checkpoint) > tmp)
			{
				tmp = count/checkpoint;
				if(tmp>0)
				{
				fout<<"tc triangle "<< tc->st->valid_num <<' '<<tc->count()<<endl;
				fout<<"BPS triangle "<<bc->st->valid_num<<' '<<bc->count()<<endl;
				fout<<"standard count "<<gc->edge_count()<<' '<<gc->triangle_count()<<endl;
				fout<<endl;
				}
				cout << "check point " << tmp << endl;
			}
			if(tmp>=120)
				break;
		}
		fin.close();
		fout.close();
		delete tc;
		delete gc;
		delete bc; 
		
	}
	return 0;
}
