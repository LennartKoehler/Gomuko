#pragma once

#include "Trainer.h"

namespace DataAugmentation {

    template<typename T>
    Matrix<T> rotate90(Matrix<T>& m) {
        return m.rotate90();
    }

    template<typename T>
    Matrix<T> mirrorHorizontal(Matrix<T>& m) {
        return m.mirrorHorizontal();
    }

    template<typename T>
    Matrix<T> mirrorVertical(Matrix<T>& m) {
        return m.mirrorVertical();
    }

    template<class IntF, class FloatF>
    PlayOutReward augmentSample(const PlayOutReward& in,
                                IntF  tf_int,
                                FloatF tf_float)
    {
        PlayOutReward out = in;
        out.new_state    = tf_int(out.new_state);
        out.action_probs = tf_float(out.action_probs);
        return out;
    }

    inline std::vector<PlayOutReward> augmentData(std::vector<PlayOutReward> train_examples) {
        std::vector<PlayOutReward> augmented;
        augmented.reserve(train_examples.size() * 3 * 3);

        for (auto const& sample : train_examples) {
            PlayOutReward current = sample;

            // rotate 3×, then after each rotation apply mirrorH and mirrorV
            for (int i = 0; i < 3; ++i) {
                // rotate
                current = augmentSample(current,
                                        &rotate90<int>,   // Int version
                                        &rotate90<float>  // Float version
                );
                augmented.push_back(current);

                // mirror H
                current = augmentSample(current,
                                        &mirrorHorizontal<int>,
                                        &mirrorHorizontal<float>
                );
                augmented.push_back(current);

                // mirror V
                current = augmentSample(current,
                                        &mirrorVertical<int>,
                                        &mirrorVertical<float>
                );
                augmented.push_back(current);
            }
        }

        // prepend originals if you like
        augmented.insert(augmented.begin(),
                         std::make_move_iterator(train_examples.begin()),
                         std::make_move_iterator(train_examples.end()));
        return augmented;
    }

}
