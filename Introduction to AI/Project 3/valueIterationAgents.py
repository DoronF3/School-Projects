# valueIterationAgents.py
# -----------------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


# valueIterationAgents.py
# -----------------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).from queue import priorityQueue

import mdp, util

from learningAgents import ValueEstimationAgent
import collections

class ValueIterationAgent(ValueEstimationAgent):
    """
        * Please read learningAgents.py before reading this.*

        A ValueIterationAgent takes a Markov decision process
        (see mdp.py) on initialization and runs value iteration
        for a given number of iterations using the supplied
        discount factor.
    """
    def __init__(self, mdp, discount = 0.9, iterations = 100):
        """
          Your value iteration agent should take an mdp on
          construction, run the indicated number of iterations
          and then act according to the resulting policy.

          Some useful mdp methods you will use:
              mdp.getStates()
              mdp.getPossibleActions(state)
              mdp.getTransitionStatesAndProbs(state, action)
              mdp.getReward(state, action, nextState)
              mdp.isTerminal(state)
        """
        self.mdp = mdp
        self.discount = discount
        self.iterations = iterations
        self.values = util.Counter()  # A Counter is a dict with default 0
        self.runValueIteration()

    def runValueIteration(self):
        # Write value iteration code here
        "*** YOUR CODE HERE ***"
        for i in range(self.iterations):
            new_values = self.values.copy()  # Copy the values.
            for state in self.mdp.getStates():  # go through every state.
                best_action = self.computeActionFromValues(state)  # Get the best action.
                if best_action is None:  # If its terminal state.
                    continue
                new_values[state] = self.getQValue(state, best_action)  # Assign values to states for next iteration.
            self.values = new_values  # When iterated over all values assign.


    def getValue(self, state):
        """
          Return the value of the state (computed in __init__).
        """
        return self.values[state]


    def computeQValueFromValues(self, state, action):
        """
          Compute the Q-value of action in state from the
          value function stored in self.values.
        """
        "*** YOUR CODE HERE ***"
        total = 0  # Initialize value.
        transitions = self.mdp.getTransitionStatesAndProbs(state, action)  # Get transition function.
        for transition in transitions:  # go over all possible next states.
            next_state, prob = transition  # Get the next state and its probability.
            reward = self.mdp.getReward(state, action, next_state)  # Calculate the reward for next state.
            total += prob*(reward + (self.discount * self.getValue(next_state)))  # Add to the total.
        return total

    def computeActionFromValues(self, state):
        """
          The policy is the best action in the given state
          according to the values currently stored in self.values.

          You may break ties any way you see fit.  Note that if
          there are no legal actions, which is the case at the
          terminal state, you should return None.
        """
        "*** YOUR CODE HERE ***"
        actions = self.mdp.getPossibleActions(state)  # Get a list of all possible actions.
        if self.mdp.isTerminal(state) or len(self.mdp.getPossibleActions(state)) == 0:  # No legal actions.
            return None
        best_ac = ""  # Initialize value.
        max_val = -float("inf")  # Initialize value.
        for action in actions:
            q_val = self.getQValue(state, action)  # Get Q-value.
            if q_val > max_val:  # If the value is bigger.
                max_val = q_val  # Change max value.
                best_ac = action  # Change best action.
        return best_ac


    def getPolicy(self, state):
        return self.computeActionFromValues(state)

    def getAction(self, state):
        "Returns the policy at the state (no exploration)."
        return self.computeActionFromValues(state)

    def getQValue(self, state, action):
        return self.computeQValueFromValues(state, action)


class AsynchronousValueIterationAgent(ValueIterationAgent):
    """
        * Please read learningAgents.py before reading this.*

        An AsynchronousValueIterationAgent takes a Markov decision process
        (see mdp.py) on initialization and runs cyclic value iteration
        for a given number of iterations using the supplied
        discount factor.
    """
    def __init__(self, mdp, discount = 0.9, iterations = 1000):
        """
          Your cyclic value iteration agent should take an mdp on
          construction, run the indicated number of iterations,
          and then act according to the resulting policy. Each iteration
          updates the value of only one state, which cycles through
          the states list. If the chosen state is terminal, nothing
          happens in that iteration.

          Some useful mdp methods you will use:
              mdp.getStates()
              mdp.getPossibleActions(state)
              mdp.getTransitionStatesAndProbs(state, action)
              mdp.getReward(state)
              mdp.isTerminal(state)
        """
        ValueIterationAgent.__init__(self, mdp, discount, iterations)

    def runValueIteration(self):
        "*** YOUR CODE HERE ***"
        iter_num = 0  # Initialize.
        states = self.mdp.getStates()  # Get all states
        for i in range(self.iterations):
            if iter_num == len(states):  # Check if completed a cycle.
                iter_num = 0  # Reset loop.
            state = states[iter_num]  # Get current state.
            iter_num += 1  # Increment.
            best_action = self.computeActionFromValues(state)  # Get the best action.
            if best_action is None:  # Check if its terminal state.
                continue
            self.values[state] = self.getQValue(state, best_action)  # Update the value.


class PrioritizedSweepingValueIterationAgent(AsynchronousValueIterationAgent):
    """
        * Please read learningAgents.py before reading this.*

        A PrioritizedSweepingValueIterationAgent takes a Markov decision process
        (see mdp.py) on initialization and runs prioritized sweeping value iteration
        for a given number of iterations using the supplied parameters.
    """
    def __init__(self, mdp, discount = 0.9, iterations = 100, theta = 1e-5):
        """
          Your prioritized sweeping value iteration agent should take an mdp on
          construction, run the indicated number of iterations,
          and then act according to the resulting policy.
        """
        self.theta = theta
        ValueIterationAgent.__init__(self, mdp, discount, iterations)

    def runValueIteration(self):
        "*** YOUR CODE HERE ***"
        pred_dic = util.Counter()  # A dictionary with all states.
        states = self.mdp.getStates()  # Get all states.
        states_queue = util.PriorityQueue()  # Initialize a priority queue.
        for state in states:  # Go over all states.
            if not self.mdp.isTerminal(state):  # Check if the state is terminal.
                pred_dic[state] = []  # Create a list in the dictionary.
        for state in states:  # Go over all states.
            if self.mdp.isTerminal(state):  # Check if its terminal.
                continue
            actions = self.mdp.getPossibleActions(state)  # Get a list of all possible actions.
            for action in actions:  # Go over all actions.
                transitions = self.mdp.getTransitionStatesAndProbs(state, action)  # Get transition function
                for transition in transitions:
                    next_state, prob = transition
                    if prob != 0 and not self.mdp.isTerminal(next_state) and state not in pred_dic[next_state]:
                        pred_dic[next_state].append(state)
            best_action = self.computeActionFromValues(state)  # Get the best action.
            diff = abs(self.values[state] - self.getQValue(state, best_action))  # Calculate the diff.
            states_queue.push(state, -diff)  # Push to queue.
        for i in range(self.iterations):
            if states_queue.isEmpty():
                return
            state = states_queue.pop()  # Get the state with the biggest diff.
            best_action = self.computeActionFromValues(state)  # Get the best action.
            self.values[state] = self.getQValue(state, best_action)  # Set his value.
            for pred in pred_dic[state]:
                best_action = self.computeActionFromValues(pred)  # Get the best action.
                diff = abs(self.values[pred] - self.getQValue(pred, best_action))  # Calculate the absolute diff.
                if diff > self.theta:
                    states_queue.update(pred, -diff)
