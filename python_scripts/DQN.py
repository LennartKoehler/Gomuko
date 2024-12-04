import torch
import torch.nn as nn


# Define Deep Q-Network (DQN) Model
class DQN(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(DQN, self).__init__()
        self.fc1 = nn.Linear(input_dim, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, output_dim)

    def forward(self, x):
        x = self.one_hot_encoding(x)
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = self.fc3(x)
        return x
    
    def one_hot_encoding(self, state):
        output = torch.zeros(len(state)*3) # 0, 1 and 2
        for i,value in enumerate(state):
            if value == 0:
                output[i*3] = 1
            elif value == 1:
                output[i*3+1] = 1
            elif value == 2:
                output[i*3+2] = 1
            else:
                print("error in state matrix:", state)
        return output