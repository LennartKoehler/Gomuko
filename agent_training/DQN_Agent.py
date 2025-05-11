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
    def __init__(self, DQN, playerID, state_dim, action_dim, lr, gamma, epsilon, epsilon_decay, buffer_size, batch_size, agentID):
        self.agentID = agentID
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
        self.last_turn = {"state" : np.array([]), "action" : 0}
        self.win_reward = 1
        self.lose_reward = -1
        self.tie_reward = 0.5
        self.unvalid_action_reward = -10
        self.loss_interval = 20 # save every 20th loss
        self.loss_counter = 0
        self.reward_dict = {"none": 0, "win": self.win_reward, "lose": self.lose_reward, "tie": self.tie_reward, "unvalid_action": self.unvalid_action_reward}
        # map game_result to reward
    




    def turn(self, env, iteration):
        #turn_time_start = time.time()
        state = env.get_state()
        done = False
        unvalid_action_counter = 0

        if env.get_game_result()["tie"]:
            self.remember(self.last_turn["state"], self.last_turn["action"], state, "tie_opp", self.reward_dict["tie"]) #agent1 goes into this if statement for no reason, somehow i guess the game is not stopped after tie_me
            done = True
        elif env.get_game_result()["win"]:
            self.remember(self.last_turn["state"], self.last_turn["action"], state, "lose", self.reward_dict["lose"])
            done = True
        else:
            action = self.act(state)
            unvalid_action_counter, done = self.take_turn(env, state, iteration, action)

        #replay_time_start = time.time()
        self.replay(self.batch_size)



        # turn_time = (time.time() - turn_time_start)*1000
        # replay_time = (time.time() - replay_time_start)*1000
        # print(f"turn time: {turn_time:.2f}ms, replay time: {replay_time:.2f}")
        
        return unvalid_action_counter, done # done is when an agent says hes done (he still needs to remember the loss after the game is already finished)

    
    def take_turn(self, env, state, iteration, action):
        done = False
        unvalid_action_counter = 0
        unvalid_action, _ = env.step(action, 1)

        if env.get_game_result()["tie"]:
            self.remember(self.last_turn["state"], self.last_turn["action"], state, "none", self.reward_dict["none"])
            self.remember(state, action, state, "tie_me", self.reward_dict["tie"])
            done = True
        elif env.get_game_result()["win"]:
            self.remember(self.last_turn["state"], self.last_turn["action"], state, "none", self.reward_dict["none"])
            self.remember(state, action, state, "win", self.reward_dict["win"])
            done = True
        elif unvalid_action:
            self.remember(state, action, state, "unvalid_action", self.reward_dict["unvalid_action"])
            action = self.act_random_viable(env)
            return 1, self.take_turn(env, state, iteration, action)[1] # recursive
        else: #normal move
            if iteration != 0:
                self.remember(self.last_turn["state"], self.last_turn["action"], state, "none", self.reward_dict["none"])
            self.last_turn["state"] = state
            self.last_turn["action"] = action
        return unvalid_action_counter, done

    def remember(self, state, action, state_next, game_result, reward):
        self.memory.append({
        'state': state,
        'action': action,
        'state_next': state_next,
        'game_result': game_result,
        'reward': reward
    })
        
    def act_random_viable(self, env):
        state = env.get_state()
        zero_indices = [index for index, value in enumerate(state) if value == 0]
        random_index = random.choice(zero_indices)
        return random_index

    def act_random(self):
        return np.random.choice(self.action_dim)

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return self.act_random()
        q_values = self.model(torch.tensor(state, dtype=int, device=self.device))
        return torch.argmax(q_values).item()



    def replay(self, batch_size):
        if len(self.memory) < batch_size:
            return
        minibatch = random.sample(self.memory, batch_size)
        for experience in minibatch:

            state = experience['state']
            action = experience['action']
            state_next = experience['state_next']
            game_result = experience['game_result']
            reward = experience['reward']

            #s_time = time.time()

            target = reward
            output = self.model(torch.tensor(state, dtype=int, device=self.device))

            if game_result == "none": # if unvalid action then dont learn from the next state
                output_next = self.model(torch.tensor(state_next, dtype=int, device=self.device))
                target = reward + self.gamma * torch.max(output_next).item()

            debug_state = np.reshape(state, (15,15))
            debug_action = (action//15+1, action%15+1)
            debug_reward = reward
            debug_state_next = np.reshape(state_next, (15,15))


            # if game_result == "win":
            #     pass
            # if game_result == "lose":
            #     pass
            # if game_result == "tie_me":
            #     pass
            # if game_result == "unvalid_action":
            #         pass

            target_f = output.detach().clone()
            target_f[action] = target
            self.optimizer.zero_grad()
            loss = nn.MSELoss()(target_f, output)
            loss.backward()
            self.optimizer.step()
            # backprop_time = time.time() - s_time
            # print(f"backprop time: {backprop_time*1000:.2f}")

        if self.epsilon > 0.01:
            self.epsilon *= self.epsilon_decay


        self.loss_counter += 1
        if self.loss_counter % self.loss_interval == 0:
            self.losses.append(loss.cpu().detach().numpy())
