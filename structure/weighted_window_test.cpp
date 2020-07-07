#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include"../Golden/Golden-triangle/Goldencounter.h"
#include"sample.h"
#include"../BPS-structure/BPS-sample.h"

int main()
{
	ifstream fin("out.lkml-reply");
	int size = 50000;
	int window = 1000000;
	BPSsample* bs = new BPSsample(size, window);
	sample* ss = new sample(size, window);
	GoldenCounter* gc = new GoldenCounter(window);
	int count = 0;
	//ofstream fout("result.txt");
	unsigned long long s, d, r1, r2; 
	while(fin>>s>>d>>r1>>r2)
	{
		if(s==d)
			continue;
		unsigned long long tmp;
		if(s<d)
		{
			tmp = s;
			s = d;
			d = tmp;
		}
		bs->proceed(s, d, count);
		//cout<<"bs finished"<<endl;
		ss->proceed(s, d, count);
		//cout<<"ss finished"<<endl;
		gc->insert_edge(to_string(s), to_string(d), count);
		count++;
		if(count%10000==0)
		cout<<count<<" edges processed"<<endl;
		//if(count>=400000&&count%20000==0)
		if(count==1000000)
		{
			cout<<bs->count()<<' '<<ss->count()<<' '<<gc->triangle_count()<<endl;
			break;
		}
	}
}
