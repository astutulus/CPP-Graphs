#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <set>

int main() {
    /*
    Map of node to nodes reachable
    */
    std::map<int, std::vector<int>*> graph;

    short mostNumberOFConns = 1;

    /*
    Open, read and close the File
    */
    std::ifstream inputFile("SCC.txt");
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int firstInt, secondInt;
    while (inputFile >> firstInt >> secondInt) {
        if (graph.contains(firstInt)) {
            graph[firstInt]->push_back(secondInt);
            // Check quantity of links
            short numConns = graph[firstInt]->size();
            mostNumberOFConns = std::max(numConns, mostNumberOFConns);
        }
        else {
            std::vector<int>* v = new std::vector<int>{ secondInt };
            graph.insert({ firstInt, v });
        }
    }
    inputFile.close();

    std::cout << mostNumberOFConns << std::endl;
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
