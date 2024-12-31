import numpy as np

class Game:
    def __init__(self, size, win_condition_number):
        self.size = size
        self.state_matrix = np.zeros(size* size, dtype=int)
        self.win_condition_number = win_condition_number
        self.game_result = {"win": False, "tie": False}

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
        if self.state_matrix[position] == 0:
            self.state_matrix[position] = number
            return True
        else:
            return False
        
    def check_tie(self):
        if 0 in self.state_matrix:
            return False
        else:
            self.game_result["tie"] = True
            return True

    def __str__(self):
        return str(self.state_matrix.reshape((self.size, self.size)))

    def check_if_won(self, player_id):
        n_rows = n_cols = self.size
        win = False

        # Check rows and columns
        for i in range(n_rows):
            row_count = 0
            col_count = 0
            for j in range(n_cols):
                # Check row
                if self.get_value(i, j) == player_id:
                    col_count += 1
                    if col_count == self.win_condition_number:
                        win = True
                else:
                    col_count = 0

                # Check column
                if self.get_value(j, i) == player_id:
                    row_count += 1
                    if row_count == self.win_condition_number:
                        win = True
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
                        if main_diagonal_count == self.win_condition_number:
                            win = True
                    else:
                        main_diagonal_count = 0

                # Check anti-diagonal
                anti_j = n_rows - 1 - i + d
                if 0 <= anti_j < n_rows:
                    if self.get_value(i, anti_j) == player_id:
                        anti_diagonal_count += 1
                        if anti_diagonal_count == self.win_condition_number:
                            win = True
                    else:
                        anti_diagonal_count = 0

        self.game_result["win"] = win

        return win



    
class Game_training_wrapper:
    def __init__(self, size, win_con):
        self.game = Game(size, win_con)

    def get_game_result(self):
        return self.game.game_result.copy()

    def step(self, action, playerID):
        unvalid_action = not self.game.place_piece_request_position(action, playerID)
        new_state = self.get_state()
        if not unvalid_action:
            if not self.game.check_if_won(1):
                self.game.check_tie()
        return unvalid_action, new_state

    def get_state(self):
        return self.game.state_matrix.copy()
     
    def invert_state(self):
        temp = self.game.state_matrix.copy()
        temp[self.game.state_matrix == 1] = 2
        temp[self.game.state_matrix == 2] = 1
        self.game.state_matrix = temp.copy()
        
    def __str__(self):
        return self.game.__str__()
