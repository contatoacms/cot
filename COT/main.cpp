#include <iostream>
#include <job.h>
#include <node.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <file_manager.h>
#include <schedule_flow.h>
#include <algorithm>
#include <list>

using namespace std;



int main()
{

    schedule_flow s;





    vector<vector<int>> v;
    v = vector<vector<int>>(10);

    vector<int> vv;
    vv.push_back(1);
    v[2].push_back(1);

    list<int> l,r;
    l.push_back(1);
    l.push_back(3);
    l.push_back(2);


    while(l.size()!=0)
    {
        list<int>::iterator it = l.begin();
        r.push_back(*it);
        l.remove(*it);
    }


    cout << " \n fim \n";
    return 0;
}
