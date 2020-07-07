#include<iostream>
#include"../structure/sample.h"
#include"../structure/no_vision.h"
#include<string>
#include<fstream>
#include<time.h>

bool print = false;
using namespace std;

int main()
{
        unsigned long* count = new unsigned long[500000000];
        unsigned long* s = new unsigned long[500000000];
        unsigned long* d = new unsigned long[500000000];
        double time_unit = 1;
        int gap = 10000000;
        int sample_size = gap*0.04;
        int wsize = gap * time_unit;
        int aim = 100000000 * time_unit;
        int bench = gap*0.01;

        ofstream fout("speed_wiki_nv_vs_v_.txt");
        clock_t start, finish;
        ifstream fin("../../../data/dbpedia-link/out.dbpedia-link");
      //  ifstream fin("out.lkml-reply");
        sample* tc = new sample(sample_size, wsize);
        nv_sample* nc = new nv_sample(sample_size, wsize);
        int t0 = 0;
        bool set = false;
        int num = 0;
        int t;

        while(fin>>t>>s[num]>>d[num])
        {
                s[num] = s[num] + 1;
                d[num] = d[num] + 1;
                if (s[num] != d[num])
                {
                	count[num]=t0;
                	t0=t0+1;
				}
                else continue;
                if (s[num] < d[num])
                {
                        unsigned long tmp = s[num];
                        s[num] = d[num];
                        d[num] = tmp;
                }
                if(count[num] > aim) break;
               if(count[num] >= 2*wsize && !set){ //start = clock();
                        set = true;}
                if(set) 
					num++;
                if(!set){tc->proceed(s[num], d[num], count[num]);
                        nc->proceed(s[num], d[num], count[num]);}
        }
        cout<<"read finised"<<endl;
        fin.close();
    	for(int i=0;i<200;i++)
    	{
    	
    	start = clock();
        for(int j=0;j<bench;j++)
        {
        	int pos = i*bench+j; 
			tc->proceed(s[pos], d[pos], count[pos]);
	}
        finish = clock();
	fout<<i<<endl;
        fout<<"tc: "<<double(bench)/(double(finish-start)/CLOCKS_PER_SEC)<<endl;
        fout<<"tc: "<<tc->count()<<endl;
    		
    	 start = clock();
         for(int j=0;j<bench;j++)
         {
		    int pos = i*bench+j;  
			nc->proceed(s[pos], d[pos], count[pos]);
		 } 
        finish = clock();
        fout<<"nc: "<<double(bench)/(double(finish-start)/CLOCKS_PER_SEC)<<endl;
    	fout<<"nc: "<<nc->count()<<endl;
    	fout<<endl;
		}


                delete tc;
                delete nc;


        return 0;
}

