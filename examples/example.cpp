#include "a_star.hpp"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    vector<int> init_state = {7, 5, 8, 2, 4, 6, 3, 1, 0};
    
    cout << get<0>(a_star::Solver(init_state, "misplaced").solve());
    
    return 0;
}
