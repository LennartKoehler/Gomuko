import numpy as np
import game

import torch
import torch.nn as nn

# torch.manual_seed(1)
# class model(nn.Module):
#     def __init__(self):
#         super(model, self).__init__()
#         self.fc1 = nn.Linear(4, 4)
#         self.fc2 = nn.Linear(4, 4)
#     def forward(self, x):
#         return self.fc1(self.fc2(x))




# x = torch.tensor([0.5,1,1,2], dtype=torch.float)
# nn_model = model()
# optim = torch.optim.Adam(nn_model.parameters(), lr = 0.1)

# x_res = nn_model(x)
# print(x_res)

# b = x_res.clone()
# b[0] = 5

# optim.zero_grad()
# loss = nn.MSELoss()(b, x_res)
# loss.backward()
# optim.step()

# x_res = nn_model(x)
# print(x_res)
# import os
# print(os.getcwd())
import torch
def get_2d(x, input_dim):
    c = torch.nn.Conv2d(1,16, (3, 1))
    return c(x)
tensor = torch.tensor([1,2,2,2,1,1,1,2,2,0,0,0], dtype=torch.float).reshape(3,4)[None, None,:,:]
print(tensor.shape)
print(get_2d(tensor, 9).shape)
