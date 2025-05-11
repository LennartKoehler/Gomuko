#include "MCTS.h"
#include <algorithm> // For std::max_element and std::distance
#include <random> 
#include <vector>
#include <cmath> // For std::exp
#include <numeric> // For std::accumulate

float ucb_score(const Node& parent, const Node& child){
    float prior_score = child.prior * std::sqrt(parent.visit_count) / (child.visit_count + 1);
    float value_score;
    if (child.visit_count > 0){
        value_score = -child.value();
    }
    else{
        value_score = 0.0;
    }
    return value_score + prior_score;
}

Node::Node(float prior, int to_play)
    : prior(prior), to_play(to_play), value_sum(0), visit_count(0), state()
{}

bool Node::expanded() const{
    return children.size() > 0;
}

float Node::value() const {
    if (visit_count == 0){
        return 0;
    }
    else{
        return value_sum / visit_count;
    }
}

Action Node::select_action(float temperature) const{
    std::vector<int> visit_counts;
    std::vector<Action> actions;
    Action action;
    for (auto const& [key, val] : children){
        visit_counts.push_back(val->visit_count);
        actions.push_back(key);
    }
    if (temperature == 0){
        auto max_it = std::max_element(visit_counts.begin(), visit_counts.end());
        int max_index = std::distance(visit_counts.begin(), max_it);
        action = actions[max_index];
    }
    else if (temperature == std::numeric_limits<float>::infinity()) {
        // Select a random action
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, actions.size() - 1);
        int random_index = dis(gen);
        action = actions[random_index];
    }
    else {
        // Use softmax distribution over visit counts
        std::vector<float> visit_counts_distributions(visit_counts.size());
        float sum = 0.0;

        for (int i = 0; i < visit_counts.size(); ++i) {
            visit_counts_distributions[i] = std::pow(visit_counts[i], 1.0 / temperature);
            sum += visit_counts_distributions[i];
        }
        for (int i = 0; i < visit_counts_distributions.size(); ++i) {
            visit_counts_distributions[i] /= sum;
        }

        // Select an action based on the distribution
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<> dis(visit_counts_distributions.begin(), visit_counts_distributions.end());
        int selected_index = dis(gen);
        action = actions[selected_index];
    }
    return action;
}

ChildSelection Node::select_child() {
    float best_score = -std::numeric_limits<float>::infinity();
    Action best_action;
    std::shared_ptr<Node> best_child = nullptr;

    for (auto& [key, value] : children) {
        float score = ucb_score(*this, *value);
        if (score > best_score) {
            best_score = score;
            best_action = key;
            best_child = value;
        }
    }

    if (best_child == nullptr) {
        throw std::runtime_error("No children found in select_child");
    }

    return ChildSelection(best_action, best_child);
}

void Node::expand(const Matrix<int> state, int to_play, const Matrix<float>& action_probs){
    this->to_play = to_play;
    this->state = state;
    for (auto it = action_probs.begin(); it != action_probs.end(); ++it){
        MatrixElement ME = *it;
        if (ME.getValue() != 0){
            this->children.insert(std::pair<Action, std::shared_ptr<Node>>( Action(ME.getRow(),ME.getCol()), std::make_unique<Node>(Node(ME.getValue(), to_play * -1))));
        }
    }
}

//--------------------------



MCTS::MCTS(GomokuTraining* game, std::shared_ptr<Model> model, int depth) : game(game), model(model), depth(depth){}

std::shared_ptr<Node> MCTS::run(Matrix<int>& game_state, int player_id){
    std::shared_ptr<Node> root = std::make_shared<Node>(Node(0, player_id));
    Prediction prediction = model->predict(game_state);
    prediction.action_probs = prediction.action_probs * game->get_valid_moves(game_state);
    prediction.action_probs = prediction.action_probs / prediction.action_probs.sum();
    root->expand(game_state, player_id, prediction.action_probs);

    for (int i=0; i<depth; i++){
        std::shared_ptr<Node> node = root;
        std::vector<std::shared_ptr<Node>> search_path = {node};
        ChildSelection childselection;

        while(node->expanded()){
            childselection = node->select_child();
            search_path.push_back(childselection.child);
            node = childselection.child;
        }

        if (search_path.size() < 2) {
            throw std::runtime_error("search_path does not have enough elements");
        }
        std::shared_ptr<Node> parent = search_path[search_path.size() - 2];
        Matrix<int> next_state = game->get_next_state(parent->state, 1, childselection.action);
        next_state = game->get_canonical(next_state, -1);
        
        float value = game->get_reward(next_state, player_id);

        if (value == game->get_no_reward()){
            Prediction<Matrix<float>, float> prediction = model->predict(next_state);
            value = prediction.value;
            prediction.action_probs = prediction.action_probs * game->get_valid_moves(next_state);
            prediction.action_probs = prediction.action_probs / prediction.action_probs.sum();
            node->expand(next_state, parent->to_play*-1, prediction.action_probs);
        }
        backpropagate(search_path, value, parent->to_play*-1);
    }
    return root;
}

void MCTS::backpropagate(std::vector<std::shared_ptr<Node>>& search_path, float value, int player_id){
    for (std::shared_ptr<Node>& node : search_path){
        if (node->to_play == 1){
            node->value_sum += value;
        }
        else{
            node->value_sum -= value;
        }
        node->visit_count++;
    }
}
