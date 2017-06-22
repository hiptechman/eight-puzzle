//
//  main.cpp
//  A*
//
//  Created by Jonathan Appiagyei on 3/7/17.
//  Copyright © 2017 Jonathan Appiagyei. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>

#include "a_star.hpp"
#include "node.hpp"

using namespace std;

namespace a_star {

    void print_state(vector<int> state) {
        for (auto i : state) {
            cout << i << ' ';
        }
        cout << endl;
    }
    
    Solver::Solver(const vector<int> &initial_state, string h_func) {
        if (h_func == "misplaced")
            heuristic = &Solver::heuristic_misplaced;
            else
                heuristic = &Solver::heuristic_manhattan;
        init_state = initial_state;
    }
    
    Solver::~Solver(){};
    
    string Solver::path_to_goal(const Node &start_node) {
        string directions;
        shared_ptr<Node> node(new Node(start_node));
        
        while (node) {
            if (node->_dir_to_here) {
                directions += node->_dir_to_here;
            }
            node = node->_prev_node;
        }
        reverse(directions.begin(), directions.end());
        
        return directions;
    }

    void Solver::generate_neighbors(Node &node){
        vector<int> indices = {-3, -1, 1, 3};
        
        auto blank_iter = find(node._state.begin(), node._state.end(), 0);
        int blank_index = (int)distance(node._state.begin(), blank_iter);
        
        // checks if moving the blank is legal or not
        // creates new neighbor node if legal
        for (auto i : indices) {
            auto new_zero_index = blank_index + i;
            
            if (i == 1 && (new_zero_index == 3 || new_zero_index == 6))
                continue;
            else if (i == -1 && (new_zero_index == 2 || new_zero_index == 5))
                continue;
            else if (new_zero_index < 0 || new_zero_index > 8)
                continue;
            
            vector<int> neighbor_state = node._state;
            swap(neighbor_state[blank_index], neighbor_state[new_zero_index]);
            
            int h_cost = (this->*heuristic)(neighbor_state);
            node._neighbors.push_back(Node(neighbor_state, h_cost));
        }
    }

    int Solver::heuristic_misplaced(const vector<int> &state) {
        vector<int> sorted_state = state;
        int misplaced = 0;
        auto iter = find(sorted_state.begin(), sorted_state.end(), 0);
        
        // removes 0 and adds it to back after sorting
        // avoids 0 being in the front after sorting
        sorted_state.erase(iter);
        sort(sorted_state.begin(), sorted_state.end());
        sorted_state.push_back(0);
        
        for (size_t i = 0; i < sorted_state.size(); ++i) {
            if ((sorted_state[i] != state[i]) && (state[i] != 0))
                ++misplaced;
        }
        return misplaced;
    }

    int Solver::heuristic_manhattan(const vector<int> &state) {
        int sum = 0;
        int state_size = (int)state.size();
        
        for(int i = 0; i < state_size; ++i) {
            if (state[i] == 0)
                continue;
            
            int dx = abs((i % 3) - ((state[i] - 1) % 3));
            int dy = abs((i / 3) - ((state[i] - 1) / 3));
            sum += dx + dy;
        }
        return sum;
    }

    size_t a_star::hash_node::operator()(const vector<int> &state) const {
        return hash<string>()(string(state.begin(), state.end()));
    }

    // question marks just used for easier calculations, not valid directions
    char Solver::find_dir(const vector<int> state1, const vector<int> state2) {
        vector<char> dirs = {'d', '?', 'r', '?', 'l', '?', 'u'};
        
        int s1_blank = (int)distance(state1.begin(), find(state1.begin(),
                                                          state1.end(), 0));
        int s2_blank = (int)distance(state2.begin(), find(state2.begin(),
                                                          state2.end(), 0));
        
        return dirs[s1_blank - s2_blank + 3];
    }

    tuple<string,vector<vector<int>>> Solver::solve() {
        vector<vector<int>> states_visited;
        unordered_set<vector<int>, a_star::hash_node> closed_set;
        unordered_map<vector<int>, int, a_star::hash_node> open_set_map;
        set<Node> open_set;
        
        int curr_prio = (this->*heuristic)(init_state);
        
        Node start_node(init_state, curr_prio);
        
        start_node._g_val = 0;
        start_node._f_val = curr_prio;
        open_set.insert(start_node);
        open_set_map[start_node._state] = start_node._g_val;
        
        while (!open_set_map.empty()) {
            Node curr_node(*open_set.begin());
            
            open_set.erase(open_set.begin());
            open_set_map.erase(curr_node._state);
            
            closed_set.insert(curr_node._state);
            
            states_visited.push_back(curr_node._state);
            
            if (curr_node._h_val == 0)
                return make_tuple(path_to_goal(curr_node), states_visited);
            
            generate_neighbors(curr_node);
            
            // updates cost values of neighbors if they're in the open set
            // creates and addes neighbors to open_set if not in there already
            // ignores neighbors in the closed set
            for (auto neighbor : curr_node._neighbors) {
                if (closed_set.find(neighbor._state) != closed_set.end())
                    continue;
                
                int temp_g_score = curr_node._g_val + 1;
                
                if (open_set_map.find(neighbor._state) != open_set_map.end()) {
                    int old_neighbor_g_val = open_set_map[neighbor._state];
                    
                    if (temp_g_score >= old_neighbor_g_val)
                        continue;
                    else
                        open_set.erase(neighbor);
                }
                else if (temp_g_score > neighbor._g_val)
                    continue;
                
                neighbor.update_cost_vals(temp_g_score);
                neighbor._prev_node =
                std::shared_ptr<Node> (new Node(curr_node));
                neighbor._dir_to_here = find_dir(curr_node._state,
                                                 neighbor._state);
                open_set.insert(neighbor);
                open_set_map[neighbor._state] = neighbor._g_val;
            }
        }
        cout << "No path found" << endl;
        return make_tuple("", states_visited);
    }
}
