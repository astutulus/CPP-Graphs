#include <algorithm> // sort
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <tuple>
#include <map>
#include <set>

#define FILEPATH_NUMBER 1

// And just for fun...
#define report(a) std::cout << "SCC " << a << " has size " << SCC_Sizes[a] << std::endl

// Path is relative to the project root (not this file's location)
const std::string FILEPATH[]
{
    "assignment-data\\SCC.txt",
    "..\\GraphSln\\sample-data\\test_pairs.txt",
    "..\\GraphSln\\sample-data\\test_pairs_2.txt"
};

// Kosaraju requires both directions of graph
std::map<int, std::set<int>*> fwd_graph;
std::map<int, std::set<int>*> rev_graph;

std::map<int, std::set<int>*> leader_groups;

static bool build_graphs()
{
    std::string fn = FILEPATH[FILEPATH_NUMBER];
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

std::stack<int> currReccChain;

int curr_finish_time = 0;
std::stack<int> nodes_in_asc_finish_time{};

/*
Depth First Search
@param graph - The complete graph
@param node  - The start node
*/
static void DFS_Loop_One(std::map<int, std::set<int>*> &graph, int node)
{
    currReccChain.push(node);
    while (not currReccChain.empty())
    {
        int node_peek = currReccChain.top();
        nodes_seen.insert(node_peek);

        std::set<int> * outgoing_connections = graph[node_peek];

        bool foundUnseen = false;
        for (int connection : *outgoing_connections) {
            if (not nodes_seen.contains(connection))
            {
                foundUnseen = true;
                currReccChain.push(connection);
                break;
            }
        }

        if (not foundUnseen) // flag curr node as finished
        {
            currReccChain.pop();
            nodes_in_asc_finish_time.push(node_peek);
        }
    }
}

int s;

static void DFS_Loop_Two(std::map<int, std::set<int>*> &graph, int node)
{
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
        // Start DFS from every node in reversed graph.
        for (const auto& pair : rev_graph)
        {
            int node = pair.first;
            // A node already seen by a previous DFS doesn't need to be explored again.
            if (not nodes_seen.contains(node))
            {
                DFS_Loop_One(rev_graph, node);
            }
        }

        nodes_seen = {}; // Empty

        // Second pass
        // Each node in forward graph
        // In the order specified by the finishing times
        while (not nodes_in_asc_finish_time.empty())
        {
            s = nodes_in_asc_finish_time.top();
            // Ignore if finished or seen
            if (not nodes_seen.contains(s))
            {
                DFS_Loop_Two(fwd_graph, s);
            }
            nodes_in_asc_finish_time.pop();
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
            report(i);
        }
        return 0;
    }
}