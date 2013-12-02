#include "instance.h"

// Return first node of a partition.
//      @param[in] partition: reference to a 1D membership array.
//      @return first node
int min_node_in(const vector<bool> &partition) {
    // returns first node in partition
    for (size_t i = 0; i < partition.size(); ++i) {
        if (partition[i] == true)
            return int(i);
    }
    // if empty partition return -1
    return -1;
}

bool is_well_formed(const vector<vector<bool>> &partitions) {
    int assigned_node_count = 0;
    for (size_t i = 0; i < partitions.size(); ++i) {
        for (size_t j = 0; j < partitions[i].size(); ++j) {
            if (partitions[i][j] == true)
                assigned_node_count++;
        }
    }
    return (assigned_node_count == int(partitions.size()));
}

// Return true if partitions are labeled after their smallest element.
//      @param[in] partitions: reference to a 2D membership array.
//      @return coherence
bool is_coherent(const vector<vector<bool>> &partitions) {
    bool result = true;
    for (size_t p = 0; p < partitions.size(); ++p) {
        if (min_node_in(partitions[p]) == -1)
            result &= true;
        else
            result &= (min_node_in(partitions[p]) == int(p));
    }
    return result;
}

// Find nodes in partition p.
//      @param[in] p: index of partition
//      @param[in] instance: instance
//      @param[out] nodes: nodes in partition p
void nodes_in_partition(const int p, const Instance &instance,
                        vector<int> *nodes) {
    for (int n = 0; n < int(instance.partitions[p].size()); ++n) {
        if (instance.partitions[p][n] == true) {
            nodes->emplace_back(n);
        }
    }
}

// Compute traffic of partition p.
//      @param[in] p: index of partition
//      @param[in] instance: instance
//      @return traffic
int traffic(const vector<int> &nodes, const Instance &instance) {
    int traffic = 0;
    for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it) {
        auto neighbours = instance.neighbours_of_node.equal_range(*node_it);
        for (auto neighbour_it = neighbours.first;
             neighbour_it != neighbours.second;
             ++neighbour_it)
        {
            int node_i = *node_it;
            int node_j = neighbour_it->second;
            
            if (node_i < node_j) {
                pair<int, int> edge(node_i, node_j);
                int edge_traffic = instance.traffic_of_edge.find(edge)->second;
                traffic += edge_traffic;
            } else {
                pair<int, int> edge(node_j, node_i);
                int edge_traffic = instance.traffic_of_edge.find(edge)->second;
                traffic += edge_traffic;
            }
        }
    }
    return traffic;
}

int inter_partition_traffic(const Instance &instance) {
    int traffic = 0;
    for (size_t i = 0; i < instance.partitions.size(); ++i) {
        for (size_t j = 0; j < i; j++) {
            
        }
    }
    return traffic;
}

// Compute load for each partition.
//      @param[in] instance: base instance
//      @return false if load of any partition is over capacity
bool is_over_capacity(const Instance &instance) {
    for (int p = 0; p < int(instance.partitions.size()); ++p) {
        cout << "Partition " << p << endl;

        vector<int> nodes;
        nodes_in_partition(p, instance, &nodes);
        int partition_traffic = traffic(nodes, instance);
        cout << "    partition traffic: " << partition_traffic << endl;
        if (partition_traffic > instance.max_partition_traffic) {
            return true;
        }
    }
    return false;
}


// Move all nodes from a partition to another partition.
//      @param[in] from: partition index
//      @param[in] to: partition index
//      @param[out] partitions: reference to a 2D membership array.
void move_all_nodes(const int from, const int to,
                vector<vector<bool>> *partitions) {
    assert(to != from);
    assert(to >= 0); assert(to <= int(partitions->size()));
    assert(from >= 0); assert(from <= int(partitions->size()));

    for (size_t node = 0; node < partitions->size(); ++node) {
        (*partitions)[to][node] = (*partitions)[from][node];
        (*partitions)[from][node] = false;
    }
}

// Set bit to true if node in partition.
//      @param[in] node: index of the node
//      @param[in] partition: index of the partition
//      @param[out] partitions: 2D membership array
void set_nodes(const vector<int> nodes, const int partition,
               vector<vector<bool>> *partitions) {
    assert(int(nodes.size()) >= 0);
    assert(partition >= 0); assert(partition <= int(partitions->size()) - 1);

    for (auto node : nodes) {
        assert(node <= int(partitions->size()) - 1);
        (*partitions)[partition][node] = true;
    }
}

// Fix last incoherent partition.
//      @param[out] partitions: reference to a 2D membership array. 
void coherence_step(vector<vector<bool>> *partitions) {
    if (!is_coherent(*partitions)) {
        size_t p = partitions->size() - 1;
        while (min_node_in((*partitions)[p]) == -1 ||
               min_node_in((*partitions)[p]) == int(p)) {
            --p;
        }
        int from = p;
        int to = min_node_in((*partitions)[p]);
        move_all_nodes(from, to, partitions);
    }
}

// TODO: change this into a secondary 'copy' constructor.
// Beware default_random_engine are not const
// Instance(const Instance & instance, default_random_engine & generator)
//
// Create neighbour from base instance.
//      @param[in] instance: base instance
//      @param[out] neighbour: random neighbour
void create_random_neighbour(const Instance &instance,
                             Instance *neighbour) {
}


// TODO: change this into a proper struct constructor.
// Initlize instance with information from file.
//      @param[in/out] instance_file: file (out because reset to beginning)
//      @param[out] instance: initialized instance
void read_instance(ifstream &instance_file, Instance *instance) {
    // First line: n nodes
    instance_file >> instance->node_count;
    // Second line: T (maximum node traffic)
    instance_file >> instance->max_partition_traffic;
    // Third line: m edges
    instance_file >> instance->edge_count;

    // Next m lines: node_i node_j edge traffic (i < j)
    for (int i = 0; i < instance->edge_count; ++i) {
        int node_i, node_j, traffic;
        instance_file >> node_i;
        instance_file >> node_j;
        instance_file >> traffic;
        
        assert(node_i < node_j);
        
        pair<int, int> edge(node_i, node_j);
        pair<int, int> reverse_edge(node_j, node_i);
        
        instance->neighbours_of_node.insert(edge);
        instance->neighbours_of_node.insert(reverse_edge);

        pair<pair<int, int>, int> traffic_of_edge(edge, traffic); 
        instance->traffic_of_edge.insert(traffic_of_edge);
    }
    // Done reading

    // Initialize partitions to empty set 
    for (int i = 0; i < instance->node_count; ++i) {
        vector<bool> p(instance->node_count, false);
        instance->partitions.emplace_back(p);
    }
}

void show_instance(const Instance &instance) {
    string tab = "    ";
    cout << "Instance" << endl;
    cout << tab << "Max traffic: " << instance.max_partition_traffic << endl;
    
    cout << tab << "Node count: " << instance.node_count << endl;
    cout << tab << tab; 
    for (int i = 0; i < instance.node_count; ++i) {
        cout << i << " ";
    }
    cout << endl;

    cout << tab << "Edge count: " << instance.edge_count << endl;
    for (int i = 0; i < instance.node_count; ++i) {
        cout << tab << "Partition " << i << ": ";
        for (int j = 0; j < instance.node_count; ++j) {
                 cout << instance.partitions[i][j] << " ";
        } 
        cout << endl;
    }
    cout << tab << "Coherence ? " 
         << (is_coherent(instance.partitions) ? "yes" : "no")
         << endl;
    cout << tab << "Well formed ? " 
         << (is_well_formed(instance.partitions) ? "yes" : "no")
         << endl;
    cout << tab << "Over capacity ? " 
         << (is_over_capacity(instance) ? "yes" : "no")
         << endl;
}

void mock_initialize_partitions(Instance *test_instance) {
    vector<int> nodes_in_p0 = { 0 };
    vector<int> nodes_in_p2 = { 1, 2 };
    vector<int> nodes_in_p3 = { 3 };

    set_nodes(nodes_in_p0, 0, &test_instance->partitions);
    set_nodes(nodes_in_p2, 2, &test_instance->partitions);
    set_nodes(nodes_in_p3, 3, &test_instance->partitions);
}

void naive_initialize_partitions(Instance *instance) {
    for (int i = 0; i < instance->node_count; ++i) {
        vector<int> node;
        node.emplace_back(i);

        set_nodes(node, i, &instance->partitions);
    }
}

void greedy_initialize_partitions(Instance *instance) {
    int remaining_nodes = instance->node_count;
    int current_node = 0;
    int p = 0;
    while (remaining_nodes > 0) {
        vector<int> nodes;
        int i;
        for (i = current_node; i < instance->node_count; ++i) {
             nodes.emplace_back(i);
             if (traffic(nodes, *instance) > instance->max_partition_traffic) {
                 nodes.pop_back();
                 break;
             }
        }
        set_nodes(nodes, p, &instance->partitions);
        remaining_nodes -= int(nodes.size());
        current_node += int(nodes.size());
        p++;
    }
}
