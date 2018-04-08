#include "io_file.h"

io_file::io_file()
{

}

void io_file::read_input()
{

    ifstream f;
    network_path = get_current_working_dir();
    network_path += "/parameters/input";
    f.open(network_path,ifstream::in);
    read_war_input(f);
    f.close();

    string truck_input_path = get_current_working_dir();
    truck_input_path += "/parameters/input_truck";
    f.open(truck_input_path,ifstream::in);
    read_truck_input(f);
    f.close();

}

void io_file::read_war_input(ifstream &f)
{
    int att;
    float volume;
    for(int i=0;i<s->n_warehouse;i++)
    {
        f >> att; f >> volume;
        s->input_warehouse.push_back(make_pair(att,volume));
    }

}

void io_file::read_truck_input(ifstream &f)
{
    f >> s->n_trucks;
    s->input_truck = vector<schedule_flow::truck_info>(s->n_trucks);
    for(int i=0;i<s->n_trucks;i++)
    {
        schedule_flow::truck_info tr;
        f >> s->input_truck[i].type;
        f >> s->input_truck[i].att;
        f >> s->input_truck[i].vol;
        f >> s->input_truck[i].arr_time;
    }

}
