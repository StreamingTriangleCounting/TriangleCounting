#include<iostream>
#include<string>
#include<vector>
#include<map>

class EdgeTable
{
public:
	int size;
	int tsl_head, tsl_tail, expiration; // mark the head and the tail of the list. expiration is only used in BPS-sample, namely the baseline method. As in this method there are two kinds
	// of edges, expired but not double expired edges, and not expired edges. We need a pointer to mark the begin of the not expired edges.
	sample_unit* table;

	EdgeTable(int s)
	{
		size = s;
		table = new sample_unit[s];
		tsl_head = -1;
		tsl_tail = -1;
		expiration = -1;
		for (int i = 0; i<s; i++)
		{
			table[i].reset();
			table[i].vice.reset();
		}
	}
	~EdgeTable()
	{
		delete[]table;
	}
	
	void set_time_list(int pos, int type, int aim) // type == 0 means prev pointer, and type == 1 means next pointer
 	{
 		if(pos==-1)
		 	return; 
 		if(pos<size) // set the time list pointer of sample unit at the corresponding pos 
		{
			if(type==0)
				table[pos].time_list_prev = aim;
			else
				table[pos].time_list_next = aim; 
		}
		else  // set the time list pointer of candidate unit at the corresponding pos 
		{
			if(type==0)
				table[pos%size].vice.time_list_prev = aim;
			else
				table[pos%size].vice.time_list_next = aim;
			
		}
	}	
	void replace_sample(unsigned int s_num, unsigned int d_num, double p, long long time, int pos) // replace the sample edge at pos with a new edge; 
	{
		// the cross list will be changed in the upper level, as node table is needed.
		int tsl_pos = pos;
		
		int prev = table[pos].time_list_prev;
		int next = table[pos].time_list_next;
		set_time_list(prev, 1, next);
		set_time_list(next, 0, prev);
		if(tsl_head == tsl_pos)
			tsl_head = next;
		if(expiration == tsl_pos)
			expiration = next;
		if(tsl_tail == tsl_pos)
			tsl_tail = prev;  // split this pos from the time list.

		table[pos].reset(s_num, d_num, p, time, tsl_tail, -1); // insert the new edge;
		
		set_time_list(tsl_tail, 1, tsl_pos);
		tsl_tail = tsl_pos;
		if(tsl_head == -1)
			tsl_head = tsl_pos;
		if(expiration == -1)
			expiration = tsl_pos;
		// the cross list of the new edge remain to be set by the upper level, as node table is needed.
	}
	void replace_vice(unsigned int s_num, unsigned int d_num, double p, long long time, int pos) // replace the vice edge at pos with a new edge;
	{
		int tsl_pos = pos + size;
		
		int prev = table[pos].vice.time_list_prev;
		int next = table[pos].vice.time_list_next;
		set_time_list(prev, 1, next);
		set_time_list(next, 0, prev);
		
		if(tsl_head == tsl_pos)
			tsl_head = next;
		if(expiration == tsl_pos)
			expiration = next;
		if(tsl_tail == tsl_pos)
			tsl_tail = prev; 

		table[pos].vice.reset(s_num, d_num, p, time, tsl_tail, -1); // insert the new edge;
		set_time_list(tsl_tail, 1, tsl_pos);
		tsl_tail = tsl_pos;
		if (tsl_head == -1)
			tsl_head = tsl_pos;
		if(expiration == -1)
			expiration = tsl_pos;
	}
	void delete_sample(int pos) // delete the sample edge in the pos
	{
		int tsl_pos = pos;
		
		int prev = table[pos].time_list_prev;
		int next = table[pos].time_list_next;
		set_time_list(prev, 1, next);
		set_time_list(next, 0, prev);
		if(tsl_head == tsl_pos)
			tsl_head = next;
		if(expiration == tsl_pos)
			expiration = next;
		if(tsl_tail == tsl_pos)
			tsl_tail = prev;  
			
		table[pos].reset(); // reset the bucket;
	}
	void delete_vice(int pos)
	{
		int tsl_pos = pos + size;
		
		int prev = table[pos].vice.time_list_prev;
		int next = table[pos].vice.time_list_next;
		set_time_list(prev, 1, next);
		set_time_list(next, 0, prev);
		
		if(tsl_head == tsl_pos)
			tsl_head = next;
		if(expiration == tsl_pos)
			expiration = next;
		if(tsl_tail == tsl_pos)
			tsl_tail = prev; 

		table[pos].vice.reset(); // reset the bucket;
	}

	void update_sample(int pos, long long time)
	{
		int tsl_pos = pos;
		int prev = table[pos].time_list_prev;
		int next = table[pos].time_list_next;
		
		table[pos].timestamp = time;
		
		set_time_list(prev, 1, next);
		set_time_list(next, 0, prev);
		if(tsl_head == tsl_pos)
			tsl_head = next;
		if(expiration == tsl_pos)
			expiration = next;
		if(tsl_tail == tsl_pos)
			tsl_tail = prev;
		
		table[pos].time_list_prev = tsl_tail;
		table[pos].time_list_next = -1;
		set_time_list(tsl_tail, 1, tsl_pos);
		tsl_tail = tsl_pos;
		if (tsl_head == -1)
			tsl_head = tsl_pos;
		if(expiration == -1)
			expiration = tsl_pos;
	}

	void update_vice(int pos, long long time)
	{
		int tsl_pos = pos + size;
		int prev = table[pos].vice.time_list_prev;
		int next = table[pos].vice.time_list_next;
	
		table[pos].vice.timestamp = time;
	
		set_time_list(prev, 1, next);
		set_time_list(next, 0, prev);
		if(tsl_head == tsl_pos)
			tsl_head = next;
		if(expiration == tsl_pos)
			expiration = next;
		if(tsl_tail == tsl_pos)
			tsl_tail = prev;
			
		table[pos].vice.time_list_prev = tsl_tail;
		table[pos].vice.time_list_next = -1;	
		set_time_list(tsl_tail, 1, tsl_pos);
		tsl_tail = tsl_pos;
		if (tsl_head == -1)
			tsl_head = tsl_pos;
		if(expiration == -1)
			expiration = tsl_pos;
	}
};
