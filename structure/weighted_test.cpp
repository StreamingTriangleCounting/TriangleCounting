#include<iostream>
#include<fstream>
#include<set>
#include "Graph.h"
#include<string>
#include "hashTable.h" 
using namespace std;

struct weighted_edge
{
	string s;
	string d;
	int frequency;
	double p;
	weighted_edge(string ts="", string td="", double tp=0, int f = 1)
	{
		s = ts;
		d = td;
		p = tp;
		frequency = f;
	}
};
class weighted_sampler
{
	public:
		weighted_edge* array;
		int size;
		weighted_sampler(int s)
		{
			size = s;
			array = new weighted_edge[s];
			for(int i=0;i<size;i++)
				array[i].p=0;
		}
		void process_1(string s, string d)
		{
			int pos = rand()%size;
			double p = double(rand()+1)/(RAND_MAX+1);
			if(p>array[pos].p)
				array[pos] = weighted_edge(s, d, p);
		} 
		
		void process_2(string s, string d)
		{

			string e = s + d;
			double p = double(MurmurHash((const unsigned char*)e.c_str(), e.length()) % 1000000+1) / 1000001;
			int pos = (*hfunc[0])((unsigned char*)(e.c_str()), e.length())%size;
			if(array[pos].s == s &&array[pos].d==d)
				array[pos].frequency++;
			else if(p>array[pos].p)
				array[pos] = weighted_edge(s, d, p);
			else
				return;
		}
	

};


int main()
{
//	ifstream fin("contact/out.contact");
	ifstream fin("out.lkml-reply");
	string s, d;
	string r1, r2;
	int size = 4000;
	Graph* g = new Graph();
	Graph* sg = new Graph();
	Graph* tg = new Graph();
	weighted_sampler * sp = new weighted_sampler(size);
	weighted_sampler * tp = new weighted_sampler(size);
	int count = 0;
	map<string, int> F;
	while(fin>>s>>d>>r1>>r2)
	{	
		if(s==d)
			continue;
	    string tmp;
		if(s<d)
		{
			tmp = s;
			s = d;
			d = tmp;
		}
		string e = s+ "c" + d;
		map<string, int>::iterator it = F.find(e);	
		if(it==F.end())
			F[e] = 1;
		else
			F[e]++;
	//	cout<<s<<' '<<d<<endl;
		count++;
		if(count>100000)
			break; 
		g->insert_edge(s, d);
		sp->process_1(s, d);
		tp->process_2(s, d);	
	}
	cout<<g->get_edgenum()<<endl;
	cout<<count<<endl;
	int en1=0;
	int en2 = 0;
	for(int i=0;i<size;i++)
	{
		if(sp->array[i].p>0)
		{
			sg->insert_edge(sp->array[i].s, sp->array[i].d);
			en1++;
		}
		if(tp->array[i].p>0){
			en2++;
	//		string e = tp->array[i].s+'c' + tp->array[i].d;
	//		if(F[e]!=tp->array[i].frequency)
	//			cout<<"wrong"<<" "<<tp->array[i].s<<' '<<tp->array[i].d<<' '<<F[e]<<' '<<tp->array[i].frequency<<endl;
		for(int j=0;j<tp->array[i].frequency;j++)
			tg->insert_edge(tp->array[i].s, tp->array[i].d);
//		if(g->get_frequency(tp->array[i].s, tp->array[i].d)!=tp->array[i].frequency)
	//		cout<<"wrong"<<" "<<tp->array[i].s<<' '<<tp->array[i].d<<' '<<tp->array[i].frequency<<endl;
		}
	}
//	g->print();
//	tg->print();
	unsigned long long num1 = sg->count_triangle();
	unsigned long long num2 = tg->count_triangle();
	int distinct = g->get_edgenum(); 
	cout<<num1<<' '<<num2<<endl;
//	cout<<tg->get_edgenum()<<endl;
	cout<<en1<<' '<<en2<<endl;
	double p1 = (double(en1)/count) * (double(en1-1)/(count-1)) * (double(en1-2)/(count-2));
	double p2 = (double(en2)/distinct) * (double(en2-1)/(distinct-1)) * (double(en2-2)/(distinct-2));
	cout<<double(num1)/p1<<' '<<double(num2)/p2<<endl;
	cout<<g->count_triangle()<<endl;
}
