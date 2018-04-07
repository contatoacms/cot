#ifndef NODE_H
#define NODE_H

//#include<job.h>
#include<iostream>
#include<vector>

using namespace std;
class node
{
public:
    node();
    ~node();

    /*
     *Variables
     */

    pair<int,int> color;

private:

    void free_vector_memory(vector<node*> &v);

};

#endif // NODE_H
