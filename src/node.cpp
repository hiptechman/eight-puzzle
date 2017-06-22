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
    : _state(state),
    _h_val(h_val),
    _prev_node(prev_node),
    _dir_to_here(dir_to_here) {}

Node::Node(const Node &n) {
    _state = n._state;
    _neighbors = n._neighbors;
    _h_val = n._h_val;
    _g_val = n._g_val;
    _f_val = n._f_val;
    _prev_node = n._prev_node;
    _dir_to_here = n._dir_to_here;
}

void Node::update_cost_vals(const int g_val) {
    _g_val = g_val;
    _f_val = _g_val + _h_val;
}

Node & Node::operator=(const Node &n) {
    if (this != &n) {
        _state = n._state;
        _neighbors = n._neighbors;
        _h_val = n._h_val;
        _g_val = n._g_val;
        _f_val = n._f_val;
        _prev_node = n._prev_node;
        _dir_to_here = n._dir_to_here;
    }
    return *this;
}

Node::~Node(){};

// uses lexicographic state comparison if f costs are equal
bool Node::operator<(const Node &rhs) const {
    if (_f_val == rhs._f_val) {
        for (int i = 0; i < 9; ++i) {
            if (_state[i] != rhs._state[i])
                return _state[i] < rhs._state[i] ? 1 : 0;
        }
    }
    return _f_val < rhs._f_val;
}

bool Node::operator==(const Node &rhs) const {
    return _state == rhs._state;
}
