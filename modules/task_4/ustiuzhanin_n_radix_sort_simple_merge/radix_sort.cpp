// Copyright 2022 Ustiuzhanin Nikita

#include <vector>
#include <list>
#include <random>
#include <ctime>
#include <algorithm>
#include <string>
#include "../../../pp_2022_spring/modules/task_4/ustiuzhanin_n_radix_sort_simple_merge/radix_sort.h"
#include "../../../3rdparty/unapproved/unapproved.h"

using std::vector;
using std::list;

void randomVector(vector<int>* data, size_t size, size_t rad) {
    if (!size)
        return;

    data->clear();
    data->resize(size);

    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<int> dist(0, pow(10, rad));
    for (size_t i = 0; i < data->size(); i++)
        data->at(i) = dist(mersenne);
}

int getNum(int val, size_t pos) {
    while (pos > 1) {
        val /= 10;
        pos--;
    }

    return val % 10;
}

size_t rad(size_t value) {
    size_t counter = 0;
    while (value != 0) {
        value /= 10;
        counter++;
    }
    return counter;
}

void radixSort(vector<int>* data) {
    if (!data->size())
        return;

    if (std::find_if(data->begin(), data->end(), [](int val) {
        return val < 0; }) != data->end())
        throw std::string("Try sort numbers less then 0");

    size_t maxRad = rad(*std::max_element(data->begin(), data->end()));

    vector<list<int>> helpList(10);

    for (size_t i = 1; i <= maxRad; i++) {
        for (int val : *data) {
            helpList[getNum(val, i)].push_back(val);
        }

        data->clear();
        for (list<int> l : helpList) {
            for (int val : l)
                data->push_back(val);
        }

        helpList.clear();
        helpList.resize(10);
    }
}

vector<int> simpleMerge(const vector<int>& firstVector,
    const vector<int>& secondVector) {
    vector<int> resultVector(firstVector.size() + secondVector.size());

    size_t firstIt, secondIt, resutIt;
    firstIt = secondIt = resutIt = 0;

    while (firstIt < firstVector.size() && secondIt < secondVector.size()) {
        if (firstVector[firstIt] < secondVector[secondIt]) {
            resultVector[resutIt++] = firstVector[firstIt++];
        } else {
            resultVector[resutIt++] = secondVector[secondIt++];
        }
    }

    while (firstIt < firstVector.size()) {
        resultVector[resutIt++] = firstVector[firstIt++];
    }
    while (secondIt < secondVector.size()) {
        resultVector[resutIt++] = secondVector[secondIt++];
    }

    return resultVector;
}

void radixSortSTD(vector<int>* data) {
    if (!data->size())
        return;

    if (std::find_if(data->begin(), data->end(), [](int val) {
        return val < 0; }) != data->end())
        throw std::string("Try sort numbers less then 0");

    int numberOfThread = std::thread::hardware_concurrency();
    int dataPortion = data->size() / numberOfThread;

    vector<vector<int>> vecOfVec(numberOfThread);

    std::promise<vector<int>>* promises =
        new std::promise<vector<int>>[numberOfThread];
    std::future<vector<int>>* futures =
        new std::future<vector<int>>[numberOfThread];
    std::thread* threads = new std::thread[numberOfThread];

    for (int i = 0; i < numberOfThread; ++i) {
        futures[i] = promises[i].get_future();
        vector<int> local;

        if (i != numberOfThread - 1) {
            local = { data->begin() + i * dataPortion,
                     data->begin() + (i + 1) * dataPortion };
        } else {
            local = { data->begin() + i * dataPortion, data->end() };
        }

        threads[i] = std::thread([](vector<int>* commonVector,
            std::promise<vector<int>>&& pr) {
            radixSort(commonVector);
            pr.set_value(std::move(*commonVector));
            }, &local, std::move(promises[i]));
        threads[i].join();
        vecOfVec[i] = futures[i].get();
    }

    vector<int> resultVector = vecOfVec[0];
    for (int i = 1; i < numberOfThread; ++i) {
        resultVector = simpleMerge(resultVector, vecOfVec[i]);
    }

    delete[] promises;
    delete[] futures;
    delete[] threads;

    *data = resultVector;
}
