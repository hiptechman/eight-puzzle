# eight-puzzle

This program is designed to solve a 3x3 [8-puzzle](https://www.cs.princeton.edu/courses/archive/spr08/cos226/assignments/8puzzle.html). This is achieved by using the [A*](https://en.wikipedia.org/wiki/A*_search_algorithm) algorithm to find the minimal number of moves needed to arrive at the goal state. The current implementation allows you to select between two heuristics, [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) and [misplaced tiles](https://heuristicswiki.wikispaces.com/Misplaced+Tiles.).

Below is a sample problem:

__Initial State__:

<table>
<tr><td>8</td><td>4</td><td></td></tr>
<tr><td>6</td><td>2</td><td>5</td></tr>
<tr><td>7</td><td>3</td><td>1</td></tr>
</table>

__Goal State__:

<table>
<tr><td>1</td><td>2</td><td>3</td></tr>
<tr><td>4</td><td>5</td><td>6</td></tr>
<tr><td>7</td><td>8</td><td> </td></tr>
</table>

## Getting Started

Once you've cloned the repository, navigate to the `example` directory. Open `example.cpp` and enter in your initial state. Enter in which heuristic ("misplaced" or "manhattan") you'd like to use. Only nine values (0-9) can be entered with the number `0` representing the blank space. 

### Running

Compile and run the code:

 `make`

`./example`

The output represents which directions, in order, the blank space needs to be moved to arrive at the goal state. For the above sample problem, this program would output `dllurrddluruldlurrddluurdd` as the solution. 

**Note:** Some puzzles are impossible to be solved. For such puzzles, `No path found` will be returned. 
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

