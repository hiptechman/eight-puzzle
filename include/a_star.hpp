//
//  a_star.hpp
//
//  Created by Jonathan Appiagyei on 3/15/17.
//  Copyright © 2017 Jonathan Appiagyei. All rights reserved.
//

#ifndef A_star_h
#define A_star_h

#include "node.hpp"

#include <string>
#include <tuple>
#include <vector>

namespace a_star {
  class HeuristicType;

  // hash function for unordered_map and unordered_set
  struct hash_node {
    size_t operator()(const std::vector<int> &state) const;
  };
  
  void print_state(std::vector<int> state);
  
  
  class Solver {
  public:
    Solver(const std::vector<int> &init_state, std::string h_func);
    
    // calculates shortest path from initial state to goal state
    std::tuple<std::string, std::vector<std::vector<int>>>solve();
    ~Solver();

  private:
//    typedef int (Solver::*heuristic_func)(const std::vector<int> &state);
  
    // returns directions from initial state to goal state
    // directions refer to movement of empty space
    // 'l' = left, 'r' = right, 'u' = up, 'd'= down
    std::string path_to_goal(const Node &node);
    
//    void set_heuristic(HeuristicType *heuristic);
    // adds neighboring states of @param node to node's "neighbors" vector
    void generate_neighbors(Node &node);
  
    // finds direction needed to get from state1 to state2
    char find_dir(const std::vector<int> state1, const std::vector<int> state2);
//    heuristic_func heuristic;
    std::vector<int> init_state;
    
    std::shared_ptr<HeuristicType> heuristic;
  };
  
  // base class for 8-puzzle heuristics
  class HeuristicType {
  public:
    HeuristicType();
    virtual int compute_cost(const std::vector<int> &state) = 0;
    virtual ~HeuristicType();
  };
  
  // heuristic calculates sum of manhattan distances of individual tiles to
  // their correct spots
  class HeuristicManhattan : public HeuristicType {
  public:
    HeuristicManhattan();
    int compute_cost(const std::vector<int> &state) override;
    ~HeuristicManhattan();
  };
  
  // heuristic returns number of out of place tiles
  class HeuristicMisplaced : public HeuristicType {
  public:
    HeuristicMisplaced();
    int compute_cost(const std::vector<int> &state) override;
    ~HeuristicMisplaced();
  };
}

#endif /* A_star_h */
