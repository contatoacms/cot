#ifndef SCHEDULE_FLOW_H
#define SCHEDULE_FLOW_H

#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <file_manager.h>
#include <job.h>
#include <list>
#include <algorithm>
#include <limits>
#include <math.h>
//#include <input_output.h>

using namespace std;

class schedule_flow
{
public:

    schedule_flow();
    void optimize();

    struct truck_info{
        int type;
        int att;   //attribute
        float vol; //volume of the attribute
        int arr_time; //arriving time
        vector<int> processing_times;
    };

    struct arc_info{
    /*Vector with 3 positions, one for each type of arc
      * 0: disjoint arcs
      * 1: mandatory arcs
      * 2: set-up arcs
      */
    vector<vector<int>> arcs;
    };

    struct feasibility{
        /*Evaluates, for all nodes that are painted, those who has disjoint_arcs with the one
        * and for those, verify if their color is greater than the 'disjoint color,
        * if so, disjoint_color getes their color, at the end, disjoint color has the
        * 'longest' color of all the nodes connected to it        *
        */
        pair<int,int> disjoint_color;
        /*
         * Likewise the disjoint color, holds the 'longest'
         * color of a node with set_up arc connected to
         * the one evaluated
         */
        pair<int,int> set_up_color;
        /* Each pair contains ->first: the index on the vector 'jobs' that has a node not
        * yet painted with a 'mandatory' connection with the node, and second-: the key of
        * that node on the specified
        * index;
        */

        vector<pair<int,int>> mandatory_possibilities;
        bool feasible;
    };

    vector<arc_info> input_arc_info;

    int n_paths = 0;
    int n_warehouse = 0;
    int n_truck_types = 0;
    int n_attribute = 3; // 0: soy 1:corn 2:wheat
    int n_trucks = 0;
    vector<vector<int>> war_path;                  //for all warehouses, the paths that belongs to them
    vector<vector<int>> att_path;                  //for all attributes, the paths that leads to warehouses of that same attribute
    vector<vector<int>> type_path;                 //for types of trucks, the paths that they can be proccessed in
    vector<vector<int>> type_processing_times;     //for each type of truck, the processing times for all paths
    vector<vector<vector<int>>> permissible_nodes;
    vector<pair<int,float>> input_warehouse;       // for each warehouse <attribute on it, remaining space>
    vector<truck_info> input_truck;                // gets the type of truck, attribute and arriving time
    vector<job> jobs;
    vector<vector<int>> nodes_with_mandatory_arcs; //For all nodes, holds all the nodes with mandatory connections with it
    vector<vector<int>> nodes_preemption; //For all nodes, holds all the nodes with mandatory connections with it

private:

    /*
     * Routines
     */

    void initialize_graph();
    void create_att_path();
    void create_permissible_nodes();
    void create_jobs();
    void create_nodes();
    void optimize_constructive();
    void verify_disjoint_arcs(list<int>::iterator it_painted_job, int painted_node, feasibility &n, int node_key);
    void verify_mandatory_disjoint_arcs(list<int>::iterator it_painted_job, int painted_node, feasibility &n, int node_key);
    void verify_mandatory_arc(list<int>::iterator it_painted_job, int node_key, feasibility &n);
    void paint_node(int job_index, int node_key, pair<int,int> color);
    void paint_node(int job_index, int node_key,int job_index_twin, int node_key_twin, pair<int, int> color);
    int select_node(map<int, feasibility> &feasibility_hash, pair<int, int> &twin_job_node);
    void print_log();
    void print_information(int &curr_hour, int &curr_min, int &curr_sec);
    void create_log_header(int &curr_hour,  int &curr_min, int &curr_sec,  int &curr_day,  int &curr_month, int &curr_year);
    void calculate_finsish_time(int &h,int &m, int &s,int &ch,int &cm, int &cs, int &color_size);

    feasibility create_feasibility(int job_index, int node_key);

    /*
     * Variables
     */




    list<int> painted_jobs,unpainted_jobs;                      //holds all the indexes of the 'jobs' that are allready painted


};

#endif // SCHEDULE_FLOW_H
