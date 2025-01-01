from DQN_Agent import DQNAgent
from DQN import DQN
from game import Game_training_wrapper
import numpy as np
import time
import matplotlib.pyplot as plt
import torch
import time
# Initialize environment and agent with Experience Replay Buffer

game_size = 15

state_dim = game_size * game_size * 3 # one hot encoding
action_dim = game_size * game_size # place piece on either square, illegal moves are "permitted" by the network but will increase loss

win_con = 5

# Train the DQN agent with Experience Replay Buffer
batch_size = 32
num_episodes = 1000
lr_decrease_rate = 200

network = DQN(state_dim, action_dim, game_size, in_chans=1, ch_mul=8, device="cuda").to("cuda")
#network.load_state_dict(torch.load("agent_training/trained_models/long_2_3x3_1500_iterations_state_dict.pkl"))

agent1 = DQNAgent(network, 1, state_dim, action_dim, lr=0.001, gamma=0.3, epsilon=1.0, epsilon_decay=0.995, buffer_size=1000, batch_size=batch_size, agentID=1)
agent2 = DQNAgent(network, 1, state_dim, action_dim, lr=0.001, gamma=0.3, epsilon=1.0, epsilon_decay=0.995, buffer_size=1000, batch_size=batch_size, agentID=2)

def replay_replacement(a):
    return
agent2.replay = replay_replacement # replay only for one of the 2 agents for testing

rewards = []
for episode in range(num_episodes):
    env = Game_training_wrapper(game_size, win_con)
    if (episode+1) % lr_decrease_rate == 0:
        agent1.lr /= 10
        agent1.optimizer = torch.optim.Adam(agent1.model.parameters(), lr=agent1.lr)

    state = env.get_state()
    total_reward = 0
    done1 = done2 = False
    iteration = 0

    while not (done1 and done2):
        reward, done1 = agent1.turn(env, iteration)
        env.invert_state()

        if done1 and done2:
            break

        _, done2 = agent2.turn(env, iteration)
        env.invert_state()
        total_reward += reward
        iteration += 1
    print(f"Episode: {episode + 1}, Total Reward: {total_reward}, Epsilon: {agent1.epsilon:.2f}, Learning Rate: {agent1.lr:.5f}")
    rewards.append(total_reward)
torch.save(network.state_dict(), "trained_models/test_15x15_1000_iterations_state_dict.pkl")

plt.plot(agent1.losses)
plt.show()
plt.plot(rewards)
plt.show()
    
 