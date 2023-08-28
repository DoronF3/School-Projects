# Project 3: Reinforcement Learning

## Table of Contents
- [Introduction](#introduction)
- [Q1: Value Iteration](#q1-value-iteration)
- [Q2: Bridge Crossing Analysis](#q2-bridge-crossing-analysis)
- [Q3: Policies](#q3-policies)
- [Q4: Asynchronous Value Iteration](#q4-asynchronous-value-iteration)
- [Q5: Prioritized Sweeping Value Iteration](#q5-prioritized-sweeping-value-iteration)
- [Q6: Q-Learning](#q6-q-learning)
- [Q7: Epsilon Greedy](#q7-epsilon-greedy)
- [Q8: Bridge Crossing Revisited](#q8-bridge-crossing-revisited)
- [Q9: Q-Learning and Pacman](#q9-q-learning-and-pacman)
- [Q10: Approximate Q-Learning](#q10-approximate-q-learning)
- [Submission](#submission)

## Introduction
In this project, you will implement value iteration and Q-learning. You will test your agents first on Gridworld (from class), then apply them to a simulated robot controller (Crawler) and Pacman.
As in previous projects, this project includes an autograder for you to grade your solutions on your machine. This can be run on all questions with the command:
```python
python autograder.py
```

It can be run for one particular question, such as q2, by:
```python
python autograder.py -q q2
```

It can be run for one particular test by commands of the form:
```python
python autograder.py -t test_cases/q2/1-bridge-grid
```


## Files to Edit and Submit
You will fill in portions of the following files during the assignment:
- valueIterationAgents.py: A value iteration agent for solving known MDPs.
- qlearningAgents.py: Q-learning agents for Gridworld, Crawler, and Pacman.
- analysis.py: A file to put your answers to questions given in the project.

Files you should read but NOT edit:
- `mdp.py`: Defines methods on general MDPs.
- `learningAgents.py`: Defines the base classes ValueEstimationAgent and QLearningAgent, which your agents will extend.
- `util.py`: Utilities, including util.Counter, which is particularly useful for Q-learners.
- `gridworld.py`: The Gridworld implementation.
- `featureExtractors.py`: Classes for extracting features on (state, action) pairs. Used for the approximate Q-learning agent.
- ...

## Evaluation
Your code will be autograded for technical correctness. Please do not change the names of any provided functions or classes within the code, or you will wreak havoc on the autograder. However, the correctness of your implementation—not the autograder's judgments—will be the final judge of your score. If necessary, we will review and grade assignments individually to ensure that you receive due credit for your work.

## Academic Dishonesty
We will be checking your code against other submissions in the class for logical redundancy. If you copy someone else's code and submit it with minor changes, we will know. These cheat detectors are quite hard to fool, so please don't try. We trust you all to submit your own work only; please don't let us down. If you do, we will pursue the strongest consequences available to us.

## Getting Help
You are not alone! If you find yourself stuck on something, contact the course staff for help. Office hours, section, and the discussion forum are there for your support; please use them. If you can't make our office hours, let us know, and we will schedule more. We want these projects to be rewarding and instructional, not frustrating and demoralizing. But, we don't know when or how to help unless you ask.

## Discussion
Please be careful not to post spoilers.

## MDPs
To get started, run Gridworld in manual control mode, which uses the arrow keys:
