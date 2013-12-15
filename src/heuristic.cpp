#include "heuristic.h"

// Local search that returns a local optimum.
//      @param[in] instance: instance (feasible solution)
//      @param[out] best_instance: result 
void local_search(const Instance &initial_instance,
                  const NeighbourhoodType neighbourhood,
                  Instance *best_instance) {
    /*
    int best_value = total_inter_traffic(initial_instance);
        neighbour = neighbour(initial_instance);
    do {
        int improvement = improvement(initial_instance, neighbour.next());
    } while (improvement < 0)
    neighbour = create_neighbour_of(initial_instance, i);
    */
}

bool neighbour(const Instance &instance,
               const NeighbourhoodType neighbourhood) {
    return false;
}

int index(const int i_size, const int i, const int j) {
    return i * i_size + j;
}

int giving_variation(const Instance &instance, const int given_node,
                     const int from_partition, const int to_partition) {
    vector<int> from_nodes;
    nodes_in_partition(from_partition, instance, &from_nodes);
    vector<int> to_nodes;
    nodes_in_partition(to_partition, instance, &to_nodes);

    auto not_found = instance.traffic_of_edge.end();

    int increase = 0;
    int decrease = 0;

    for (auto node : to_nodes) {
        pair<int, int> edge = proper_edge(given_node, node);
        if (instance.traffic_of_edge.find(edge) != not_found) {
            cout << "  removing edge: " << edge.first << "-" << edge.second
                 << " of cost: "
                 << instance.traffic_of_edge.find(edge)->second << endl;
            decrease += instance.traffic_of_edge.find(edge)->second;
        }
    }
    for (auto node : from_nodes) {
        if (given_node == node) {
        } else {
            pair<int, int> edge = proper_edge(given_node, node);
            if (instance.traffic_of_edge.find(edge) != not_found) {
                cout << "  adding edge: " << edge.first << "-" << edge.second
                     << " of cost: "
                     << instance.traffic_of_edge.find(edge)->second << endl;
                increase += instance.traffic_of_edge.find(edge)->second;
            }
        }
    }
    return increase - decrease;
}

int giving_first_improving(Instance &instance) {
    for (int p1 = 0; p1 < int(instance.partitions.size()); ++p1) {
        vector<int> nodes;
        nodes_in_partition(p1, instance, &nodes);
        for (auto n : nodes) {
            for (int p2 = 0; p2 < int(instance.partitions.size()); ++p2) {
                if (!is_partition_empty(instance, p1) &&
                    !is_partition_empty(instance, p2) &&
                    p1 != p2) {
                int variation = giving_variation(instance, n, p1, p2);
                cout << "Giving node " << n
                     << " from partition " << p1
                     << " to partition " << p2
                     << endl;
                cout << "Variation: " << variation 
                     << endl;
                    if (variation < 0) {
                        return variation; 
                    }
                }
            }
        }
    }
    return 0;
}

int giving_best_improving(Instance &instance) {
    int best_improvement = 0;
    for (int p1 = 0; p1 < int(instance.partitions.size()); ++p1) {
        vector<int> nodes;
        nodes_in_partition(p1, instance, &nodes);
        for (auto n : nodes) {
            for (int p2 = 0; p2 < int(instance.partitions.size()); ++p2) {
                if (!is_partition_empty(instance, p1) &&
                    !is_partition_empty(instance, p2) &&
                    p1 != p2) {
                int variation = giving_variation(instance, n, p1, p2);
                cout << "Giving node " << n
                     << " from partition " << p1
                     << " to partition " << p2
                     << endl;
                cout << "Variation: " << variation 
                     << endl;
                    if (variation < best_improvement) {
                        best_improvement = variation; 
                    }
                }
            }
        }
    }
    cout << "Best improvement: " << best_improvement << endl;
    return best_improvement;
}

/*
int first_improving_neighbour() {
    for (int i = 0; i < i_max; ++i) {
        for (int j = 0; j < j_max; ++j) {
            changes = changes_to_get_neighbour(index(i, j));
            variation = cost(changes);
            if (variation > 0) {
                neighbour = changes;
                return variation;
            }
        }
    }
}

*/




