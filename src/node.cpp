//
//  node.cpp
//  A*
//
//  Created by Jonathan Appiagyei on 3/15/17.
//  Copyright © 2017 Jonathan Appiagyei. All rights reserved.
//

#include "node.hpp"

Node::Node(std::vector<int> state, int h_val, std::shared_ptr<Node> prev_node,
           char dir_to_here)
  : state(state),
  h_val(h_val),
  prev_node(prev_node),
  dir_to_here(dir_to_here) {}

Node::Node(const Node &n) {
  state = n.state;
  neighbors = n.neighbors;
  h_val = n.h_val;
  g_val = n.g_val;
  f_val = n.f_val;
  prev_node = n.prev_node;
  dir_to_here = n.dir_to_here;
}

void Node::update_cost_vals(const int g_vale) {
  g_val = g_vale;
  f_val = g_val + h_val;
}

Node & Node::operator=(const Node &n) {
  if (this != &n) {
    state = n.state;
    neighbors = n.neighbors;
    h_val = n.h_val;
    g_val = n.g_val;
    f_val = n.f_val;
    prev_node = n.prev_node;
    dir_to_here = n.dir_to_here;
  }
  return *this;
}

Node::~Node(){};

// uses lexicographic state comparison if f costs are equal
bool Node::operator<(const Node &rhs) const {
  const int state_len = 9;
  
  if (f_val == rhs.f_val) {
    for (int i = 0; i < state_len; ++i) {
      if (state[i] != rhs.state[i])
        return state[i] < rhs.state[i];
    }
  }
  return f_val < rhs.f_val;
}

bool Node::operator==(const Node &rhs) const {
  return state == rhs.state;
}
