#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <set>

// Saves re-typing
std::string filename[]
{
    "SCC.txt",
    "test_pairs.txt",
    "test_pairs_2.txt"
};

int curr_finish_time = 0;

std::set<int> all_nodes;
std::set<int> nodes_currently_working_on;
std::set<int> nodes_finished;

std::map<int, int> node_finish_times;

std::map<int, std::set<int>*> fwd_graph;
std::map<int, std::set<int>*> rev_graph;


/*
Depth First Search
@param graph The complete graph
@param node  The start node
*/
void DFS_Loop(std::map<int, std::set<int>*> &graph, int node) 
{
    // Ignore if finished or seen
    if (nodes_finished.count(node) or nodes_currently_working_on.count(node)) {
        return;
    }

    // Finish if it's a dead end
    if (graph.find(node) == graph.end()) {
        node_finish_times[node] = curr_finish_time--;
        return;
    }

    // Find all outgoing nodes
    auto outgoing_connections = *graph[node];

    // Visit all outgoing nodes
    for (auto conn : outgoing_connections) {
        if (node_finish_times[conn] == 0 and (conn)) {
            DFS_Loop(graph, conn);
        }
        else {
            node_finish_times[conn] = curr_finish_time--;
        }
    }

    nodes_currently_working_on.insert(node);
    // If it has no outgoing nodes it must be an SCC by itself

}

/*
Open, read and close the File
*/
int main()
{
    std::ifstream inputFile(filename[2]);

    if (!inputFile)
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    int firstInt, secondInt;

    while (inputFile >> firstInt >> secondInt)
    {
        // Make a definitive set of nodes
        all_nodes.insert(firstInt);
        all_nodes.insert(secondInt);

        // Add outgoing connection to forward graph
        if (fwd_graph.contains(firstInt))
        {
            fwd_graph[firstInt]->push_back(secondInt);
        }
        else
        {
            std::vector<int>* v = new std::vector<int>{ secondInt };
            fwd_graph.insert({ firstInt, v });
        }

        // Add incoming connection to reversed graph
        if (rev_graph.contains(secondInt))
        {
            rev_graph[secondInt]->push_back(firstInt);
        }
        else
        {
            std::vector<int>* v = new std::vector<int>{ firstInt };
            rev_graph.insert({ secondInt, v });
        }
    }
    inputFile.close();

    // Set all nodes to search
    for (int n : all_nodes)
    {
        node_finish_times.insert({ n, 0 });
    }

    curr_finish_time = all_nodes.size();

    //
    for (int node: all_nodes)
    {
        DFS_Loop(rev_graph, node);
    }

    // Print
    for (auto node : node_finish_times)
    {
        std::cout << node.first << " -> " << node.second << std::endl;
    }

    return 0;
}