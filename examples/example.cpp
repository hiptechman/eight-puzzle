#include "a_star.hpp"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    vector<int> init_state = {8, 4, 0, 6, 2, 5, 7, 3, 1};
  
  try {
    cout << get<0>(a_star::Solver(init_state, "misplaced").solve()) << endl;
  }
  catch(exception &e) {
    cerr << e.what();
    return EXIT_FAILURE;
  }
    return 0;
}
