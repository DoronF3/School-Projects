# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        if action == Directions.STOP:  # Penalize for stopping in place.
            stop = -100
        else:
            stop = 0
        if successorGameState.isLose():  # If the next state is losing state state return lowest number.
            return -float("inf")
        elif successorGameState.isWin():  # If winning return highest number.
            return float("inf")
        num_capsules = len(successorGameState.getCapsules())  # The number of remaining capsules.
        score = successorGameState.getScore()  # The current score.
        food_list = newFood.asList()  # The list of remaining food.
        num_food = len(food_list)  # The number of food left.
        min_f_distance = float('inf')  # Set minimum distance to infinity.
        for food in food_list:  # Find path to the closest food.
            dist = util.manhattanDistance(newPos, food)  # Calculate the distance.
            if dist < min_f_distance:
                min_f_distance = dist
        min_g_distance = float('inf')  # Set minimum distance to infinity.
        for ghost_state in newGhostStates:  # Find the distance to closet active ghost.
            if ghost_state.scaredTimer == 0:  # Check if ghost is active.
                dist = util.manhattanDistance(newPos, ghost_state.start.pos)  # Calculate the distance.
                if dist < min_g_distance:
                    min_g_distance = dist
        min_g_distance = max(min_g_distance, 6)  # Set a maximum penalty for active ghost distance.
        return score + min_g_distance / (10 * min_f_distance) + stop - num_food - num_capsules

def scoreEvaluationFunction(currentGameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        "*** YOUR CODE HERE ***"
        return move_pacman(self, gameState, 1, "min")

    def ghosts_move(self, game_state, ghost_index, depth):
        if game_state.isWin() or game_state.isLose():  # If the game ended already.
            return self.evaluationFunction(game_state)
        ghost_num = ghost_index + 1
        if ghost_index == game_state.getNumAgents() - 1:
            ghost_num = 0
        min_score = float('inf')  # Initialize value.
        ghost_ac = game_state.getLegalActions(ghost_index)  # Get the list of possible actions for the ghost.
        if len(ghost_ac) == 0:  # If no legal moves.
            return self.evaluationFunction(game_state)
        for action in ghost_ac:
            successor_state = game_state.generateSuccessor(ghost_index, action)
            if ghost_num == 0:  # If there are no more ghosts.
                if depth == self.depth:  # If we are in the desired depth.
                    score = self.evaluationFunction(successor_state)
                else:
                    score = move_pacman(self, successor_state, depth + 1, "min")  # Move pacman and increase depth.
            else:
                score = self.ghosts_move(successor_state, ghost_num, depth)  # Move next ghost.
            if score < min_score:  # Get the smallest value.
                min_score = score
        return min_score


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        return move_pacman(self, gameState, 1, "ab", -float('inf'), float('inf'))

    def ghosts_move_a_b(self, game_state, ghost_index, depth, alpha, beta):
        if game_state.isWin() or game_state.isLose():  # If the game ended already.
            return self.evaluationFunction(game_state)
        ghost_num = ghost_index + 1
        if ghost_index == game_state.getNumAgents() - 1:
            ghost_num = 0
        min_score = float('inf')  # Initialize value.
        ghost_ac = game_state.getLegalActions(ghost_index)  # Get the list of possible actions for the ghost.
        if Directions.STOP in ghost_ac:
            ghost_ac.remove(Directions.STOP)
        if len(ghost_ac) == 0:  # If no legal moves.
            return self.evaluationFunction(game_state)
        for action in ghost_ac:
            successor_state = game_state.generateSuccessor(ghost_index, action)
            if ghost_num == 0:  # If there are no more ghosts.
                if depth == self.depth:  # If we are in the desired depth.
                    score = self.evaluationFunction(successor_state)
                else:
                    score = move_pacman(self, successor_state, depth + 1, "ab", alpha, beta)  # Move pacman and increase depth.
            else:
                score = self.ghosts_move_a_b(successor_state, ghost_num, depth, alpha, beta)  # Move next ghost.
            if score < min_score:  # Get the smallest value.
                min_score = score
            beta = min(beta, score)
            if min_score < alpha:
                return min_score
        return min_score

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"
        return move_pacman(self, gameState, 1, "exp")

    def ghosts_move_ex(self, game_state, ghost_index, depth):
        if game_state.isWin() or game_state.isLose():  # If the game ended already.
            return self.evaluationFunction(game_state)
        ghost_num = ghost_index + 1
        if ghost_index == game_state.getNumAgents() - 1:  # If all ghosts played.
            ghost_num = 0
        score = 0
        ghost_ac = game_state.getLegalActions(ghost_index)  # Get the list of possible actions for the ghost.
        if len(ghost_ac) == 0:  # If no legal moves.
            return self.evaluationFunction(game_state)
        inv = 1.0 / len(ghost_ac)
        for action in ghost_ac:
            successor_state = game_state.generateSuccessor(ghost_index, action)
            if ghost_num == 0:  # If there are no more ghosts.
                if depth == self.depth:  # If we are in the desired depth.
                    value = self.evaluationFunction(successor_state)
                else:
                    value = move_pacman(self, successor_state, depth + 1, "exp")  # Move pacman and increase depth.
            else:
                value = self.ghosts_move_ex(successor_state, ghost_num, depth)  # Move next ghost.
            score = score + value
        return score * inv


def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    pos = currentGameState.getPacmanPosition()
    food = currentGameState.getFood()
    ghost_states = currentGameState.getGhostStates()
    if currentGameState.isLose():
        return -float("inf")
    elif currentGameState.isWin():
        return float("inf")
    num_capsules = len(currentGameState.getCapsules())  # The number of remaining capsules.
    score = currentGameState.getScore()  # The current score.
    food_list = food.asList()  # The list of remaining food.
    num_food = len(food_list)  # The number of food left.
    min_f_distance = float('inf')  # Set minimum distance to infinity.
    for food in food_list:  # Find path to the closest food.
        dist = util.manhattanDistance(pos, food)  # Calculate the distance.
        if dist < min_f_distance:
            min_f_distance = dist
    min_g_distance = float('inf')  # Set minimum distance to infinity.
    min_g_s_distance = float('inf')  # Set minimum distance to infinity.
    for ghost_state in ghost_states:
        if ghost_state.scaredTimer == 0:
            dist = util.manhattanDistance(pos, ghost_state.start.pos)  # Calculate the distance.
            if dist < min_g_distance:
                min_g_distance = dist
        else:
            dist = util.manhattanDistance(pos, ghost_state.start.pos)  # Calculate the distance.
            if dist < min_g_s_distance:
                min_g_s_distance = dist
    if min_g_s_distance == float('inf'):
        min_g_s_distance = 0
    return score - num_capsules - num_food - min_f_distance + 1 / min_g_distance - min_g_s_distance


def move_pacman(self, game_state, depth, search, alpha=-float('inf'), beta=float('inf')):
    if game_state.isWin() or game_state.isLose():  # If the game ended already.
        return self.evaluationFunction(game_state)
    move = Directions.STOP  # Default value.
    num_ghosts = game_state.getNumAgents() - 1
    max_score = -float('inf')
    pac_ac = game_state.getLegalActions(0)  # Get the list of possible actions for pacman.
    if Directions.STOP in pac_ac:
        pac_ac.remove(Directions.STOP)
    if len(pac_ac) == 0:  # If no legal moves.
        return self.evaluationFunction(game_state)
    for action in pac_ac:
        successor_state = game_state.generateSuccessor(0, action)
        if successor_state.isWin() or (depth == self.depth and num_ghosts <= 0):  # If next move is win or only pac go there.
            score = self.evaluationFunction(successor_state)
        else:
            if search == "min":  # If minmax.
                score = self.ghosts_move(successor_state, 1, depth)
            elif search == "exp":  # If expectimax
                score = self.ghosts_move_ex(successor_state, 1, depth)
            else:  # If alpha beta.
                score = self.ghosts_move_a_b(successor_state, 1, depth, alpha, beta)
        if score > max_score:  # If the returned score is better than the current one assign and assign the move.
            max_score = score
            move = action
        if search == "ab":  # If alpha beta.
            alpha = max(alpha, score)
            if max_score > beta:
                return max_score
    if depth == 1:  # If root node.
        return move
    else:
        return max_score


# Abbreviation
better = betterEvaluationFunction
