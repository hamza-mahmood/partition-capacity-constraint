#include "traffic.h"

bool is_partition_empty(const Instance &instance, const int p) {
    bool result = true;
    for (size_t i = 0; i < instance.partitions.size(); ++i) {
        if (instance.partitions[p][i] == true) {
            return false;
        }
    }
    return result;
}

pair<int, int> proper_edge(const int i, const int j) {
    pair<int, int> edge;
    if (i == j) {
        cout << "BIG PROBLEM HERE" << endl;
        edge = make_pair(i, j);
    } else if (i < j) {
        edge = make_pair(i, j);
    } else if (i > j) {
        edge = make_pair(j, i);
    }
    return edge;
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
// Compute inbound and outbound traffic of a set of nodes.
//      @param[in] p: partition
//      @param[in] instance: instance
//      @return traffic
int intra_traffic(const int p, const Instance &instance) {
    vector<int> nodes;
    nodes_in_partition(p, instance, &nodes);
    int traffic = 0;
    for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it) {
        auto neighbours = instance.neighbours_of_node.equal_range(*node_it);
        for (auto neighbour_it = neighbours.first;
             neighbour_it != neighbours.second;
             ++neighbour_it)
        {
            int node_i = *node_it; int node_j = neighbour_it->second;

            if (instance.partitions[p][node_j]) {
                pair<int, int> edge = proper_edge(node_i, node_j);
                int edge_traffic = instance.traffic_of_edge.find(edge)->second;
                traffic += edge_traffic;
            }
        }
    }
    return traffic / 2;
}
// Compute inbound and outbound traffic of a set of nodes.
//      @param[in] nodes: set of nodes.
//      @param[in] instance: instance
//      @return traffic
int in_out_node_traffic(const vector<int> &nodes, const Instance &instance) {
    int traffic = 0;
    for (auto node_it = nodes.begin(); node_it != nodes.end(); ++node_it) {
        auto neighbours = instance.neighbours_of_node.equal_range(*node_it);
        for (auto neighbour_it = neighbours.first;
             neighbour_it != neighbours.second;
             ++neighbour_it)
        {
            int node_i = *node_it;
            int node_j = neighbour_it->second;
            
            pair<int, int> edge = proper_edge(node_i, node_j);
            int edge_traffic = instance.traffic_of_edge.find(edge)->second;
            traffic += edge_traffic;
        }
    }
    return traffic;
}

int in_out_traffic(const int p, const Instance &instance) {
    vector<int> nodes;
    nodes_in_partition(p, instance, &nodes);
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
                if (!instance.partitions[p][node_j]) {
                    pair<int, int> edge(node_j, node_i);
                    int edge_traffic = instance.traffic_of_edge.find(edge)->second;
                    traffic += edge_traffic;
                }
            }
        }
    }
    return traffic;
}

// Compute traffic on the entire graph.
//      @param[in] instance: instance containing the graph
int total_traffic(const Instance &instance) {
    int traffic = 0;
    auto begin(instance.traffic_of_edge);
    for (auto edge_it = instance.traffic_of_edge.begin();
         edge_it != instance.traffic_of_edge.end();
         ++edge_it) {
        traffic += edge_it->second;
    }
    cout << "Total traffic: " << traffic << endl;
    return traffic;
}

// Compute intra partition traffic on the entire graph.
//      @param[in] instance: instance containing the graph
int total_intra_traffic(const Instance &instance) {
    int traffic = 0;
    for (int i = 0; i < int(instance.partitions.size()); ++i) {
       traffic += intra_traffic(i, instance); 
    }
    cout << "Total intra traffic: " << traffic << endl;
    return traffic;
}

// Compute inter partition traffic on the entire graph.
//      @param[in] instance: instance containing the graph
int total_inter_traffic(const Instance &instance) {
    return total_traffic(instance)
           - total_intra_traffic(instance);
}

