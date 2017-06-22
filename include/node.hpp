//
//  Node.hpp
//  A*
//
//  Created by Jonathan Appiagyei on 3/15/17.
//  Copyright © 2017 Jonathan Appiagyei. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include <vector>
#include <memory>
#include <limits>

// Node stores state information for 8-puzzle board
class Node {
public:
    std::vector<int> _state;
    std::shared_ptr<Node> _prev_node; // parent node
    std::vector<Node> _neighbors;
    
    char _dir_to_here; // direction blank_space moved to get to this node
    int _g_val = std::numeric_limits<int>::max(); // cost to initial state node
    int _f_val = std::numeric_limits<int>::max(); // sum of _g_val and _f_val
    int _h_val; // cheapest cost from this node to goal node

    Node(std::vector<int> state, int h_val, std::shared_ptr<Node>
         prev_node = nullptr, char dir_to_here = '\0');
    
    Node(const Node &);
    
    Node & operator=(const Node &);
    
    ~Node();
    
    void update_cost_vals(const int g_val);
    bool operator<(const Node &) const;   // used for checking set membership
    bool operator==(const Node &) const;
};


#endif /* Node_h */
