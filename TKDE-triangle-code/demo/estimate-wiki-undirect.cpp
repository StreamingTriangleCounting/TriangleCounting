#include<iostream>
#include<fstream>
#include<vector>
#include<time.h>
#define triangle_type 1	// triangle type is defined for directed counting, 0 means type A, 1 means type B. Details of the types can be found in our paper.
#include "../SWTC-structure/undirected/sample.h"	// undirected, binary counting, include different files for different semantics.
#include "../SWTC-structure/undirected/asy_sample.h"
#include "../BPS-structure/undirected/BPS-sample.h"
using namespace std;

int main()
{
	 	unsigned int s, d, w, w_;
        long long t;
        string t1, t2, p;
        double time_unit = 18.545;	// the unit of the window length, we use average time span as unit, namely (maximum timestamp- minimum timestamp)/number_of_edges .
        int gap = 2000000; // the length of the sliding window
        for(int hindex =0;hindex<5; hindex++)	// use different hash functions to carry out multiple groups of 
        {
        	for(int sample_size = 40000;sample_size<=120000;sample_size+=20000)
        	{
        	long wsize = gap*time_unit;
            long count = 0;
            ifstream fin("../../data/WikiTalk.txt");	// 
            string index = "result/undirect/Wiki/EstimateResult_wiki_undirect_2M_h";
            index += char(hindex+'0');
            index += '_';
            index += my_to_string(sample_size/20000);
            ofstream fout(index.c_str());
            asy_sample* ac = new asy_sample(sample_size, wsize,10, hindex);
            sample* sc = new sample(sample_size, wsize, hindex);
            BPSsample* bc = new BPSsample(sample_size, wsize, hindex);
            long long t0 = -1;
            long long tmp_point = 0;
            int checkpoint = wsize/10;
            int num = 0;
            while(fin>>s>>d>>t)
            {
                if(t0<0) t0 = t;
                s = s + 1;
                d = d + 1;
                if (s != d)     count = t - t0;	// use the 
                else continue;
                num++;

            	ac->proceed(s, d, count);
                bc->proceed(s, d, count);
                sc->proceed(s, d, count);

                if (count>=2*wsize && int(count/checkpoint) > tmp_point)	// whenever the window 
                {
                    srand((int)time(0));
                    tmp_point = count/checkpoint;
                    ac->prepare();
                    bc->prepare();
                    sc->prepare();
                    fout<<"asyn triangle "<< ac->valid_count() <<' '<<ac->edge_estimate<<' '<<ac->ss->trcount<<' '<<ac->count()<<endl;
                    fout<<"BPS triangle "<<bc->st->valid_num<<' '<<bc->edge_estimate<<' '<<bc->st->trcount<<' '<<bc->count()<<endl;
                    fout<<"swtc triangle "<<sc->st->valid_num<<' '<<sc->edge_estimate<<' '<<sc->st->trcount<<' '<<sc->count()<<endl;
                    fout<<endl;
                    cout<<sample_size<<" check point " << tmp_point << endl;
                                //cout<<t<<' '<<count<<' '<<num<<endl;
                }
            }
            fin.close();
            fout.close();

        	delete ac;
            delete sc;
            delete bc;
        	}
        }
        return 0;
}
                                                                      
