// Copyright 2020 Pauzin Leonid
#ifndef MODULES_TASK_1_PYATCKIN_N_SHELL_SORT_PYATCKIN_N_SHELL_SORT_H_
#define MODULES_TASK_1_PYATCKIN_N_SHELL_SORT_PYATCKIN_N_SHELL_SORT_H_

#include <vector>


std::vector <int> ShellSort(const std::vector <int> &vec);
std::vector<int> myMerge(std::vector<int> vec1,
    std::vector<int> vec2, std::vector<int> result);
std::vector <int> mySort(std::vector<int> vec1);
std::vector<int> getRandomVector(int n);

#endif  // MODULES_TASK_1_PYATCKIN_N_SHELL_SORT_PYATCKIN_N_SHELL_SORT_H_
