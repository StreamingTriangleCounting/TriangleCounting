#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"../common_structure/nodetable.h"

class EdgeTable
{
public:
	int size;
	list_unit* tsl_head, *tsl_tail;
	sample_unit* table;

	EdgeTable(int s)
	{
		size = s;
		table = new sample_unit[s];
		tsl_head = NULL;
		tsl_tail = NULL;
		for (int i = 0; i<s; i++)
		{
			table[i].reset();
			table[i].vice.reset();
		}
	}
	~EdgeTable()
	{
		delete[]table;
		list_unit* cur, *next;
		cur = tsl_head;
		while (cur)
		{
			next = cur->next;
			delete cur;
			cur = next;
		}
	}

	void replace_sample(unsigned long s_num, unsigned long d_num, double p, long time, int pos) // replace the sample edge at pos with a new edge; 
	{
		// the cross list will be changed in the upper level, as node table is needed.
		list_unit* tmp = table[pos].window_pos; // delete the list unit
		if (tmp)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tsl_head == tmp)
				tsl_head = tmp->next;
			if (tsl_tail == tmp)
				tsl_tail = tmp->prev;
			delete tmp;
		}

		table[pos].reset(s_num, d_num, p, time); // insert the new edge;
		list_unit* cur = new list_unit; // insert a new list unit
		cur->pos = pos;
		cur->next = NULL;
		cur->prev = tsl_tail;
		if (tsl_tail)
			tsl_tail->next = cur;
		tsl_tail = cur;
		if (!tsl_head)
			tsl_head = cur;
		table[pos].window_pos = cur; 
		// the cross list of the new edge remain to be set by the upper level, as node table is needed.
	}
	void replace_vice(unsigned long s_num, unsigned long d_num, double p, long time, int pos) // replace the vice edge at pos with a new edge;
	{
		
		list_unit* tmp = table[pos].vice.window_pos; // delete the list unit of the vice edge
		if (tmp)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tsl_head == tmp)
				tsl_head = tmp->next;
			if (tsl_tail == tmp)
				tsl_tail = tmp->prev;
			delete tmp;
		}

		table[pos].vice.reset(s_num, d_num, p, time); // insert the new edge;
		list_unit* cur = new list_unit; // insert a new list unit
		cur->pos = pos;
		cur->next = NULL;
		cur->prev = tsl_tail;
		if (tsl_tail)
			tsl_tail->next = cur;
		tsl_tail = cur;
		if (!tsl_head)
			tsl_head = cur;
		table[pos].vice.window_pos = cur;
	}
	void delete_sample(int pos) // delete the sample edge in the pos
	{

		list_unit* tmp = table[pos].window_pos; // delete the list unit
		if (tmp)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tsl_head == tmp)
				tsl_head = tmp->next;
			if (tsl_tail == tmp)
				tsl_tail = tmp->prev;
			delete tmp;
		}

		table[pos].reset(); // reset the bucket;
	}
	void delete_vice(int pos)
	{
		list_unit* tmp = table[pos].vice.window_pos; // delete the list unit of the vice edge
		if (tmp)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tsl_head == tmp)
				tsl_head = tmp->next;
			if (tsl_tail == tmp)
				tsl_tail = tmp->prev;
			delete tmp;
		}

		table[pos].vice.reset(); // reset the bucket;
	}

	void update_sample(int pos, long time)
	{
		table[pos].timestamp = time;
		list_unit* tmp = table[pos].window_pos; // delete the old copy in the list;
		if (tmp)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tsl_head == tmp)
				tsl_head = tmp->next;
			if (tsl_tail == tmp)
				tsl_tail = tmp->prev;
			delete tmp;
		}
		list_unit* cur = new list_unit;
		cur->pos = pos;
		cur->next = NULL;
		cur->prev = tsl_tail;
		if (tsl_tail)
			tsl_tail->next = cur;
		tsl_tail = cur;
		if (!tsl_head)
			tsl_head = cur;
		table[pos].window_pos = cur;
	}

	void update_vice(int pos, long time)
	{
		table[pos].vice.timestamp = time;
		list_unit* tmp = table[pos].vice.window_pos; // delete the old copy in the list;
		if (tmp)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tsl_head == tmp)
				tsl_head = tmp->next;
			if (tsl_tail == tmp)
				tsl_tail = tmp->prev;
			delete tmp;
		}
		list_unit* cur = new list_unit;
		cur->pos = pos;
		cur->next = NULL;
		cur->prev = tsl_tail;
		if (tsl_tail)
			tsl_tail->next = cur;
		tsl_tail = cur;
		if (!tsl_head)
			tsl_head = cur;
		table[pos].vice.window_pos = cur;
	}
};
