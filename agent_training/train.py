from DQN_Agent import DQNAgent
from DQN import DQN
from game import Game_training_wrapper
import numpy as np
import time
import matplotlib.pyplot as plt
import torch

# Initialize environment and agent with Experience Replay Buffer

game_size = 3

state_dim = game_size * game_size * 3 # one hot encoding
action_dim = game_size * game_size # place piece on either square, illegal moves are "permitted" by the network but will increase loss



# Train the DQN agent with Experience Replay Buffer
batch_size = 16
num_episodes = 500

network = DQN(state_dim, action_dim, "cuda").to("cuda")

agent1 = DQNAgent(network, 1, state_dim, action_dim, lr=0.001, gamma=0.2, epsilon=1.0, epsilon_decay=0.990, buffer_size=100, batch_size=batch_size)
agent2 = DQNAgent(network, 1, state_dim, action_dim, lr=0.001, gamma=0.2, epsilon=1.0, epsilon_decay=0.990, buffer_size=100, batch_size=batch_size)
rewards = []
for episode in range(num_episodes):
    env = Game_training_wrapper(game_size)
    if episode % 100 == 0:
        agent1.optimizer = torch.optim.Adam(agent1.model.parameters(), lr=agent1.lr/10)

    state = env.get_state()
    total_reward = 0
    done1 = done2 = 0
    iteration = 0

    while done1 == 0 and done2 == 0:

        done1, reward = agent1.turn(env, iteration)
        total_reward += reward
        env.invert_state()
        if done1 != 0:
            break
        done2, _ = agent2.turn(env, iteration)
        env.invert_state()
        iteration += 1

    # done1, reward = agent1.turn(env, iteration)
    # total_reward += reward
    # env.invert_state()
    # done2, _ = agent2.turn(env, iteration)
    print(f"Episode: {episode + 1}, Total Reward: {total_reward}, Epsilon: {agent1.epsilon}")
    rewards.append(total_reward)


plt.plot(rewards)
plt.show()
    
 