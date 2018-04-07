#ifndef JOB_H
#define JOB_H

#include<vector>
#include<map>
#include"node.h"

class schedule_flow;

using namespace std;

class job
{
public:

    job(int n_paths, vector<int> admissible_nodes);
    job();
    ~job();
    map<int,node> node_column;
    vector<int> painted_node; //keys of the node_column that are painted

    /*  if a node with mandatory arc is used, it means that
        some other node must necessarily have the same color as this,
        this variable (twin) holds the index of the job, in the
        'schedule_flow' 'jobs' that has the same color
    */

    int twin = -1;

private:


    void initialize_node_column(int &n_paths, vector<int> &admissible_nodes);

};

#endif // JOB_H
