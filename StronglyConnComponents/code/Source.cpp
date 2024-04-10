#include <algorithm> // sort
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <set>

// Select test without re-typing
const std::string FILEPATH[]
{
    // Path is relative to the project root (not this file's location)
    "assignment-data\\SCC.txt",
    "..\\GraphSln\\sample-data\\test_pairs_2.txt"
};

// Kosaraju requires both directions of graph
std::map<int, std::set<int>*> fwd_graph;
std::map<int, std::set<int>*> rev_graph;

std::map<int, std::set<int>*> leader_groups;

static bool build_graphs()
{
    std::string fn = FILEPATH[1];
    std::ifstream inputFile(fn);
    if (!inputFile)
    {
        std::cerr << "Error opening file." << std::endl;
        return false;
    }
    else
    {
        int firstStr, secondStr;
        while (inputFile >> firstStr >> secondStr)
        {
            int firstNode = int(firstStr);
            int secondNode = int(secondStr);
            // Ensure each graph contains both nodes mentioned
            for (  std::map<int, std::set<int>*>  *graph : { &fwd_graph, &rev_graph })
            {
                for (auto node : { firstNode, secondNode })
                {
                    if (not (*graph).contains(node))
                    {
                        (*graph)[node] = new std::set<int>;
                    }
                }
            }
            // Add connection to both graphs
            fwd_graph[firstNode]->insert(secondNode);
            rev_graph[secondNode]->insert(firstNode);
        }
        inputFile.close();
        return true;
    }
}

std::set<int> nodes_seen{};

int curr_finish_time = 0;
// std::map<int, int> node_finish_times;
std::vector<int> nodes_in_asc_finish_time;

/*
Depth First Search
@param graph - The complete graph
@param node  - The start node
*/
static void DFS_Loop_One(std::map<int, std::set<int>*> &graph, int node)
{
    // Ignore if finished or seen
    if (nodes_seen.contains(node))
    {
        return;
    }

    nodes_seen.insert(node);
    // Find all outgoing nodes
    std::set<int>* outgoing_connections = graph[node];

    for (int connection : *outgoing_connections)
    {
        if (not nodes_seen.contains(connection))
        {
            DFS_Loop_One(graph, connection);
        }
    }
    nodes_in_asc_finish_time.push_back(node);
    return;
}

int s;

static void DFS_Loop_Two(std::map<int, std::set<int>*> &graph, int node)
{
    // Ignore if finished or seen
    if (nodes_seen.contains(node))
    {
        return;
    }

    nodes_seen.insert(node);
    // Find all outgoing nodes
    std::set<int>* outgoing_connections = graph[node];

    for (int connection : *outgoing_connections)
    {
        if (not nodes_seen.contains(connection))
        {
            DFS_Loop_Two(graph, connection);
        }
    }

    if (not leader_groups.contains(s))
    {
        leader_groups[s] = new std::set<int>;
    }
    leader_groups[s]->insert(node);
    return;
}

/*
Open, read and close the File
*/
int main()
{
    if (not build_graphs())
    {
        return 1;
    }
    else
    {
        // First pass
        // Each node in reversed graph
        for (const auto& pair : rev_graph)
        {
            int firstNode = pair.first;
            DFS_Loop_One(rev_graph, firstNode);
        }

        nodes_seen = {}; // Empty

        // Second pass
        // Each node in forward graph
        // In the order specified by the finishing times
        for (auto it = nodes_in_asc_finish_time.rbegin(); it != nodes_in_asc_finish_time.rend(); ++it)
        {
            s = *it;
            DFS_Loop_Two(fwd_graph, s);
        };

        // Count leader group sizes
        std::vector<int> SCC_Sizes;

        for (const auto& [key, value] : leader_groups)
        {
            SCC_Sizes.push_back(value->size());
        }

        std::sort(SCC_Sizes.begin(), SCC_Sizes.end(), std::greater{});

        int numberToPrint = std::min(5, (int)SCC_Sizes.size());


        for (size_t i = 0; i < numberToPrint; i++)
        {

            std::cout << i << " -> " << SCC_Sizes[i] << std::endl;
        }
           

        return 0;
    }
}