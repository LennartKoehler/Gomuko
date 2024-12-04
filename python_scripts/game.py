import numpy as np

class Game:
    def __init__(self, size, win_condition_number):
        self.size = size
        self.state_matrix = np.zeros(size* size)
        self.win_condition_number = win_condition_number

    def set_value(self, x, y, value):
        self.state_matrix[x + y * self.size] = value

    def get_value(self, x, y):
        return self.state_matrix[x + y * self.size]

    def place_piece(self, x, y, number):
        self.set_value( x, y, number)

    def place_piece_request(self, x, y, number):
        if self.get_value(x, y) == 0:
            self.place_piece(x, y, number)
            return True
        
        else:
            return False
        
    def place_piece_request_position(self, position, number):
        y = position // self.size
        x = position % self.size
        return self.place_piece_request(x, y, number)



    def check_if_won(self, player_id):
        n_rows = n_cols = self.size

        # Check rows and columns
        for i in range(n_rows):
            row_count = 0
            col_count = 0
            for j in range(n_cols):
                # Check row
                if self.get_value(i, j) == player_id:
                    col_count += 1
                    if col_count >= self.win_condition_number:
                        return True
                else:
                    col_count = 0

                # Check column
                if self.get_value(j, i) == player_id:
                    row_count += 1
                    if row_count >= self.win_condition_number:
                        return True
                else:
                    row_count = 0

        # Check diagonals
        for d in range(-n_rows + 1, n_cols):
            main_diagonal_count = 0
            anti_diagonal_count = 0
            for i in range(n_rows):
                j = i + d
                # Check main diagonal
                if 0 <= j < n_rows:
                    if self.get_value(i, j) == player_id:
                        main_diagonal_count += 1
                        if main_diagonal_count >= self.win_condition_number:
                            return True
                    else:
                        main_diagonal_count = 0

                # Check anti-diagonal
                anti_j = n_rows - 1 - i + d
                if 0 <= anti_j < n_rows:
                    if self.get_value(i, anti_j) == player_id:
                        anti_diagonal_count += 1
                        if anti_diagonal_count >= self.win_condition_number:
                            return True
                    else:
                        anti_diagonal_count = 0

        return False



    
class Game_training_wrapper:
    def __init__(self, size):
        self.game = Game(size, 5)
        self.win_reward = 10
        self.unvalid_penalty = -10

    def step(self, action, playerID):
        valid_action = self.game.place_piece_request_position(action, playerID)
        win = self.game.check_if_won(playerID)
        reward = self.reward_function(win, valid_action)
        new_state = self.get_state()
        return reward, win, new_state

    def get_state(self):
        return self.game.state_matrix      
    
    def reward_function(self, win: bool, valid: bool):
        if not valid:
            return self.unvalid_penalty
        if win:
            return self.win_reward
        else:
            return 0
