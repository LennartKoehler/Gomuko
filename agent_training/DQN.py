import torch
import torch.nn as nn
import torch.nn.functional as F
import time
# Define Deep Q-Network (DQN) Model


class Block(nn.Module):
    def __init__(self, in_filters, out_filters, kernel_size, stride):
        super(Block, self).__init__()

        self.conv1=nn.Conv2d(in_filters, out_filters, kernel_size=kernel_size, stride=stride)    
        
        self.relu1 = nn.ReLU()
        self.dropout1 = nn.Dropout(0.15) 
        

    def forward(self, x):
        x = self.conv1(x)
        x = self.relu1(x)
        x = self.dropout1(x)
        
        return x

class DQN(nn.Module):
    def __init__(self, input_dim, output_dim, square_size, in_chans, ch_mul, device):
        self.input_dim = input_dim
        self.output_dim = output_dim
        self.square_size = square_size
        self.device = device
        super(DQN, self).__init__()
        
        self.enc1=Block(in_chans, ch_mul, kernel_size=(3,9), stride=(1,3))
        self.enc2=Block(ch_mul, 2*ch_mul, kernel_size=3, stride=1)
        self.enc3=Block(2*ch_mul, 4*ch_mul, kernel_size=3, stride=1)
        self.enc4=Block(4*ch_mul, 8*ch_mul, kernel_size=3, stride=1)
        
        self.linear=nn.Linear(8*ch_mul*7*7, 8*ch_mul*7*3)
        self.final=nn.Linear(8*ch_mul*7*3+input_dim, output_dim)
        self.softmax = nn.Softmax()
        
    def forward(self, x):
        #s_time = time.time()
        one_hot_x = self.get_one_hot(x, 3)
        #one_hot_time = time.time() - s_time
        x_2d = self.get_2d(one_hot_x)
        #time_2d = time.time() - one_hot_time - s_time

        enc1=self.enc1(x_2d)
        enc2=self.enc2(enc1)
        enc3=self.enc3(enc2)
        enc4=self.enc4(enc3)

        flat_enc4=enc4.view(-1)
        
        linear = self.linear(flat_enc4)
        linear_cat = torch.cat([linear, one_hot_x])
        final=self.final(linear_cat)

        # time_nn = time.time() - one_hot_time - s_time - time_2d
        # print( one_hot_time*1000, time_2d*1000, time_nn*1000)
        return final




    def get_one_hot(self, targets, nb_classes):
        res = torch.eye(nb_classes, device=self.device)[targets]
        return res.flatten()

    def get_2d(self, x):
        x = torch.reshape(x, (self.square_size, self.square_size*3))[None,None,:,:]
        return x
    
if __name__ == "__main__":
    import numpy as np
    from torchviz import make_dot
    model = DQN(15*15*3, 15*15, 15, 1, 8, "cuda")
    model.to("cuda")
    model_parameters = filter(lambda p: p.requires_grad, model.parameters())
    params = sum([np.prod(p.size()) for p in model_parameters])
    print(params)
    yhat = model(torch.zeros(15*15, dtype=torch.int, device="cuda"))
    dot = make_dot(yhat, params=dict(list(model.named_parameters())))
    dot.format = 'png'
    dot.render('./agent_training/DQN_vis')

