# Project 2: Multi-Agent Search

## Table of Contents
1. [Introduction](#introduction)
2. [Q1: Reflex Agent](#q1-reflex-agent)
3. [Q2: Minimax](#q2-minimax)
4. [Q3: Alpha-Beta Pruning](#q3-alpha-beta-pruning)
5. [Q4: Expectimax](#q4-expectimax)
6. [Q5: Evaluation Function](#q5-evaluation-function)
7. [Submission](#submission)

## Introduction
In this project, you will design agents for the classic version of Pacman, including ghosts. You will implement the minimax and expectimax search algorithms and work on designing a better evaluation function. Get ready to make Pacman a more capable player!

## Welcome
First, play a game of classic Pacman by running the following command:
```python
python pacman.py
```

Use the arrow keys to move. Now, run the provided ReflexAgent in multiAgents.py:
```python
python pacman.py -p ReflexAgent
```

Note that it plays poorly, so there's room for improvement!

## Question Details

### Q1: Reflex Agent

Improve the `ReflexAgent` in `multiAgents.py` to play respectably. Consider both food and ghost locations to perform well. Test your agent on various layouts, for example:
```python
python pacman.py -p ReflexAgent -l testClassic
```

### Q2: Minimax
Implement the minimax search algorithm in the `MinimaxAgent` class in `multiAgents.py`. Ensure your algorithm is general enough to handle any number of ghosts and arbitrary depth. Test your agent on different layouts and depths, for example:
```python
python pacman.py -p MinimaxAgent -l smallClassic -a depth=3
```

### Q3: Alpha-Beta Pruning
Implement the alpha-beta pruning technique in the `AlphaBetaAgent` class in `multiAgents.py`. Make your agent more efficient at exploring the minimax tree. Test your agent on different layouts and depths, for example:
```python
python pacman.py -p AlphaBetaAgent -l smallClassic -a depth=3
```

### Q4: Expectimax
Implement the `ExpectimaxAgent` class in `multiAgents.py`, which models probabilistic behavior of agents who may make suboptimal choices. Test your agent on different layouts, for example:
```python
python pacman.py -p ExpectimaxAgent -l minimaxClassic -a depth=3
```

### Q5: Evaluation Function
Design a better evaluation function for Pacman in the `betterEvaluationFunction` function in `multiAgents.py`. The function should evaluate states rather than actions. Test your evaluation function on different layouts and game scenarios.
