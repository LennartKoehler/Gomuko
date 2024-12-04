from DQN_Agent import DQNAgent
from game import Game_training_wrapper
import numpy as np


# Initialize environment and agent with Experience Replay Buffer

state_dim = 15 * 15 * 3 # one hot encoding
action_dim = 15 * 15 # place piece on either square, illegal moves are "permitted" by the network but will increase loss

agent1 = DQNAgent(state_dim, action_dim, lr=0.001, gamma=0.99, epsilon=1.0, epsilon_decay=0.995, buffer_size=10000)
agent2 = DQNAgent(state_dim, action_dim, lr=0.001, gamma=0.99, epsilon=1.0, epsilon_decay=0.995, buffer_size=10000)

# Train the DQN agent with Experience Replay Buffer
batch_size = 32
num_episodes = 1000
for episode in range(num_episodes):
    env = Game_training_wrapper(15)
    state0 = env.get_state()
    total_reward = 0
    done = False
    iteration = 0
    while not done:

        action1 = agent1.act(state0)
        reward1, done1, state1 = env.step(action1, 1) # where do i get next_state from
        if iteration != 0:
            agent2.remember(state1_old, action2, reward2, state1, done2)
        state1_old = np.copy(state1)

        action2 = agent2.act(state1)
        reward2, done2, state2 = env.step(action2, 2) # where do i get next_state from

        agent1.remember(state0, action1, reward1, state2, done1)

        total_reward += reward1
        agent1.replay(batch_size)
        agent2.replay(batch_size)
        iteration += 1
    print(f"Episode: {episode + 1}, Total Reward: {total_reward}")