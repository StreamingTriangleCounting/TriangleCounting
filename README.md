# TriangleCounting
The source code for paper "Sliding Window-based Approximate Triangle Counting over Streaming Graphs with Duplicate Edges"

The code is divided into the following directories:
## common

it contains basic structures shared by the baseline and SWTC, including Node.h, nodetable.h and edgetable.h for sample graph maintaining, count_join.h for counting joined parts of two vectors (used in triangle counting) and hashfunctions. Node-direct.h and nodetable-direct.h are directed-graph-version of Node.h and nodetable.h. undirected-setting.h and directed-setting.h include settings for undirected or directed counting semantics.


## SWTC-structure

It cantains codes of SWTC. It is further divided into 4 directories:

**undirected**: SWTC-codes for undirected, binary counting. There are 4 files:
sample.h: code of sample class, which is a SWTC-sampler. It has 6 interfaces:

void sample(int size, int w, int hi): initialization of the sampler. size is the number of substreams, k. w is the window length (N multiplies the time unit, which is the average time span of the dataset), and hi is the hashindex. Change hi from 0 to 10 will enable the SWTC to be implemented with different hash functions.

void proceed(unsigned int s, unsigned int d, long long time): proceed an undirected edge (s, d) with timestamp time. Current time T will be updated to parameter time.

void prepare(): make preparations for querying, need to be called before calling of the following functions.

int count(), return the estimated global triangle count.

int local_count(unsigned int v): return the local count of node v.

void all_local(unordered_map<unsigned int, int> &cr), store local count of all sampled nodes in cr as <node ID， local count> pair. Nodes not in the sampled graph is not reported. The estimated local count of these nodes is 0.

sampletable.h: code of SampleTable class, which maintaings the sample graph and is the major component of sample class.

asy_sample.h code of asy_sample class, which is a SWTC-sampler with AG technique. It has the same interfaces as the sample class, except the initialization function:
void asy_sample(int size, int w, int g, int hi): size, w, and hi are the same as sample. g is the number of groups in AG technique.

asy_sampletable.h: code of asy_sampletable, which maintaings the sample graph and is the major component of asy_sample class.

**directed**: SWTC-codes for directed, binary counting. There are 4 files
sample-direct.h: direct version of sample.h. It has the same interfaces as sample.h 

sampletable-direct.h: direct version of sampletable.h.

asy_sample-direct.h direct version of asy_sample.h. It has the same interfaces as asy_sample.h 

asy_sampletable-direct.h: direct version of asy_sampletable.h.

**weighted**: SWTC-code for undirected, weighted counting. There are 4 files:
sample-weighted: weighted version of sample.h. It has the same interfaces as sample.h, except that count() function returns unsigned long long type result, as the triangle count may be very large.

sampletable-weighte: weighted version of sampletable.h.

asy_sample-weighted: weighted version of asy_sample.h, except that count() function returns unsigned long long type result.

asy_sampletable-weighted: weighted version of asy_sampletable.h

**no-vision**: SWTC-code for undirected, binary counting, without vision counting technique or AG technique. We only use it as a comparison. Thus the code is simple with limmited queries supported.
no_vision.h: code for nv_sample class, which is a SWTC-sample without vision counting or AG grouping. It has 3 interfaces:
void nv_sample(int size, int w, int hi): initialization function. The parameters are the same as sample.h
void proceed(unsigined int s, unsigned int d, long long time): the same interface as proceed() in sample.h
void count(): estimate the number of global triangles in the sliding window. We use prepare() function in sample.h and asy_sample.h as they support both local and global count, and these query functions share the same preparation steps. But in no_vision, we only provide interfaces of global counting. Thus the steps in prepare are included in count();

## BPS-structure
It contains code of the baseline method. It is further divided into 3 directories:

**undirected** 
Code of the baseline method for undirected, binary counting. It includes 2 files:
BPS-sample.h: code for BPSsample class, which is a baseline sampler. it has 6 interfaces:
void BPS-sample(int size, int w, in hi): initialization function, parameters are the same as initialization function of sample.h
proceed(), count(), local_count(), all_local(): functions to process new edges, estimate global triangles, estimate local count of one node, and estimate local count of all nodes. Parameters and returned values are the same as functions of the same name;

BPS-sampletable.h: code for BPSSampleTable class, which maintains the sample graph and is the major component of the BPSsample class.

**directed** 
Code of the baseline method for directed, binary counting. It includes 2 files: BPS-sample-direct.h and BPS-sampletable-direct.h. They are the directed version of BPS-sample.h and BPS-sampletable.h, and have the same interfaces as the corresponding undirected version.


**weighted**
Code of the baseline method for undirected, weighted counting. It includes 2 files: BPS-sample-weighted.h and BPS-sampletable-weighted.h. They are the weighted version of BPS-sample.h and BPS-sampletable.h, and have the same interfaces as the corresponding binary version (except that the returned value of count() function in BPS-sample-weighted in unsigned long long type).


## Golden-triangle
Code for GoldenCounter, which count the accurate triangle number in the sliding window. It is divided into 2 directories:
** 
contains 2 files:
GoldenCounter.h: code for GoldenCounter class, the accurate triangle counter. It contains 8 interfaces:

void GoldenCounter(int w) initialization function, where w is the window length (N multiplies the time unit, which is the average time span of the dataset)
void insert_edge(unsigned int s, unsigned int d, long long time): insert an edge (s, d) with timestamp time, and update current time T to parameter time, triggering deleting expeired edges.
int duplicate_count(): count duplicate edge number in the sliding window.
int edge_count(): count distinct edge number in the sliding window.
int triangle_count(): binary count the global triangles in the sliding window.
unsigned long long weighted_count(): weighted count the global triangles in the sliding window.






The "demo/estimate-wiki-undirect.cpp" gives an example of using these algorithms. It reads the data file "../../data/WikiTalk.txt", and sets the window length to 2M*average time span. It varies the number of substreams in the baseline method and SWTC from 40k to 120k to carry out experiment, with intervals of 20k. For each substream number, it carries out 5 experiments with different hashfunctions.
In each experiment, it set a checkpoint whenever the window slides 1/10 of its length (the first 2 windows have no checkpoints), and compute the triangle estimation of SWTC, baseline and golden standard at each checkpoint.
The result is output as "result/undirected/Wiki/EstimateResult_wiki_undirect_2M_h?_xx" where ? is the hash index, varying from 0 to 4. xx is  (number of substreams)/20k. Each output file contains multiple check points, and each checkpoint contains the following 4 lines:

"asy triangle "   (the valid sample size of SWTC-AG)  (the triangle estimation of SWTC-AG)

"tc triangle "   (the valid sample size of SWTC)  (the triangle estimation of SWTC)

"BPS triangle "   (the valid sample size of baseline)  (the triangle estimation of baseline)

"standard count " (the distinct edges in the sliding window) (the accurate triangle count in the sliding window).

By including different files (directed or weighted), experiment can be implemented in different semantics. In directed counting, triangle_type need to be defined. 
 0 means directed triangle type A and 1 means directed triangle type B.

The data are not uploaded to Github, as they are too large. But we give the link of the 4 datasets as follows:

Wikitalk:  http://konect.cc/networks/wiki_talk_en/

stackoverflow : http://snap.stanford.edu/data/sx-stackoverflow.html

yahoo: https://webscope.sandbox.yahoo.com/catalog.php?datatype=g

actor: http://konect.cc/

The dataset need to be sorted according to timestamps first.

Note that counting accurate triangle count is time consuming therefore the example.cpp takes a long time to finish.
