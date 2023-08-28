# Project 1: Search

## Table of Contents
1. [Introduction](#introduction)
2. [Q1: Depth First Search](#q1-depth-first-search)
3. [Q2: Breadth First Search](#q2-breadth-first-search)
4. [Q3: Uniform Cost Search](#q3-uniform-cost-search)
5. [Q4: A* Search](#q4-a-star-search)
6. [Q5: Corners Problem: Representation](#q5-corners-problem-representation)
7. [Q6: Corners Problem: Heuristic](#q6-corners-problem-heuristic)
8. [Q7: Eating All The Dots: Heuristic](#q7-eating-all-the-dots-heuristic)
9. [Q8: Suboptimal Search](#q8-suboptimal-search)
10. [Submission](#submission)

## Introduction
In this project, you will work on implementing search algorithms to guide Pacman through different scenarios in a maze world. You will develop and apply search algorithms such as Depth First Search, Breadth First Search, Uniform Cost Search, and A* Search to solve various challenges.

## How to Run
- You can test your implementations using the provided Pacman simulation environment by running the `pacman.py` script.
- Use command-line options to specify different layouts and agents, for example:
```python
python pacman.py -l tinyMaze -p SearchAgent -a fn=tinyMazeSearch
```

## Project Structure
- `search.py`: File where your search algorithms will reside.
- `searchAgents.py`: File where your search-based agent implementations will reside.
- `pacman.py`: Main file to run the Pacman game. Defines a Pacman GameState type.
- `game.py`: Logic behind the Pacman world and supporting types like AgentState, Agent, Direction, Grid.
- `util.py`: Data structures for implementing search algorithms.
- Other supporting files: `graphicsDisplay.py`, `graphicsUtils.py`, `textDisplay.py`, `ghostAgents.py`, `keyboardAgents.py`, `layout.py`, `autograder.py`, `testParser.py`, `testClasses.py`, `test_cases/`, `searchTestClasses.py`, and more.

## Question Details

### Q1: Depth First Search
- Implement the depth-first search (DFS) algorithm in `depthFirstSearch` function in `search.py`.
- Test your implementation using different mazes. For example:
  ```python
  python pacman.py -l tinyMaze -p SearchAgent -a fn=tinyMazeSearch
  ```

### Q2: Breadth First Search
- Implement the breadth-first search (BFS) algorithm in `breadthFirstSearch` function in `search.py`.
- Test your implementation on different mazes. For example:
  ```python
  python pacman.py -l mediumMaze -p SearchAgent -a fn=bfs
  ```

### Q3: Uniform Cost Search
- Implement the uniform-cost search (UCS) algorithm in `uniformCostSearch` function in `search.py`.
- Test your implementation on different mazes. For example:
  ```python
  python pacman.py -l bigMaze -z .5 -p SearchAgent -a fn=ucs
  ```

### Q4: A* Search
- Implement A* graph search in the `aStarSearch` function in `search.py`.
- Test your implementation on different mazes. For example:
  ```python
  python pacman.py -l bigMaze -z .5 -p SearchAgent -a fn=astar,heuristic=manhattanHeuristic
  ```

### Q5: Corners Problem: Representation
- Implement the `CornersProblem` search problem in `searchAgents.py`.
- Test your implementation on different mazes. For example:
  ```python
  python pacman.py -l tinyCorners -p SearchAgent -a fn=bfs,prob=CornersProblem
  ```

### Q6: Corners Problem: Heuristic
- Implement a consistent heuristic for the `CornersProblem` in `cornersHeuristic` function.
- Test your heuristic using A* search on different mazes.

### Q7: Eating All The Dots: Heuristic
- Implement a consistent heuristic for the `FoodSearchProblem` in `foodHeuristic` function.
- Test your heuristic using A* search on different mazes.

### Q8: Suboptimal Search
- Implement the `findPathToClosestDot` function in `searchAgents.py`.
- Test your ClosestDotSearchAgent using different mazes.
