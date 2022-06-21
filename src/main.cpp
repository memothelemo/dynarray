#include "../include/dynarray/core.hpp"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <vector>

using dynarray::Array;

using std::ratio;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::chrono::time_point_cast;
using std::chrono::nanoseconds;

static int TIMES = 1000000;

static std::vector<int> VECTOR_COLLECTION = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Array accepts array pointers btw, but you'll
// need to specify the size.
static Array<int> ARRAY_COLLECTION = Array<int>((int[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10);

static std::vector<int>::iterator VEC_BEGIN = VECTOR_COLLECTION.begin();

#define BENCHMARK_WITH_PREREQ(name, prereq, times, block) \
    void name() { \
        Array<duration<long, ratio<1, 1000000000>>> samples; \
        for (int i = 0; i < times; i++) { \
            time_point<high_resolution_clock> start_pt, end_pt; \
            start_pt = high_resolution_clock::now(); \
            prereq \
            block \
            end_pt = high_resolution_clock::now(); \
            auto start = time_point_cast<nanoseconds>(start_pt); \
            auto end = time_point_cast<nanoseconds>(end_pt); \
            samples.push((end - start)); \
        } \
        double lowest = 1000000; \
        double totalTime = 0; \
        double highest = 0; \
        for (auto time: samples) { \
            double count = time.count(); \
            if (count < lowest) \
            { \
                lowest = count;\
            } \
            if (count > highest) \
            { \
                highest = count;\
            } \
            totalTime += count; \
        } \
        double average = (totalTime / times);\
        std::cout << "[" << __FUNCTION__ << "]\t" \
            << "Samples: " << times \
            << "\t Average: " << average << "ns" \
            << "\t Highest: " << highest << "ns" \
            << "\t Lowest: " << lowest << "ns" \
            << std::endl; \
    } \

BENCHMARK_WITH_PREREQ(vec_push,
    std::vector<int> array;,
    TIMES,
    array.push_back(10););

BENCHMARK_WITH_PREREQ(vec_pop,
    std::vector<int> array = {1};, 
    TIMES,
    array.pop_back(););

BENCHMARK_WITH_PREREQ(vec_push_pop,
    std::vector<int> array;,
    TIMES,
{
    array.push_back(10);
    array.pop_back();
});

BENCHMARK_WITH_PREREQ(arr_push,
    Array<int> array;,
    TIMES,
    array.push(10););

BENCHMARK_WITH_PREREQ(arr_pop,
    Array<int> array; array.push(1);, 
    TIMES, array.pop(););

BENCHMARK_WITH_PREREQ(arr_push_pop,
    Array<int> array;,
    TIMES,
{
    array.push(10);
    array.pop();
});

int main() {
    for (int i = 0; i < TIMES; i++) {}

    vec_push();
    arr_push();

    vec_pop();
    arr_pop();

    vec_push_pop();
    arr_push_pop();
    return 0;
}
