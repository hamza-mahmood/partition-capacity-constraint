#ifndef HEURISTIC_H_INCLUDED
#define HEURISTIC_H_INCLUDED

#include <iostream>
#include "instance_def.h"
#include "traffic.h"

using namespace std;

enum NeighbourhoodType {
    Trade,
    Give,
    Complement,
};

bool is_local_optimum(const Instance &instance);

void local_search(const Instance &instance,
                  const NeighbourhoodType n,
                  Instance *best_instance);

void simulated_annealing(const Instance &instance,
                         const double temperature,
                         //const cooling_function_t cooling_function,
                         const int epoch,
                         Instance *best_instance);

int giving_variation(const Instance &instance, const int given_node,
                     const int from_partition, const int to_partition);
int giving_first_improving(Instance &instance);
int giving_best_improving(Instance &instance);

#endif
