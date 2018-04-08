#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H
#include <schedule_flow.h>
#include <file_manager.h>
#include <string>

class input_output
{
public:
    input_output();

    string network_path;
    schedule_flow* s;
    void read_parameters();
    virtual void read_input() = 0;


private:
    void read_war_path(ifstream &f);
    void read_type_path(ifstream &f);
    void read_preemption(ifstream &f);
    void read_arc_input(ifstream &f);

};

#endif // INPUT_OUTPUT_H
