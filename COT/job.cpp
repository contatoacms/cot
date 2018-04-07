#include "job.h"
//#include "schedule_flow.h"

job::job()
{

}

job::job(int n_paths, vector<int> admissible_nodes/*schedule_flow &v*/)
{

   /*
    * The schedule_flow &v is necessary to access
    * the arcs that must be attributed to the nodes,
    * and that information is held "globally" at the
    * schedule class
    */

    //this->v = &v;
    initialize_node_column(n_paths,admissible_nodes);

}

job::~job()
{

}

void job::initialize_node_column(int &n_paths, vector<int> &admissible_nodes)
{

    /*
     *  With the admissible_nodes, creates nodes in
     * a map, where the key is the "number of the
     * path", and the value is a node
     */

    for(int i=0;i<admissible_nodes.size();i++)
    {
         node n;
         node_column[admissible_nodes[i]] = n;
    }
}


