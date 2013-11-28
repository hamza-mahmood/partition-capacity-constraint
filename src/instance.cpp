#include "instance.h"

// Return first node of a partition.
//      @param[in] partition: reference to a 1D membership array.
//      @return first node
int min_node_in(const vector<bool> &partition) {
    // returns first node in partition
    for (size_t i = 0; i < partition.size(); i++) {
        if (partition[i] == true)
            return int(i);
    }
    // if empty partition return -1
    return -1;
}

// Return true if partitions are labeled after their smallest element.
//      @param[in] partitions: reference to a 2D membership array.
//      @return coherence
bool is_coherent(const vector<vector<bool>> &partitions) {
    bool result = true;
    for (size_t p = 0; p < partitions.size(); p++) {
        if (min_node_in(partitions[p]) == -1)
            result &= true;
        else
            result &= (min_node_in(partitions[p]) == int(p));
    }
    return result;
}

// TODO: figure out these friggin' types...
// Compute load for each partition.
//      @param[in] instance: base instance
//      @return false if load of any partition is over capacity
bool is_over_capacity(const Instance &instance) {
    //for each partition
      //  traffic in+out partition <= T
    for (int p = 0; p < instance.node_count; ++p) {
        int partition_capacity = 0;
        
        //pair<multimap<const int, pair<int, int>>::iterator,
          //   multimap<const int, pair<int, int>>::iterator> edge_range;
        auto edge_range = instance.edges_of_partition.equal_range(p);
        
        //multimap<const int, pair<int, int>>::iterator first_edge;
        auto first_edge = edge_range.first;
        //multimap<const int, pair<int, int>>::iterator last_edge;
        auto last_edge = edge_range.second;
        //multimap<const int, pair<int, int>>::iterator edge_it;

        for (auto edge_it = first_edge; edge_it != last_edge; ++edge_it) {
            //partition_capacity += instance.traffic_of_edge[(*edge_it).second];
            auto edge = edge_it->second;
            partition_capacity += instance.traffic_of_edge.find(edge)->second;
            //cout << "Partial capacity: " << partition_capacity << endl;
        }
        //cout << "Total capacity: " << partition_capacity << endl;
        //cout << "Max capacity: " << instance.max_partition_traffic << endl;

        if (partition_capacity > instance.max_partition_traffic) {
            return true;
        }
    }
    return false;
}

// Move all nodes from a partition to another partition.
//      @param[in] from: partition index
//      @param[in] to: partition index
//      @param[out] partitions: reference to a 2D membership array.
void move_nodes(const int from, const int to,
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

// Find edges that have one extremity in a partition.
//      @param[in] edges: for each node in partition, edges starting with node
//      @param[in] p: partition index
//      @param[out] edges_of_partition: partition, edges with one extremity in
void set_edges(const vector<pair<int, int>> &edges, const int partition,
               multimap<const int, pair<int, int>> *edges_of_partition) {
    assert(int(edges.size()) >= 0);
    assert(partition >= 0); 

    for (auto edge : edges) {
        edges_of_partition->insert(make_pair(partition, edge));
    }
}

// Initialize partition with given nodes.
void set_partition() {
    // for every edge in all_edges and every rev_edge in all_rev_edges
    //      does edge or rev_edge have an extremity in partition
    //          if so, add it to edges
    // set_edges(edges)
    /*
    for (auto node : nodes) {
        for (auto edge : edges) {
            
        }
    }
    */
}


// Fix first incoherent partition.
//      @param[out] partitions: reference to a 2D membership array. 
void coherence_step(vector<vector<bool>> *partitions) {
    if (!is_coherent(*partitions)) {
        size_t p = 0;
        while (min_node_in((*partitions)[p]) == -1 ||
               min_node_in((*partitions)[p]) == int(p)) {
            ++p;
        }
        int from = p;
        int to = min_node_in((*partitions)[p]);
        move_nodes(from, to, partitions);
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
        
        pair<int, pair<int, int>> index_edge(i, edge);
        pair<int, pair<int, int>> index_reverse_edge(i, reverse_edge);
        
        instance->edges.insert(index_edge);
        instance->reverse_edges.insert(index_reverse_edge);

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
    cout << tab << tab;
    for (auto it = instance.edges.begin(); it != instance.edges.end(); ++it) {
        pair<int, int> edge = it->second;
        cout << edge.first << "-" << edge.second << " ";
    }
    cout << endl;

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
