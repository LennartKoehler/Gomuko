#pragma once
#include "../src/Game_Client/VectorMatrix.hpp"


struct PlayOutCommon{
    Matrix<int> old_state;
    Matrix<float> action_probs;
    Matrix<int> new_state;
    PlayOutCommon(const Matrix<int>& old_state, const Matrix<float>& action_probs, const Matrix<int>& new_state)
        : old_state(old_state), 
        action_probs(action_probs), 
        new_state(new_state){}
};

struct PlayOut : public PlayOutCommon {
    int player;

    // Constructor
    PlayOut(const Matrix<int>& old_state, int player, const Matrix<float>& action_probs, const Matrix<int>& new_state)
        : PlayOutCommon(old_state, action_probs, new_state),
          player(player) {}
};

struct PlayOutReward : public PlayOutCommon {
    float reward;

    // Constructor
    PlayOutReward(const Matrix<int>& old_state, float reward, const Matrix<float>& action_probs, const Matrix<int>& new_state)
        : PlayOutCommon(old_state, action_probs, new_state),
          reward(reward) {}

    // Constructor from PlayOut
    PlayOutReward(const PlayOut& play_out, float reward)
        : PlayOutCommon(play_out.old_state, play_out.action_probs, play_out.new_state),
          reward(reward) {}
          
    PlayOutReward(const PlayOutReward& other)
        : PlayOutCommon(other.old_state, other.action_probs, other.new_state),
          reward(other.reward) {}
};