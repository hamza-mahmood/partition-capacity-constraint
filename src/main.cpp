#include <iostream>
#include <fstream>
#include <vector>
#include "instance.h"
#include "heuristic.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0]
             << " <file name>"
             << endl;
    } else {
        ifstream instance_file;
        instance_file.open(argv[1]);

        if(instance_file.is_open()) {
            Instance instance;
            // out argument: always a pointer so always &something
            // spot side-effects by /&
            read_instance(instance_file, &instance);
            naive_initialize_partitions(&instance);
            cout << "NAIVE INIT" << endl;
            show_instance(instance);

            /*
            instance_file.seekg(0, instance_file.beg);
           
            Instance test_instance;
            read_instance(instance_file, &test_instance);
            mock_initialize_partitions(&test_instance);
            cout << "MANUAL INIT" << endl;
            //show_instance(test_instance);
            while (!is_coherent(test_instance.partitions)) {
                coherence_step(&test_instance.partitions);
                //show_instance(test_instance);
            }
            show_instance(test_instance);
            */
            instance_file.seekg(0, instance_file.beg);

            Instance test_instance2;
            read_instance(instance_file, &test_instance2);
            greedy_initialize_partitions(&test_instance2);
            cout << "GREEDY INIT" << endl;
            show_instance(test_instance2);
            cout << "FIRST IMPROVING" << endl;
            giving_first_improving(test_instance2);
            cout << "BEST IMPROVING" << endl;
            giving_best_improving(test_instance2);

        } 
    }
    return 0;
}
