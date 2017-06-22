//
//  A*.hpp
//  A*
//
//  Created by Jonathan Appiagyei on 3/15/17.
//  Copyright © 2017 Jonathan Appiagyei. All rights reserved.
//

#ifndef A__h
#define A__h

#include <string>
#include <tuple>
#include <vector>

#include "node.hpp"

namespace a_star {
    // only used by unordered_map and unordered_set
    struct hash_node {
        size_t operator()(const std::vector<int> &state) const;
    };

    void print_state(std::vector<int> state);

    class Solver {
        typedef int (Solver::*heuristic_func)(const std::vector<int> &state);
        heuristic_func heuristic;
        std::vector<int> init_state;
        
        // returns directions from initial state to goal state
        // directions refer to movement of empty space
        // 'l' = left, 'r' = right, 'u' = up, 'd'= down
        std::string path_to_goal(const Node &node);
        
        // adds neighboring states of @param node to node's "neighbors" vector
        void generate_neighbors(Node &node);
        
        // returns number of out of place tiles
        int heuristic_misplaced(const std::vector<int> &state);
        
        // calculates sum of manhattan distances of individual tiles to their
        // correct spots
        int heuristic_manhattan(const std::vector<int> &state);
        
        // finds direction needed to get from state1 to state2
        char find_dir(const std::vector<int> state1, const std::vector<int> state2);
    public:

        // calculates shortest path from initial state to goal state
        std::tuple<std::string, std::vector<std::vector<int>>>solve();
        Solver(const std::vector<int> &init_state, std::string h_func);
        
        ~Solver();
    };
}

#endif /* A__h */
