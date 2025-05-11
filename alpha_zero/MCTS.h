#pragma once
#include <map>
#include <tuple>
#include "Gomoku.h"
#include "ModelParallel.h"


class Node;
class Action;
class ChildSelection;

struct Action{
    Action(int row, int col): row(row), col(col){}
    Action(){}
    bool operator<(const Action& other) const {
        // Compare rows first
        if (row != other.row) {
            return row < other.row;
        }
        // If rows are equal, compare columns
        return col < other.col;
    }
    int row;
    int col;
};

struct ChildSelection {
    Action action;
    std::shared_ptr<Node> child = nullptr;;

    ChildSelection(){}
    ChildSelection(const Action& action, const std::shared_ptr<Node>& child)
        : action(action), child(child) {}
};

class Node{
public:
    int visit_count;
    float prior;
    int to_play;
    float value_sum;
    std::map<Action, std::shared_ptr<Node>> children;
    Matrix<int> state;

    Node(float prior, int to_play);
    bool expanded() const;
    float value() const;
    Action select_action(float temperature) const;
    ChildSelection select_child();
    void expand(const Matrix<int> state, int to_play, const Matrix<float>& action_probs);

};

class MCTS{
public:
    MCTS(GomokuTraining* game, std::shared_ptr<Model> model, int depth);
    std::shared_ptr<Node> run(Matrix<int>& game_state, int player_id);

private:
    void backpropagate(std::vector<std::shared_ptr<Node>>& search_path, float value, int player_id);
    GomokuTraining* game;
    std::shared_ptr<Model> model;
    int depth;

};