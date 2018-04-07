#include <file_manager.h>

std::string get_current_working_dir(void)
{

    /*
     * Gets the current directory path
     */

  char buff[FILENAME_MAX];
  GetCurrentDir( buff, FILENAME_MAX );
  std::string current_working_dir(buff);
  return current_working_dir;

}

std::vector<int> concatenate_vector(std::vector<int> v1, std::vector<int> v2)
{

    /*
     *  Returns a vector with two
     * concatenated vectors, removing duplicates
     */

    v1.insert(v1.end(),v2.begin(),v2.end()); //concatenate v2 into v1
    sort(v1.begin(),v1.end());               //sort
    std::vector<int>::iterator it;
    it = unique(v1.begin(),v1.end());        //iterator with the unique elements of the vector
    v1.resize(distance(v1.begin(),it));
    return v1;

}

std::vector<int> intersection(std::vector<int> &v1, std::vector<int> &v2)
{

    std::vector<int> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v3));

    return v3;
}

void copy_vector_without_duplicate(std::vector<int> &v_copy, std::vector<int> &v)
{
    for(int i=0;i<v.size();i++)
    {
        if(find(v_copy.begin(),v_copy.end(),v[i]) == v_copy.end())
        {
            v_copy.push_back(v[i]);
        }
    }
}
