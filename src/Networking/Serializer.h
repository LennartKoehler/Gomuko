#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>  // for memcpy
#include <arpa/inet.h> // for ntohl (on POSIX); use WinSock equivalent on Windows
#include <stdexcept> // for std::runtime_error
#include "../Game_Client/VectorMatrix.hpp"

namespace Serializer {
    std::vector<uint8_t> serializeText(const std::string& text){
        return std::vector<uint8_t>(text.begin(), text.end());
    }

    std::string deserializeText(const std::vector<uint8_t>& data){
        return std::string(data.begin(), data.end());
    }



    std::vector<uint8_t> serializeGameState(const Matrix<int>& state, int playerID) {
        std::vector<uint8_t> buffer;
        uint8_t temp[4];

        auto pushInt = [&](int value) {
            uint32_t net = htonl(static_cast<uint32_t>(value));
            std::memcpy(temp, &net, 4);
            buffer.insert(buffer.end(), temp, temp + 4);
        };
        pushInt(playerID);

        // 1. tick
        pushInt(state.get_num_rows());

        // 2. score
        pushInt(state.get_num_cols());

        // 3. vector size
        pushInt(static_cast<int>(state.size()));

        // 4. data
        for (auto me : state) {
            pushInt(me.getValue());
        }

        return buffer;
    }

    std::tuple<Matrix<int>, int> deserializeGameState(const std::vector<uint8_t>& data){
        
        size_t offset = 0;

        auto readInt = [&](int& out) {
            if (offset + 4 > data.size()) {
                throw std::runtime_error("deserializeGameState: Unexpected end of data");
            }
            uint32_t netInt = 0;
            std::memcpy(&netInt, &data[offset], 4);
            offset += 4;
            out = static_cast<int>(ntohl(netInt));
        };
        int playerID = 0;
        readInt(playerID);

        // 1. Read tick
        int rows = 0;
        readInt(rows);

        // 2. Read score
        int cols = 0;
        readInt(cols);

        // 3. Read vector size
        int vectorSize = 0;
        readInt(vectorSize);

        if (vectorSize < 0 || offset + vectorSize * 4 > data.size()) {
            throw std::runtime_error("deserializeGameState: Invalid vector size or data too short");
        }


        // 4. Read each int from the vector
        std::vector<int> values;
        values.reserve(vectorSize);

        for (int i = 0; i < vectorSize; ++i) {
            int value = 0;
            readInt(value);
            values.push_back(value);
        }

        return std::tuple<Matrix<int>, int>{Matrix<int>{rows, cols, values}, playerID};
    }
}