# TriangleCounting
The source code for paper "Sliding Window-based Approximate Triangle Counting over Streaming Graphs with Duplicate Edges"

The code is divided into the following parts:

SWTC-structure cantains the structure of SWTC. The head file sample.h defines class sample, which is used in SWTC algorithm. It has 3 functions: 
Initialization funtion sample(int size, int w), where size is the parameter k, namely the number of substreams, all the maximum number of sampled edges, and w is the sie of the sliding window N.
Insertion function proceed(unsigned long s, unsigned long d, long time): insert an edge s, d with timestamp time.
count(): returns the triangle count estimation.

weighted-SWTC contains the structure of SWTC for weighted counting. The files, classes and functions are organizetion in the same manner as SWTC-structure.

baseline-structure contains the structure of the baseline method. The head file BPS-sample.h defines class BPSsample, which is used in baseline algorithm. It also has 3 functions, the initialization
function BPSsample(int size, int w), Insertion function proceed(unsigned long s, unsigned long d, long time), and count(). These meaning of parameters of these functions is the same as the SWTC-structure.

weighted-baseline contains the structure of baseline for weighted counting. The files, classes and functions are organizetion in the same manner as baseline-structure.

Golden contains the structure used for computing accurate triangle count. The head file GoldenCounter.h defines class GoldenCounter. It mainly has 3 functions. the initialization
function GoldenCounter(int w), where w is the size of the sliding window.  Insertion function insert_edge(string s, string d, long time), and triangle_count() which returns the triangle number.

weighted-Golden contains the structure used for computing accurate weighted triangle count. The files, classes and functions are organizetion in the same manner as Golden.

common_structure contains structs, classes and functions shared by all the above algorithms.

The example.cpp gives an example of using these algorithms. It reads the data file "stackoverflow.txt", and varies the sliding window size from 4M to 5M, inceased by 0.1 million each time, to carry out
a set of experiments. In each experiment, it sets 100 checkpoints, and compute the triangle estimation of SWTC, baseline and golden standard at each checkpoint. The sample rate is 0.04 for SWTC and baseline,
which means their max sample size is 0.04 of the sliding window size. 
The result is output as "stack_4%_xx" where xx is  (sliding window size)/0.1M. Each output file contains 100 checkpoints, and each checkpoint contains the following 3 lines:

"tc triangle "   (the valid sample size of SWTC)  (the triangle estimation of SWTC)

"BPS triangle "   (the valid sample size of baseline)  (the triangle estimation of baseline)

"standard count " (the distinct edges in the sliding window) (the accurate triangle count in the sliding window).


The data are not uploaded to Github, as they are too large. But we give the link of the 3 datasets as follows:

Wikipedia:  http://konect.uni-koblenz.de/networks/dbpedia-link

stackoverflow : http://snap.stanford.edu/data/sx-stackoverflow.html

yahoo: https://webscope.sandbox.yahoo.com/catalog.php?datatype=g

actor: http://konect.cc/

The dataset need to be sorted according to timestamps first.

Note that counting accurate triangle count is time consuming therefore the example.cpp takes a long time to finish.
