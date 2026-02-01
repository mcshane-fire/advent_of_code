#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <regex>
#include <sstream>

void add_edge_counts(std::map<int,int>& found, std::map<std::pair<int,int>,int>& edges, int node)
{
    while(node != -1) {
        std::pair<int,int> edge(std::min(node,found[node]), std::max(node,found[node]));
        if(!edges.contains(edge)) {
            edges[edge] = 1;
        } else {
            edges[edge]++;
        }
        node = found[node];
    }
}

int flood_routes(std::map<int,std::set<int>>& connections, std::map<std::pair<int,int>,int>& edges, int node) {
    std::map<int,int> found;
    std::map<int,std::vector<std::pair<int,int>>> explore;
    explore[0] = std::vector<std::pair<int,int>>();
    explore[0].emplace_back(-1, node);

    for(auto it = explore.begin(); it != explore.end(); it++) {
        for(auto n : (*it).second) {
            if(!found.contains(n.second)) {
                found[n.second] = n.first;
                //std::cout << " - found route to " << n.second << " via " << n.first << " in " << (*it).first << "\n";
                add_edge_counts(found, edges, n.second);

                for(auto &s : connections[n.second]) {
                    if(!found.contains(s)) {
                        if(!explore.contains((*it).first+1)) {
                            explore[(*it).first+1] = std::vector<std::pair<int,int>>();
                        }
                        explore[(*it).first+1].emplace_back(n.second, s);
                    }
                }
            }
        }
    }
    return found.size();
}

int find_all_shortest_paths(std::map<int,std::set<int>>& connections, std::vector<std::string> node_names) {
    std::map<std::pair<int,int>,int> edges;

    for(auto& p : connections) {
        //std::cout << "Starting from " << p.first << "\n";
        flood_routes(connections, edges, p.first);
    }

    std::map<int,std::vector<std::pair<int,int>>> redges;
    for(auto &n : edges) {
        if(!redges.contains(n.second)) {
            redges[n.second] = std::vector<std::pair<int,int>>();
        }
        redges[n.second].push_back(n.first);
    }

    // remove most popular three edges
    auto rit = redges.rbegin();
    std::vector<std::pair<int,int>> removed;
    for(int i=0; i<3; i++) {
        auto p = (*rit).second.back();
        (*rit).second.pop_back();
        if((*rit).second.size() == 0) {
            rit++;
        }
        connections[p.first].erase(p.second);
        connections[p.second].erase(p.first);
        removed.push_back({p.first,p.second});
        //std::cout << " removed " << node_names[p.first] << " - " << node_names[p.second] << "\n";
    }

    int parta = flood_routes(connections, edges, removed[0].first);
    int partb = flood_routes(connections, edges, removed[0].second);

    if(parta == partb && parta == connections.size()) {
        std::cout << " (failed) ";
    }

    return parta * partb;
}

int get_node_number(std::vector<std::string>& node_names, const std::string& node) {
    auto it = std::find(node_names.begin(), node_names.end(), node);
    int num;
    if(it == node_names.end()) {
        num = node_names.size();
        node_names.push_back(node);
    } else {
        num = it - node_names.begin();
    }
    return num;
}

int main(int argc, char *argv[]) {
    std::ifstream input(argc >= 2 ? argv[1] : "test_input.txt");
    std::string line;
    std::regex pat = std::regex("^([a-z]*): (.*)$");
    std::smatch res;
    std::map<int,std::set<int>> connections;
    std::vector<std::string> node_names;

    while(std::getline(input, line)) {
        if(std::regex_match(line, res, pat)) {
            int src_num = get_node_number(node_names, std::string(res[1]));

            if(!connections.contains(src_num)) {
                connections[src_num] = std::set<int>();         
            }
            std::stringstream ss(res[2]);
            std::string comp;
            while(getline(ss, comp, ' ')) {
                int dest_num = get_node_number(node_names, comp);
                connections[src_num].insert(dest_num);
                if(!connections.contains(dest_num)) {
                    connections[dest_num] = std::set<int>();
                }
                connections[dest_num].insert(src_num);
            }
        }
    }

    std::cout << "Part1: " << find_all_shortest_paths(connections, node_names) << "\n";

    return 0;
}