#include "node.h"
#include<malloc.h>

node::node()
{

}

node::~node()
{

}

void node::free_vector_memory(vector<node *> &v)
{

    /*
     * Given a vector of pointer,
     * free the memory on the whole vector
     */

    for(int i=0;i<v.size();i++)
    {
        delete v[i];
    }


}
