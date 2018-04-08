#include "input_output.h"

input_output::input_output()
{
   network_path = get_current_working_dir();
}

void input_output::read_parameters()
{

    network_path = get_current_working_dir();
    network_path += "/parameters/network";
    ifstream f;
    f.open(network_path,ifstream::in);
    read_war_path(f);
    read_type_path(f);
    read_arc_input(f);
    read_preemption(f);
    f.close();
}

void input_output::read_war_path(ifstream &f)
{

    /*
     *   Read the information of the paths that leads
     * to all warehouses eg:
     * 2         \\there is two warehouses
     * 2 1 2     \\the first have 2 paths, 1 and 2
     * 1 3       \\ the second one path, 3
     *
     */

    f >> s->n_warehouse;
    f >> s->n_paths;
    int n_paths,path;
    for(int i=0;i<s->n_warehouse;i++)
    {
        f >> n_paths;
        vector<int> paths(n_paths);
        for(int j=0;j<n_paths;j++)
        {
            f >> path;
            paths[j] = path;
        } s->war_path.push_back(paths);
    }
    char garbage[1];
    f >> garbage;

}

void input_output::read_type_path(ifstream &f)
{

    /*
     *   Read the information of the paths that a type
     * of truck could be processed in:
     * 2         \\there are two types of trucks
     * 2 1 2     \\ the first one could be processes in 2 paths, 1 and 2
     * 1 3       \\ the second in one path, 3
     *
     */

    f >> s->n_truck_types;
    int numb_paths,path;
    for(int i=0;i<s->n_truck_types;i++)
    {
        f >> numb_paths;
        vector<int> paths(numb_paths);
        for(int j=0;j<numb_paths;j++)
        {
            f >> path;
            paths[j] = path;
        } s->type_path.push_back(paths);
    }

    s->type_processing_times = vector<vector<int>>(s->n_truck_types);
    for(int i=0;i<s->n_truck_types;i++)
    {
        s->type_processing_times[i] = vector<int>(s->n_paths);
        for(int j=0;j<s->n_paths;j++)
        {
            f >> s->type_processing_times[i][j];
        }
    }

    char garbage[1];
    f >> garbage;
}

void input_output::read_preemption(ifstream &f)
{
    /*
     * For all the paths (nodes), read all the nodes
     * that it is allowed to make a preemption with the
     * first one.
     */

     s->nodes_preemption = vector<vector<int>>(s->n_paths);
    for(int i=0;i<s->n_paths;i++)
    {
        int n_preemption;
        f >>n_preemption;
        vector<int> v_preemption(n_preemption);
        for(int j=0;j<n_preemption;j++)
        {
            f >> v_preemption[j];
        }
        s->nodes_preemption[i] = v_preemption;
    }

}

void input_output::read_arc_input(ifstream &f)
{


    /*
     * Read, for all the possible paths, the type of arcs that are
     * attached to them, and the nodes where those arcs must
     * be connected to
     */

    s->input_arc_info = vector<schedule_flow::arc_info>(s->n_paths);
    s->nodes_with_mandatory_arcs = vector<vector<int>>(s->n_paths);
    int n_arc_types,arc_type,n_arcs;
    for(int i=0;i<s->n_paths;i++)
    {
        s->input_arc_info[i].arcs = vector<vector<int>>(3);
        f >> n_arc_types;
        for(int j=0;j<n_arc_types;j++)
        {
            f >> arc_type;
            f >> n_arcs;
            vector<int> arcs(n_arcs,0);
            for(int k=0;k<n_arcs;k++)
            {
                f >> arcs[k];
            }
            s->input_arc_info[i].arcs[arc_type] = arcs;
            //if the arc is amandatory one, holds in a vector with no duplicates
            if(arc_type == 1)
            {
                for(int k=0;k<arcs.size();k++)
                {
                    if(find(s->nodes_with_mandatory_arcs[arcs[k]].begin(),s->nodes_with_mandatory_arcs[arcs[k]].end(),i)==s->nodes_with_mandatory_arcs[arcs[k]].end())
                    {
                        s->nodes_with_mandatory_arcs[arcs[k]].push_back(i);
                    }
                }
            }
        }
    }

    char garbage[1];
    f >> garbage;
}
