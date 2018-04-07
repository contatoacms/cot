#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#endif // FILE_MANAGER_H

#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>
#include<vector>
#include<algorithm>


std::string get_current_working_dir(void);
std::vector<int> concatenate_vector(std::vector<int> v1, std::vector<int> v2);
std::vector<int> intersection(std::vector<int> &v1, std::vector<int> &v2);
void copy_vector_without_duplicate(std::vector<int> &v_copy,std::vector<int> &v);
