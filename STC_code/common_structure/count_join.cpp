#include<iostream>
#include<string>
#include<vector>
#include<map>
#include <math.h> 
#include<algorithm>
using namespace std;

#ifndef B
#define B 
template<typename T>
int count_join(vector<T> &s, vector<T> &d)
{
	if (s.empty() || d.empty())
		return 0;
	int sum = 0;
	if (s.size() > d.size())
	{
		std::sort(d.begin(), d.end());
		for (int i = 0; i < s.size(); i++)
		{
			int l = 0;
			int r = d.size() - 1;
			while (l <= r)
			{
				int tmp = l + (r - l) / 2;
				if (s[i] == d[tmp])
				{
					sum++;
					break;
				}
				else if (s[i] < d[tmp])
					r = tmp - 1;
				else
					l = tmp + 1;
			}
		}
	}
	else
	{
		std::sort(s.begin(), s.end());
		for (int i = 0; i < d.size(); i++)
		{
			int l = 0;
			int r = s.size() - 1;
			while (l <= r)
			{
				int tmp = l + (r - l) / 2;
				if (d[i] == s[tmp])
				{
					sum++;
					break;
				}
				else if (d[i] < s[tmp])
					r = tmp - 1;
				else
					l = tmp + 1;
			}
		}
	}

	return sum;
}

template<typename T>

unsigned long long weighted_count(vector<T> &s, vector<T> &d, map<T, int> &ms, map<T, int> &md)
{
    if (s.empty() || d.empty())
		return 0;
	unsigned long long sum = 0;
	if (s.size() > d.size())
	{
		std::sort(d.begin(), d.end());
		for (int i = 0; i < s.size(); i++)
		{
			int l = 0;
			int r = d.size() - 1;
			while (l <= r)
			{
				int tmp = l + (r - l) / 2;
				if (s[i] == d[tmp])
				{
					int c1 = ms[s[i]];
					int c2 = md[d[tmp]];
					sum+= c1*c2;
					break;
				}
				else if (s[i] < d[tmp])
					r = tmp - 1;
				else
					l = tmp + 1;
			}
		}
	}
	else
	{
		std::sort(s.begin(), s.end());
		for (int i = 0; i < d.size(); i++)
		{
			int l = 0;
			int r = s.size() - 1;
			while (l <= r)
			{
				int tmp = l + (r - l) / 2;
				if (d[i] == s[tmp])
				{
					int c1 = md[d[i]];
					int c2 = ms[s[tmp]];
					sum+=c1*c2;
					break;
				}
				else if (d[i] < s[tmp])
					r = tmp - 1;
				else
					l = tmp + 1;
			}
		}
	}

	return sum;
}
#endif
