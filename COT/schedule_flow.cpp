#include "schedule_flow.h"

schedule_flow::schedule_flow()
{

    //optimize();

}

void schedule_flow::optimize()
{
    initialize_graph();
    optimize_constructive();
    print_log();
}

void schedule_flow::initialize_graph()
{
    //read_data();
    create_att_path();
    create_permissible_nodes();
    create_jobs();
}

void schedule_flow::create_att_path()
{

    /*
     *  Fill's the att_path, for each attribute, the
     * paths that lead to the warehouses containing
     * the same attribute
     */

    for(int i=0;i<n_attribute;i++)
    {
        vector<int> v;
        att_path.push_back(v);
    }

    for(int i=0;i<n_warehouse;i++)
    {
        for(int j =0;j<war_path[i].size();j++)
        {
            att_path[input_warehouse[i].first].push_back(war_path[i][j]);
        }
    }


}

void schedule_flow::create_permissible_nodes()
{

    /*
     *   Creates the matrix with permissible node, for
     * each combination attribute x truck_type, the paths permissible are given:
     *
     * a   truck_types
     * t     |   1    |   2   |  3  |  4  |  5  |
     * r    1|_{1,2,3}|       |     |     |     |
     * t    2|_{1,3}__|       |     |     |     |
     * i    3|{1,2,3}_|       |     |     |     |
     *
     * eg. for atruck of type 1, containing a cargo
     * with attribute 1, the paths that he could use are {1,2,3}
     */


    for(int i=0;i<att_path.size();i++)
    {
        vector<vector<int>> v;
        for(int j=0;j<type_path.size();j++)
        {
            v.push_back(intersection(att_path[i],type_path[j]));
        }
        permissible_nodes.push_back(v);
    }

}

void schedule_flow::create_jobs()
{
    /*
     * Create the vector of jobs
     * for each input truck, there must be
     * a job, inside of it, a "column" of
     * permissible nodes to operate in.
     * Populates the
     */


    for(int i=0;i<input_truck.size();i++)
    {
        //vector<int> a = permissible_nodes[input_truck[i].type][input_truck[i].att];
        job j(n_paths,permissible_nodes[input_truck[i].att][input_truck[i].type]);
        jobs.push_back(j);
        if(permissible_nodes[input_truck[i].att][input_truck[i].type].size() !=0 )
        {
            unpainted_jobs.push_back(i);
        }

    }

}

void schedule_flow::optimize_constructive()
{

    /*
     * Give an initial constructive solution;
     * for the first to the last job, select the
     * first feasible color, on the first
     * node permissible
     */

    while(unpainted_jobs.size()!=0) //For each unpainted_job
    {
        list<int>::iterator it_job = unpainted_jobs.begin(); //allways selecting the first element of the list
        map<int,node>::iterator it_node;
        map<int,feasibility> feasibility_hash;
        for(it_node = jobs[*it_job].node_column.begin(); it_node!=jobs[*it_job].node_column.end();it_node++)
        {
            feasibility_hash[it_node->first] = create_feasibility(*it_job,it_node->first);
        }
        pair<int,int> twin_job_node;
        int selected_node = select_node(feasibility_hash,twin_job_node);

        if(twin_job_node.first != -1)
        {
            paint_node(*it_job,selected_node,twin_job_node.first,twin_job_node.second,
                       make_pair(feasibility_hash[selected_node].disjoint_color.second + 1,
                                feasibility_hash[selected_node].disjoint_color.second + 1 + type_processing_times[input_truck[*it_job].type][selected_node]));
        }else{
            paint_node(*it_job,selected_node,make_pair(feasibility_hash[selected_node].disjoint_color.second + 1,
                    feasibility_hash[selected_node].disjoint_color.second + 1 + type_processing_times[input_truck[*it_job].type][selected_node]));
        }

    }

}

schedule_flow::feasibility schedule_flow::create_feasibility(int job_index, int node_key)
{

    /*
     * Collect the data to the feasibility
     * struct and returns it.
     * If the node must have an mandatory arc, but there is no mandatory arc
     * coming to it, it means that is impossible to paint that node, so
     * it is unfeasible
     *
     */


    feasibility n;
    n.disjoint_color = make_pair(0,0);

    list<int>::iterator it_painted_jobs;
    for(it_painted_jobs = painted_jobs.begin();it_painted_jobs!=painted_jobs.end();it_painted_jobs++) //for all the jobs tht are painted
    {
        int painted_node = jobs[*it_painted_jobs].painted_node[0]; //now there is only one color, no preemption allowed...yet!!
        verify_disjoint_arcs(it_painted_jobs,painted_node,n,node_key);
        verify_mandatory_disjoint_arcs(it_painted_jobs,painted_node,n,node_key);
    }


    //verify if there is mandatory arcs coming to the node
    list<int>::iterator it_unpainted_jobs;
    for(it_unpainted_jobs = unpainted_jobs.begin();it_unpainted_jobs != unpainted_jobs.end();it_unpainted_jobs++) //for all the jobs tht are painted
    {

        //cout << *it_unpainted_jobs << "  " << job_index;
        if(*it_unpainted_jobs!=job_index && jobs[*it_unpainted_jobs].twin == -1)
        {
           verify_mandatory_arc(it_unpainted_jobs,node_key,n);
        }
    }

    if((input_arc_info[node_key].arcs[1].size() != 0) && n.mandatory_possibilities.size() == 0)
    {
        n.feasible = false;
    }
    else
    {
        n.feasible = true;
    }

    return n;
}

void schedule_flow::verify_disjoint_arcs(list<int>::iterator it_painted_job, int painted_node, schedule_flow::feasibility &n, int node_key)
{

    /*
     * Verify if there is an disjoint arc between
     * the node 'node_key' and the painted node,
     * if so
     */


    if(find(input_arc_info[painted_node].arcs[0].begin(),input_arc_info[painted_node].arcs[0].end(),node_key)!=input_arc_info[painted_node].arcs[0].end())
    {//there is a connection of disjuntion from the node to the selected node
        if(jobs[*it_painted_job].node_column[painted_node].color.second > n.disjoint_color.second)
        {
            n.disjoint_color.first = jobs[*it_painted_job].node_column[painted_node].color.first;
            n.disjoint_color.second = jobs[*it_painted_job].node_column[painted_node].color.second;
        }
    }
}

void schedule_flow::verify_mandatory_disjoint_arcs(list<int>::iterator it_painted_job, int painted_node, schedule_flow::feasibility &n, int node_key)
{

    /*
     * Verify if the painted_node has a
     * mandatory connection with the node and if it has a
     * twin node, if so, his mandatory arc becomes a disjoint
     * one, because they are already painted.     *
     */


    if(find(input_arc_info[painted_node].arcs[1].begin(),input_arc_info[painted_node].arcs[1].end(),node_key)!=input_arc_info[painted_node].arcs[1].end())
    {
        //there is a mandatory connection from the node to the selected node
        if(jobs[*it_painted_job].twin != -1) //the mandatory arc already has a twin, impact as if it were a disjoint arc
        {
            if(jobs[*it_painted_job].node_column[painted_node].color.second > n.disjoint_color.second)
            {
                n.disjoint_color.first = jobs[*it_painted_job].node_column[painted_node].color.first;
                n.disjoint_color.second = jobs[*it_painted_job].node_column[painted_node].color.second;
            }
        }
    }
}

void schedule_flow::verify_mandatory_arc(list<int>::iterator it_unpainted_jobs, int node_key, schedule_flow::feasibility &n)
{

    /*
     * Verify, for all the unpainted nodes
     * those who have a mandatory arc with the 'node_key'
     * and holds the job_index and the node_ksey at the
     * mandatory_possibilities
     */

    map<int,node>::iterator it_node;
    for(it_node = jobs[*it_unpainted_jobs].node_column.begin(); it_node!=jobs[*it_unpainted_jobs].node_column.end();it_node++)
    {
        // the the job has a node with connection
        if(jobs[*it_unpainted_jobs].twin == -1)
        {
            if(find(nodes_with_mandatory_arcs[node_key].begin(),nodes_with_mandatory_arcs[node_key].end(),(*it_node).first)!=nodes_with_mandatory_arcs[node_key].end())
            {
                n.mandatory_possibilities.push_back(make_pair(*it_unpainted_jobs,(*it_node).first));
            }
        }

    }

}

void schedule_flow::paint_node(int job_index, int node_key, pair<int, int> color)
{

    /*
     * Paint the node at the jobs in job_index
     * with the color 'color',and also updates
     * the painted and unpainted jobs.
     */

    jobs[job_index].node_column[node_key].color = color;
    jobs[job_index].painted_node.push_back(node_key);

    painted_jobs.push_back(job_index);
    unpainted_jobs.remove(job_index);
}

void schedule_flow::paint_node(int job_index, int node_key,int job_index_twin, int node_key_twin, pair<int, int> color)
{

    /*
     * Paint the nodes at the jobs in job_index
     * with the color 'color', updates the 'twin'
     * for both ob them, and also updates the
     * painted and unpainted jobs.
     */

    jobs[job_index].node_column[node_key].color = color;
    jobs[job_index].painted_node.push_back(node_key);
    jobs[job_index].twin = job_index_twin;

    jobs[job_index_twin].node_column[node_key_twin].color = color;
    jobs[job_index_twin].painted_node.push_back(node_key_twin);
    jobs[job_index_twin].twin = job_index;

    painted_jobs.push_back(job_index);
    painted_jobs.push_back(job_index_twin);

    unpainted_jobs.remove(job_index);
    unpainted_jobs.remove(job_index_twin);

}

int schedule_flow::select_node(map<int, feasibility> &feasibility_hash, pair<int,int> &twin_job_node)
{

    /*
     * Iterate through the hash and saves up to two
     * nodes, the best onw with no mandatory arcs
     * and (maybe not) a feasible one with mandatory arcs.
     * choose one of the candidates for twin of the node.
     * if a node with mandatory arcs is feasible, it is
     * chosen, otherwise, the best common node is choosen
     *
     */


    int best_common_node;
    int best_common_node_initial_color = 99999; //change to 'limits <int'
    int best_mandatory_node = -1;
    twin_job_node = make_pair(-1,-1);
    //int twin_job = -1;
    //int twin_node = -1;

    map<int, feasibility>::iterator it;
    for(it = feasibility_hash.begin(); it != feasibility_hash.end();it++)
    {
        if((it)->second.feasible == true)
        {
            if((it)->second.mandatory_possibilities.size() != 0)
            { //if there is a possibility to select some node with mandatory arcs
                best_mandatory_node = it->first;
                //creates the feasibility for the first <job,node> of mandatory_feasibility
                feasibility f_twin = create_feasibility(it->second.mandatory_possibilities[0].first,it->second.mandatory_possibilities[0].second);
                if(f_twin.disjoint_color.second > it->second.disjoint_color.second)
                {
                       it->second.disjoint_color.second = f_twin.disjoint_color.second;
                }

                twin_job_node.first = it->second.mandatory_possibilities[0].first;
                twin_job_node.second = it->second.mandatory_possibilities[0].second;
            }else
            {
                if((it)->second.disjoint_color.second < best_common_node_initial_color)
                {
                    best_common_node_initial_color = it->second.disjoint_color.second;
                    best_common_node = it->first;
                }
            }
        }

    }

    if(best_mandatory_node == -1)
    {
        return best_common_node;
    }
    else
    {
        return best_mandatory_node;
    }

}

void schedule_flow::print_log()
{
    /*
     * Print the solution on the
     * console.
     */

    struct tm* local;
    time_t aux_time;
    time(&aux_time);
    local = localtime(&aux_time);

    int curr_day = local->tm_mday;
    int curr_month = local->tm_mon+1;
    int curr_year = local->tm_year + 1900;
    int curr_hour = local->tm_hour;
    int curr_min = local->tm_min;
    int curr_sec = local->tm_sec;

    create_log_header(curr_hour,curr_min,curr_sec,curr_day,curr_month,curr_year);
    print_information(curr_hour,curr_min,curr_sec);
}

void schedule_flow::print_information(int &curr_hour, int &curr_min, int &curr_sec)
{
    for(int i =0;i<jobs.size();i++)
    {
        if(jobs[i].painted_node.size() == 0)
        {
            cout << "\n# Impossible to process Job " << i << " at the moment \n";
        }else
        {
            int painted_node = jobs[i].painted_node[0];
            int h,m,s,hb,mb,sb,finish_color,beginning_color;
            finish_color = jobs[i].node_column[painted_node].color.second;/*-jobs[i].node_column[painted_node].color.first*/;
            beginning_color = jobs[i].node_column[painted_node].color.first;
            calculate_finsish_time(h,m,s,curr_hour,curr_min,curr_sec,finish_color);
            calculate_finsish_time(hb,mb,sb,curr_hour,curr_min,curr_sec,beginning_color);

            cout << "# Job " << i << " processed on path " << painted_node << " beginning : " << hb <<":" <<mb <<" - " << h <<":" <<m <<"\n";
            //cout << "# Job " << i << " processed on path " << painted_node << " with duration (" << jobs[i].node_column[painted_node].color.first << "-" <<
                                                                //jobs[i].node_column[painted_node].color.second << ") \n";
        }
    }
    cout << "\n############################################################## \n";
}

void schedule_flow::create_log_header(int &curr_hour,  int &curr_min, int &curr_sec, int &curr_day, int &curr_month,  int &curr_year)
{
    /*
     * Creates the heder for the solution
     * log, with current date and time,
     * the truck times are increased on the current time
     */



    cout << "##################### TRUCK SCHEDULE LOG ##################### \n \n" ;
    cout << "# |Date|: " << curr_day << "/" << curr_month << "/" << curr_year << "| \n";
    cout << "# |Time|: " << curr_hour << ":" << curr_min << ":" << curr_sec << "| \n \n";

}

void schedule_flow::calculate_finsish_time(int &h, int &m, int &s, int &ch, int &cm, int &cs, int &color_size)
{
    int total_min = cm + color_size;
    int increased_h = floor(total_min/60);
    h = ch + increased_h;
    m = (total_min%60);


}



