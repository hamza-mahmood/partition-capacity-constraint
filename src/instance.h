#ifndef INSTANCE_H_INCLUDED
#define INSTANCE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>
#include "assert.h"
#include "instance_def.h"
#include "traffic.h"

using namespace std;

void create_random_neighbour(const Instance &instance, Instance *neighbour);

void read_instance(ifstream &instance_file, Instance *instance);
void show_instance(const Instance &instance); 

void mock_initialize_partitions(Instance *test_instance);
void naive_initialize_partitions(Instance *instance);
void greedy_initialize_partitions(Instance *instance);

#endif
