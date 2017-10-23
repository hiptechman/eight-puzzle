//
//  main.cpp
//  A*
//
//  Created by Jonathan Appiagyei on 3/7/17.
//  Copyright © 2017 Jonathan Appiagyei. All rights reserved.
//

#include "a_star.hpp"
#include "node.hpp"

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;

namespace a_star {
  
  void print_state(vector<int> &state) {
    for (auto i : state) {
      cout << i << ' ';
    }
    cout << endl;
  }
  
  Solver::Solver(const vector<int> &initial_state, string h_func) {
    if (h_func == "misplaced")
      heuristic = shared_ptr<HeuristicType> (new HeuristicMisplaced());
    else if (h_func == "manhattan")
      heuristic = shared_ptr<HeuristicType> (new HeuristicManhattan());
    else
      throw invalid_argument("Invalid heuristic chosen\n");

    init_state = initial_state;
  }
  
  Solver::~Solver(){};
  
  string Solver::path_to_goal(const Node &start_node) {
    string directions;
    shared_ptr<Node> node(new Node(start_node));
    
    while (node) {
      if (node->dir_to_here) {
        directions += node->dir_to_here;
      }
      node = node->prev_node;
    }
    reverse(directions.begin(), directions.end());
    
    return directions;
  }
  
  void Solver::generate_neighbors(Node &node){
    enum BoardPositions {
      mid_left = 3, bottom_left = 6, top_right = 2, mid_right = 5, top_left = 0,
      bottom_right = 8
    };
    vector<Moves> moves_to_try = {up, down, left, right};
    
    auto blank_iter = find(node.state.begin(), node.state.end(), 0);
    int blank_index = (int)distance(node.state.begin(), blank_iter);
    
    // checks if moving the blank is legal or not
    // creates new neighbor node if legal
    for (auto move : moves_to_try) {
      auto new_index = blank_index + move;
      
      if (move == right && (new_index == mid_left || new_index == bottom_left))
        continue;
      else if (move == left && (new_index == top_right || new_index == mid_right))
        continue;
      else if (new_index < top_left || new_index > bottom_right)
        continue;
      
      vector<int> neighbor_state = node.state;
      swap(neighbor_state[blank_index], neighbor_state[new_index]);
      
      int h_cost = heuristic->compute_cost(neighbor_state);
      node.neighbors.push_back(Node(neighbor_state, h_cost));
    }
  }
  
  HeuristicType::HeuristicType() {};
  HeuristicType::~HeuristicType() {};
  
  HeuristicMisplaced::HeuristicMisplaced() : HeuristicType() {};
  HeuristicMisplaced::~HeuristicMisplaced() {};
  
  int HeuristicMisplaced::compute_cost(const vector<int> &state) {
    int misplaced = 0;

    for (size_t i = 0; i < state.size(); ++i) {
      if (state[i] != i + 1 && state[i] != 0)
        ++misplaced;
    }
    return misplaced;
  }
  
  HeuristicManhattan::HeuristicManhattan() : HeuristicType() {};
  HeuristicManhattan::~HeuristicManhattan() {};
  
  int HeuristicManhattan::compute_cost(const vector<int> &state) {
    int distance = 0;
    int state_size = (int)state.size();
    
    for(int i = 0; i < state_size; ++i) {
      if (state[i] == 0)
        continue;
      
      int dx = abs((i % 3) - ((state[i] - 1) % 3));
      int dy = abs((i / 3) - ((state[i] - 1) / 3));
      distance += dx + dy;
    }
    return distance;
  }
  
  size_t a_star::hash_node::operator()(const vector<int> &state) const {
    return hash<string>()(string(state.begin(), state.end()));
  }
  
  char Solver::find_dir(const vector<int> state1, const vector<int> state2) {
    char dir;
    
    int s1_blank = (int)distance(state1.begin(), find(state1.begin(),
                                                      state1.end(), 0));
    int s2_blank = (int)distance(state2.begin(), find(state2.begin(),
                                                      state2.end(), 0));
    // difference between blank piece indices correspond to a direction
    switch (s2_blank - s1_blank) {
        case up:
          dir = 'u';
          break;
      case down:
        dir = 'd';
        break;
      case left:
        dir = 'l';
        break;
      case right:
        dir = 'r';
        break;
      default:
        dir = '?';
    }
    return dir;
  }
  
  tuple<string,vector<vector<int>>> Solver::solve() {
    vector<vector<int>> states_visited;
    unordered_set<vector<int>, a_star::hash_node> closed_set;
    unordered_map<vector<int>, int, a_star::hash_node> open_set_map;
    set<Node> open_set;
    
    int curr_prio = heuristic->compute_cost(init_state);
    
    Node start_node(init_state, curr_prio);
    
    start_node.g_val = 0;
    start_node.f_val = curr_prio;
    open_set.insert(start_node);
    open_set_map[start_node.state] = start_node.g_val;
    
    while (!open_set_map.empty()) {
      Node curr_node(*open_set.begin());
      
      open_set.erase(open_set.begin());
      open_set_map.erase(curr_node.state);
      
      closed_set.insert(curr_node.state);
      
      states_visited.push_back(curr_node.state);
      
      if (curr_node.h_val == 0)
        return make_tuple(path_to_goal(curr_node), states_visited);
      
      generate_neighbors(curr_node);
      
      // updates cost values of neighbors if they're in the open set
      // creates and addes neighbors to open_set if not in there already
      // ignores neighbors in the closed set
      for (auto neighbor : curr_node.neighbors) {
        if (closed_set.find(neighbor.state) != closed_set.end())
          continue;
        
        int temp_g_score = curr_node.g_val + 1;
        
        if (open_set_map.find(neighbor.state) != open_set_map.end()) {
          int old_neighbor_g_val = open_set_map[neighbor.state];
          
          if (temp_g_score >= old_neighbor_g_val)
            continue;
          else
            open_set.erase(neighbor);
        }
        else if (temp_g_score > neighbor.g_val)
          continue;
        
        neighbor.update_cost_vals(temp_g_score);
        neighbor.prev_node =
        std::shared_ptr<Node> (new Node(curr_node));
        neighbor.dir_to_here = find_dir(curr_node.state,
                                         neighbor.state);
        open_set.insert(neighbor);
        open_set_map[neighbor.state] = neighbor.g_val;
      }
    }
    cout << "No path found" << endl;
    return make_tuple("", states_visited);
  }
}
