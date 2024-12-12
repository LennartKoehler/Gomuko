import torch
import torch.nn as nn

# Define Deep Q-Network (DQN) Model
class DQN(nn.Module):
    def __init__(self, input_dim, output_dim, device):
        self.device = device
        super(DQN, self).__init__()
        self.fc1 = nn.Linear(input_dim, 900)
        self.fc2 = nn.Linear(900, 900)
        self.fc3 = nn.Linear(900, 900)
        self.fc4 = nn.Linear(900, 900)
        self.fc5 = nn.Linear(900, 900)

        self.fc6 = nn.Linear(900, output_dim)

    def forward(self, state):
        x = self.get_one_hot(state, 3)
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        x = torch.relu(self.fc3(x))
        x = torch.relu(self.fc4(x))
        x = torch.relu(self.fc5(x))

        x = self.fc6(x)
        return x
    def get_one_hot(self, targets, nb_classes):
        res = torch.eye(nb_classes, device=self.device)[targets]
        return res.flatten()
