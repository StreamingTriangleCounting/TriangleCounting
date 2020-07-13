#include<iostream>
#include<sstream>
#include<string> 
#define next_s 0
#define next_d 1
#define last_s 2
#define last_d 3
using namespace std;
class sample_node
{
	public:
	unsigned long nodeID;
	
	int first_edge;
	
	sample_node* next;
	
	sample_node()
	{
		nodeID = 0;
		next = NULL;
		first_edge = -1;
	} 
	sample_node(unsigned long s, int edge = -1)
	{
		nodeID = s;
		next = NULL;
		first_edge = edge;
	 } 
	 void init(unsigned long s, int edge = -1)
	 {
	 	nodeID = s;
		next = NULL;
		first_edge = edge;
	 }
	 void set_first_edge(int s)
	 {
	 	first_edge = s;
	 }
	 void reset()
	 {
	 	nodeID = 0;
	 	first_edge = -1;
	 }
};
 struct list_unit
 {
 	int pos;
 	list_unit* next;
 	list_unit* prev;
 };

 class candidate_unit
 {
 public:
	 unsigned long long s, d;
	 double priority;
	 long timestamp;
	 list_unit* window_pos;
	 candidate_unit(unsigned long snum = 0, unsigned long dnum = 0, double p = -1, long time = -1)
	 {
		 s = snum;
		 d = dnum;
		 priority = p;
		 timestamp = time;
		 window_pos = NULL;
	 }
	 void reset(unsigned long snum = 0, unsigned long dnum = 0, double p = -1, long time = -1)
	 {
		 s = snum;
		 d = dnum;
		 priority = p;
		 timestamp = time;
		 window_pos = NULL;
	 }
 };
class sample_unit
{
	public:
	unsigned long s, d;
	double priority;
	long timestamp;    
	int pointers[4];
	list_unit* window_pos; 
	candidate_unit vice;
	sample_unit(unsigned long snum = 0, unsigned long dnum = 0)
	{
		s = snum;
		d = dnum;
		for(int i=0;i<4;i++)
			pointers[i] = -1;
		priority = -1;
		timestamp = -1;
		window_pos = NULL;
		vice.reset();
	}
	void set_next_s(int s){pointers[next_s] = s;}
	void set_next_d(int d){pointers[next_d] = d;}
	void set_last_s(int s){pointers[last_s] = s;}
	void set_last_d(int d){pointers[last_d] = d;}
	void reset(unsigned long snum = 0, unsigned long dnum = 0, double p = -1, long time = -1)
	{
		s = snum;
		d = dnum;
		for(int i=0;i<4;i++)
			pointers[i] = -1;
		priority = p;
		timestamp = time;
		window_pos = NULL;
	}	
}; 
 

class test_unit
{
public:
	unsigned long long s, d;
	double priority;
	long timestamp;
	list_unit* window_pos;
	test_unit(double p = -1, long time = -1, unsigned long long s_= 0, unsigned long long d_ = 0)
	{
		s = s_;
		d = d_;
		priority = p;
		timestamp = time;
		window_pos = NULL;
	}
	void reset(double p = -1, long time = -1, unsigned long long s_= 0, unsigned long long d_ = 0)
	{
		s = s_;
		d = d_;
		priority = p;
		timestamp = time;
		window_pos = NULL;
	}
};

class BPS_unit
{
public:
	unsigned long s, d;
	double priority;
	long timestamp;
	int pointers[4];
	list_unit* window_pos;
	test_unit test;
	BPS_unit(unsigned long snum = 0, unsigned long dnum = 0)
	{
		s = snum;
		d = dnum;
		for (int i = 0; i<4; i++)
			pointers[i] = -1;
		priority = -1;
		timestamp = -1;
		window_pos = NULL;
		test.reset();
	}
	void set_next_s(int s){ pointers[next_s] = s; }
	void set_next_d(int d){ pointers[next_d] = d; }
	void set_last_s(int s){ pointers[last_s] = s; }
	void set_last_d(int d){ pointers[last_d] = d; }
	void reset(unsigned long snum = 0, unsigned long dnum = 0, double p = -1, long time = -1)
	{
		s = snum;
		d = dnum;
		for (int i = 0; i<4; i++)
			pointers[i] = -1;
		priority = p;
		timestamp = time;
		window_pos = NULL;
	}
};
