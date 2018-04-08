#include "simulation.h"

simulation::simulation()
{

    schedule_flow s;
    io_file input;
    input.s = &s;
    input.read_parameters();
    input.read_input();
    s.optimize();


}
