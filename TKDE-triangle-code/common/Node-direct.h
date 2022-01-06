#include<iostream>
#include<sstream>
#include<string> 
#define next_s 0
#define next_d 1
#define last_s 2
#define last_d 3

#define out 0
#define in 1
using namespace std;

string my_to_string(unsigned int a)
{
   	string str="";
	while(a>0)
	{
		str += (char)('0'+a%10);
		a=a/10;
	}
	string astr;
	astr.resize(str.length());
	for(int i=str.length()-1;i>=0;i--)
		astr[str.length()-1-i] = str[i]; 
     return astr ;
}
class sample_node
{
	public:
	unsigned int  nodeID;
	unsigned int  local_count;
	unsigned int  vision_count;
	
	int out_edge;
	int in_edge;
	
	sample_node* next;
	
	sample_node()
	{
		nodeID = 0;
		local_count = 0;
		vision_count = 0;
		next = NULL;
		out_edge = -1;
		in_edge = -1;
	} 
	sample_node(unsigned int s, int out_edge_ = -1, int in_edge_= -1)
	{
		nodeID = s;
		next = NULL;
		out_edge = out_edge_;
		in_edge = in_edge_;
		local_count = 0;
		vision_count = 0;
	 } 
	 void init(unsigned int s, int out_edge_ = -1, int in_edge_= -1)
	 {
	 	nodeID = s;
		next = NULL;
		out_edge = out_edge_;
		in_edge = in_edge_;
		local_count = 0;
		vision_count = 0;
	 }
	 void set_edge(int s, int type)
	 {
	 	if(type==out)
	 		out_edge = s;
	 	else
	 		in_edge = s;
	 }
	 int get_edge(int type)
	 {
	 	if(type==out)
	 		return out_edge;
	 	else
	 		return in_edge;
	 }
	 void reset()
	 {
	 	nodeID = 0;
	 	out_edge = -1;
		in_edge = -1;
		local_count = 0;
		vision_count = 0;
	 }
};
 
  class candidate_unit
 {
 public:
	 unsigned int  s, d;
	 double priority;
	 long long timestamp;
	 int time_list_prev; // suppose the size of the sample table is m. A pointer in value range 0 ~ m-1 means sample unit in the corresponding table pos. 
	 //A pointer in value range m ~ 2m-1 means candidate unit in the corresponding table pos. -1 means an empty pointer.
	 int time_list_next;
	 candidate_unit(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = -1)
	 {
		 s = snum;
		 d = dnum;
		 priority = p;
		 timestamp = time;
		 time_list_prev = prev;
		 time_list_next = next;
	 }
	 void reset(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = -1)
	 {
		 s = snum;
		 d = dnum;
		 priority = p;
		 timestamp = time;
		 time_list_prev = prev;
		 time_list_next = next;
	 }
 };
 class sample_unit
{
	public:
	unsigned int s, d;
	double priority;
	long long timestamp;    
	int pointers[4];
	int time_list_prev;
	int time_list_next;
	candidate_unit vice;
	sample_unit(unsigned int snum = 0, unsigned int dnum = 0,double p = -1,  long long time = -1, int prev = -1, int next = -1)
	{
		s = snum;
		d = dnum;
		for(int i=0;i<4;i++)
			pointers[i] = -1;
		priority = p;
		timestamp = time;
		time_list_prev = prev;
		time_list_next = next;
		vice.reset();
	}
	void set_next_s(int s){pointers[next_s] = s;}
	void set_next_d(int d){pointers[next_d] = d;}
	void set_last_s(int s){pointers[last_s] = s;}
	void set_last_d(int d){pointers[last_d] = d;}
	void reset(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = -1)
	{
		s = snum;
		d = dnum;
		for(int i=0;i<4;i++)
			pointers[i] = -1;
		priority = p;
		timestamp = time;
		time_list_prev = prev;
		time_list_next = next;
	}	
}; 


