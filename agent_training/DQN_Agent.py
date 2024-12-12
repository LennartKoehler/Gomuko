import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import random
from collections import deque
import time

from DQN import DQN

# Define DQN Agent with Experience Replay Buffer

class DQNAgent:
    def __init__(self, DQN, playerID, state_dim, action_dim, lr, gamma, epsilon, epsilon_decay, buffer_size, batch_size):
        self.batch_size = batch_size
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.playerID = playerID
        self.state_dim = state_dim
        self.action_dim = action_dim
        self.lr = lr
        self.gamma = gamma
        self.epsilon = epsilon
        self.epsilon_decay = epsilon_decay
        self.memory = deque(maxlen=buffer_size)
        self.model = DQN
        self.optimizer = optim.Adam(self.model.parameters(), lr=lr)
        self.losses = []
        self.last_values = {"state" : np.array([]), "action" : 0, "reward" : 0}
        self.win_reward = 1
        self.lose_reward = -1
        self.unvalid_action_reward = -10

    def reward_function(self, win: bool, lose: bool, unvalid_action: bool):
        reward = 0
        if win:
            reward += self.win_reward
        if lose:
            reward += self.lose_reward
        if unvalid_action:
            reward += self.unvalid_action_reward
        return reward
    
    
    def turn(self, env, iteration):
        state = env.get_state()

        action = self.act(state)
        unvalid_action, done, state_next = env.step(action, 1)

        reward = self.reward_function(done==1, done==2, unvalid_action)
        if iteration != 0 and done == 0:
            self.remember(self.last_values["state"], self.last_values["action"], self.last_values["reward"], state, done)

        if done == 1 or done == 2 or done == 3: # if win or lose
            self.remember(state, action, reward, state, done)
        # i never learn from a tie
        self.last_values["state"] = state.copy()
        self.last_values["action"] = action
        self.last_values["reward"] = reward

        self.replay(self.batch_size)

        while unvalid_action:
            action = self.act_random()
            unvalid_action, done, state = env.step(action, 1) # so that the rest of the game is not learned with unequal number of X / O

        return done, reward
    
    def act_random(self):
        return np.random.choice(self.action_dim)

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return np.random.choice(self.action_dim)
        q_values = self.model(torch.tensor(state, dtype=int, device=self.device))
        return torch.argmax(q_values).item()

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))


    def replay(self, batch_size):
        if len(self.memory) < batch_size:
            return
        minibatch = random.sample(self.memory, batch_size)
        for state, action, reward, state_next, done in minibatch:
            target = reward
            output = self.model(torch.tensor(state, dtype=int, device=self.device))
            output_next = self.model(torch.tensor(state_next, dtype=int, device=self.device))
            if reward == self.unvalid_action_reward:
                pass
            if done == 0:
                target = reward + self.gamma * torch.max(output_next).item()
            target_f = output
            target_f[action] = target
            self.optimizer.zero_grad()
            loss = nn.MSELoss()(torch.tensor(target_f), self.model(torch.tensor(state, dtype=torch.int)))
            loss.backward()
            self.optimizer.step()
            #self.losses.append(loss.cpu().detach().numpy())

        if self.epsilon > 0.01:
            self.epsilon *= self.epsilon_decay
        #print(loss)