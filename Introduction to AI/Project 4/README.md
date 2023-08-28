# Table of Contents

- [Introduction](#introduction)
- [Q0: DiscreteDistribution Class](#q0-discretedistribution-class)
- [Q1: Observation Probability](#q1-observation-probability)
- [Q2: Exact Inference Observation](#q2-exact-inference-observation)
- [Q3: Exact Inference with Time Elapse](#q3-exact-inference-with-time-elapse)
- [Q4: Exact Inference Full Test](#q4-exact-inference-full-test)
- [Q5: Approximate Inference Initialization and Beliefs](#q5-approximate-inference-initialization-and-beliefs)
- [Q6: Approximate Inference Observation](#q6-approximate-inference-observation)
- [Q7: Approximate Inference with Time Elapse](#q7-approximate-inference-with-time-elapse)
- [Q8: Joint Particle Filter Initialization](#q8-joint-particle-filter-initialization)
- [Q9: Joint Particle Filter Observation](#q9-joint-particle-filter-observation)
- [Q10: Joint Particle Filter Time Elapse and Full Test](#q10-joint-particle-filter-time-elapse-and-full-test)

## Introduction

Pacman spends his life running from ghosts, but things were not always so. Legend has it that many years ago, Pacman's great grandfather Grandpac learned to hunt ghosts for sport. However, he was blinded by his power and could only track ghosts by their banging and clanging.

In this project, you will design Pacman agents that use sensors to locate and eat invisible ghosts. You'll advance from locating single, stationary ghosts to hunting packs of multiple moving ghosts with ruthless efficiency.

**Files you'll edit:**
- `bustersAgents.py`: Agents for playing the Ghostbusters variant of Pacman.
- `inference.py`: Code for tracking ghosts over time using their sounds.

**Files you will not edit:**
- `busters.py`: The main entry to Ghostbusters (replacing Pacman.py)
- `bustersGhostAgents.py`: New ghost agents for Ghostbusters
- `distanceCalculator.py`: Computes maze distances
- `game.py`: Inner workings and helper classes for Pacman
- `ghostAgents.py`: Agents to control ghosts
- `graphicsDisplay.py`: Graphics for Pacman
- `graphicsUtils.py`: Support for Pacman graphics
- `keyboardAgents.py`: Keyboard interfaces to control Pacman
- `layout.py`: Code for reading layout files and storing their contents
- `util.py`: Utility functions

**Files to Edit:**
You will fill in portions of `bustersAgents.py` and `inference.py` during the assignment. Please do not change the other files in this distribution.

## Ghostbusters and BNs

In the Ghostbusters project, you'll be working with Pacman's ability to sense ghost positions using sonar (ears) that provide noisy distance readings to each ghost. The goal is to locate and eat all the invisible ghosts. The distance readings are affected by noise and provide information about the Manhattan distance to each ghost.

Throughout the project, you'll implement algorithms for exact and approximate inference using Bayes' Nets. The project involves challenges, so it's recommended to start early and seek assistance whenever needed.

## Question 0: DiscreteDistribution Class

In this section, you'll implement the `DiscreteDistribution` class, which is crucial for modeling belief and weight distributions. The class is an extension of Python's built-in dictionary, where keys are discrete elements and values represent the belief or weight assigned to each element.

## Question 1: Observation Probability

Implement the `getObservationProb` method in the `InferenceModule` class. This method calculates the probability of a noisy distance reading given Pacman's position, the ghost's position, and other relevant information. Special cases such as when the ghost is in jail or the distance reading is None must be handled.

**Testing**: To test your implementation, run the following command:
```shell
python3 autograder.py -q q1
```

## Question 2: Exact Inference Observation

Implement the `observeUpdate` method in the `ExactInference` class. This method updates the belief distribution over ghost positions based on observations from Pacman's sensors. The belief at each position on the map is updated after receiving a sensor reading.

## Question 3: Exact Inference with Time Elapse

Implement the `elapseTime` method in the `ExactInference` class. This method updates the belief distribution over ghost positions after one time step elapses. Pacman's knowledge about the ghosts' movement patterns is used to update beliefs over time.

## Question 4: Exact Inference Full Test

Combine the `observeUpdate` and `elapseTime` methods to implement a simple greedy hunting strategy. The `GreedyBustersAgent` assumes ghosts are in their most likely positions and moves toward the closest ghost. Implement the `chooseAction` method to execute this strategy.

## Question 5: Approximate Inference Initialization and Beliefs

Implement the `initializeUniformly` and `getBeliefDistribution` methods in the `ParticleFilter` class. This class tracks a single ghost's position using particles (samples) and converts particle distributions into belief distributions.

## Question 6: Approximate Inference Observation

Implement the `observeUpdate` method in the `ParticleFilter` class. This method calculates particle weights based on observations and resamples particles accordingly. Special cases such as all particles receiving zero weight must be handled.

## Question 7: Approximate Inference with Time Elapse

Implement the `elapseTime` method in the `ParticleFilter` class. This method updates particles based on the ghosts' movement model, considering their valid movements and previous positions.

## Question 8: Joint Particle Filter Observation

Extend the particle filtering algorithm to track multiple ghosts simultaneously. Implement the `initializeUniformly` method in the `JointParticleFilter` class to initialize particles for joint tracking.

## Question 9: Joint Particle Filter Observation

Implement the `observeUpdate` method in the `JointParticleFilter` class. This method performs joint observation updates for all ghost distances, weighting and resampling particles accordingly.

## Question 10: Joint Particle Filter Time Elapse and Full Test

Complete the `elapseTime` method in the `JointParticleFilter` class to perform joint time elapse updates for all ghosts. Implement a greedy strategy that considers the joint particle distributions to minimize the distance to the closest ghost.

