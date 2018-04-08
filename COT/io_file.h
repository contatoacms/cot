#ifndef IO_FILE_H
#define IO_FILE_H
#include <input_output.h>

class io_file:public input_output
{
public:
    io_file();
    void read_input();
private:
    void read_war_input(ifstream &f);
    void read_truck_input(ifstream &f);
};

#endif // IO_FILE_H
