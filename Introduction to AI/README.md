# Introduction to AI projects

This directory contains a collection of exercises related to introduction to AI course projects. Each exercise focuses on different concepts and challenges in the field of operating systems.

## Project 1: Search

- **Description**: In this project, you will work on implementing search algorithms to guide Pacman through different scenarios in a maze world. You will develop and apply search algorithms such as Depth First Search, Breadth First Search, Uniform Cost Search, and A* Search to solve various challenges.
- **Key Files**:
  - `search.py`: File where your search algorithms will reside.
  - `searchAgents.py`: File where your search-based agent implementations will reside.
  - `pacman.py`: Main file to run the Pacman game. Defines a Pacman GameState type.
  - `game.py`: Logic behind the Pacman world and supporting types like AgentState, Agent, Direction, Grid.
  - `util.py`: Data structures for implementing search algorithms.

## Project 2: Multi-Agent Search

- **Description**: In this project, you will design agents for the classic version of Pacman, including ghosts. You will implement the minimax and expectimax search algorithms and work on designing a better evaluation function to enhance Pacman's performance.
- **Key Files**:
  - `bustersAgents.py`: Agents for playing the Ghostbusters variant of Pacman.
  - `inference.py`: Code for tracking ghosts over time using their sounds.

## Project 3: Reinforcement Learning
- **Description**: In this project, you will implement value iteration and Q-learning algorithms. You will test your agents on various scenarios, including Gridworld, a simulated robot controller (Crawler), and Pacman.
- **Key Files**:
  - `valueIterationAgents.py`: A value iteration agent for solving known MDPs.
  - `qlearningAgents.py`: Q-learning agents for Gridworld, Crawler, and Pacman.
  - `analysis.py`: File to put your answers to questions given in the project.

## Project 4: Ghostbusters
- **Description**: In this project, you'll work with Pacman's ability to sense ghost positions using sonar. Your task is to locate and eat invisible ghosts by implementing algorithms for exact and approximate inference using Bayes' Nets.
- **Key Files**:
  - `bustersAgents.py`: Agents for playing the Ghostbusters variant of Pacman.
  - `inference.py`: Code for tracking ghosts over time using their sounds.
