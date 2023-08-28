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

## Files to Edit
You will fill in portions of the following files during the assignment:
- `valueIterationAgents.py`: A value iteration agent for solving known MDPs.
- `qlearningAgents.py`: Q-learning agents for Gridworld, Crawler, and Pacman.
- `analysis.py`: A file to put your answers to questions given in the project.

Files you should read but NOT edit:
- `mdp.py`: Defines methods on general MDPs.
- `learningAgents.py`: Defines the base classes ValueEstimationAgent and QLearningAgent, which your agents will extend.
- `util.py`: Utilities, including util.Counter, which is particularly useful for Q-learners.
- `gridworld.py`: The Gridworld implementation.
- `featureExtractors.py`: Classes for extracting features on (state, action) pairs. Used for the approximate Q-learning agent.

Files you can ignore:
- `environment.py`: Abstract class for general reinforcement learning environments. Used by gridworld.py .
- `graphicsGridworldDisplay.py`: Gridworld graphical display.
- `graphicsUtils.py`: Graphics utilities.
- `textGridworldDisplay.py`: Plug-in for the Gridworld text interface.
- `crawler.py`: The crawler code and test harness. You will run this but not edit it.
- `graphicsCrawlerDisplay.py`: GUI for the crawler robot.
- `autograder.py`: Project autograder
- `testParser.py`: Parses autograder test and solution files
- `testClasses.py`: General autograding test classes
- `test_cases/`: Directory containing the test cases for each question
- `reinforcementTestClasses.py`: Project 3 specific autograding test classes

## MDPs
To get started, run Gridworld in manual control mode, which uses the arrow keys:
```python
python gridworld.py -m
```

You will see the two-exit layout from class. The blue dot is the agent. Note that when you press up, the agent only actually moves north 80% of the time. Such is the life of a Gridworld agent!
You can control many aspects of the simulation. A full list of options is available by running:
```python
python gridworld.py -h
```


### Question 1: Value Iteration
In this question, you'll implement the Value Iteration algorithm. Your task is to complete the `ValueIterationAgent` class, which performs offline planning using the value iteration algorithm. This agent will compute k-step estimates of optimal state values and synthesize policies based on these values. Be sure to follow the guidelines provided in the project specifications to correctly implement the required methods.

To test your implementation, you can run the autograder using the following command:

```python
python autograder.py -q q1
```

### Question 2: Bridge Crossing Analysis
In this question, you'll explore a grid world map called BridgeGrid and manipulate its parameters to influence the agent's behavior. Your task is to change either the discount factor or the noise parameter to make the agent prefer crossing the bridge, taking risks if necessary. Implement your solution in the question2 function in the analysis.py file.

To check your answer, run the autograder:

```python
python autograder.py -q q2
```

### Question 3: Policies
In this question, you'll work with different optimal policy scenarios on a grid layout called DiscountGrid. You'll adjust the discount, noise, and living reward parameters to achieve specific policy behaviors. Your task is to find parameter settings that produce the desired policy types listed in the project specifications. Implement your solutions in the question3a through question3e functions in the analysis.py file.

To check your answers, run the autograder:

```python
python autograder.py -q q3
```

### Question 4: Asynchronous Value Iteration
In this question, you'll implement a variant of the value iteration algorithm called asynchronous value iteration. You'll update one state's value in each iteration instead of doing a batch-style update. Your task is to complete the AsynchronousValueIterationAgent class to perform value iteration using this approach.

To test your implementation, run the autograder:

```python
python autograder.py -q q4
```

### Question 5: Prioritized Sweeping Value Iteration
In this question, you'll implement the prioritized sweeping algorithm for value iteration. Your task is to complete the PrioritizedSweepingValueIterationAgent class to perform prioritized updates of state values based on their priorities. Follow the provided algorithm to correctly implement the agent.

To test your implementation, run the autograder:

```python
python autograder.py -q q5
```

### Question 6: Q-Learning
In this question, you'll implement the Q-learning algorithm, which allows the agent to learn by trial and error from interactions with the environment. Your task is to complete the QLearningAgent class, implementing methods for updating Q-values and choosing actions based on the learned values. Be sure to follow the provided algorithm and guidelines for proper implementation.

To test your implementation, run the autograder:

```python
python autograder.py -q q6
```

### Question 7: Epsilon Greedy
In this question, you'll implement epsilon-greedy action selection for the Q-learning agent. Your task is to modify the getAction method in the QLearningAgent class to choose random actions an epsilon fraction of the time and follow the best Q-values otherwise.

To test your implementation, run the autograder:

```python
python autograder.py -q q7
```

Question 8: Bridge Crossing Revisited
In this question, you'll explore training a Q-learning agent on the BridgeGrid. Your task is to find an epsilon and a learning rate that allow the agent to reliably learn the optimal policy after a specific number of episodes. Implement your solution in the question8 function in the analysis.py file.

To check your answer, run the autograder:

css
Copy code
python autograder.py -q q8
Question 9: Q-Learning and Pacman
In this question, you'll apply Q-learning to the Pacman problem. Your task is to train a Q-learning agent to play Pacman effectively using the provided PacmanQAgent. You'll need to fine-tune learning parameters to achieve a high win rate in test games.

To test your implementation, run the autograder:

css
Copy code
python autograder.py -q q9
Question 10: Approximate Q-Learning
In this question, you'll implement an approximate Q-learning agent that uses feature-based representation of states. Your task is to complete the ApproximateQAgent class, which learns weights for features of states. You'll implement methods to update feature weights and choose actions based on these weights.

To test your implementation, run the autograder:

css
Copy code
python autograder.py -q q10
