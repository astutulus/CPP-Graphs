#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <set>

// Saves re-typing
std::string filename[] {
    "SCC.txt",
    "test_pairs.txt",
    "test_pairs_2.txt"
};

int main() {
    /*
    Map each node with outgoing connection, to nodes reachable
    */
    std::map<int, std::vector<int>*> graph;

    short mostNumberOfConns = 1; // Stats

    /*
    Open, read and close the File
    */
    std::ifstream inputFile(filename[2]);
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }
    int firstInt, secondInt;
    while (inputFile >> firstInt >> secondInt) {
        if (graph.contains(firstInt)) {
            graph[firstInt]->push_back(secondInt);

            // Stats
            short numConns = short(graph[firstInt]->size());
            mostNumberOfConns = std::max(numConns, mostNumberOfConns);
        }
        else {
            std::vector<int>* v = new std::vector<int>{ secondInt };
            graph.insert({ firstInt, v });
        }
    }
    inputFile.close();

    std::cout << "The most outgoing conns is " << mostNumberOfConns << std::endl; // Stats

    //// Access elements
    //for (const auto& t : myVector) {
    //    int intValue = std::get<0>(t);
    //    std::vector<int>* intVectorPtr = std::get<1>(t);

    //    // Print values
    //    std::cout << "Integer value: " << intValue << std::endl;
    //    std::cout << "Vector of integers: ";
    //    for (int val : *intVectorPtr) {
    //        std::cout << val << " ";
    //    }
    //    std::cout << std::endl;

    //    // Clean up memory (don't forget to delete the vector)
    //    delete intVectorPtr;


    /*
    DFS Loop
    */

    // Set of nodes visited
    std::set<int> nodesVisited;
    //a.insert('G');

    return 0;
}
