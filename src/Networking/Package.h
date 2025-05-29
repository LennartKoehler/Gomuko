#pragma once
#include <vector>
#include <cstdint>
#include <cstring>  // for memcpy
#include <arpa/inet.h> // for ntohl (on POSIX); use WinSock equivalent on Windows
#include <stdexcept> // for std::runtime_error
#include <variant>

#include "MessageTypes.h"
#include "Serializer.h"
#include "Networking.h"

struct Package {
    MessageType type;
    std::vector<uint8_t> payload;

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;

        // 1. Message type
        buffer.push_back(static_cast<uint8_t>(type));

        // 2. Payload length
        uint32_t netPayloadSize = static_cast<uint32_t>(payload.size());
        // uint32_t netPayloadSize = htonl(payloadSize); // convert to network byte order

        uint8_t sizeBytes[4];
        std::memcpy(sizeBytes, &netPayloadSize, sizeof(netPayloadSize));
        buffer.insert(buffer.end(), sizeBytes, sizeBytes + 4);

        // 3. Payload data
        buffer.insert(buffer.end(), payload.begin(), payload.end());

        return buffer;
    }

    static Package deserialize(const std::vector<uint8_t>& buffer) {
        if (buffer.size() < 5) {
            throw std::runtime_error("Buffer too small to contain a valid packet");
        }

        Package pkt;

        // 1. Extract message type
        pkt.type = static_cast<MessageType>(buffer[0]);
    
        // 2. Extract payload size (next 4 bytes)
        uint32_t payloadSize = 0;
        std::memcpy(&payloadSize, &buffer[1], sizeof(uint32_t));

        // payloadSize = ntohl(payloadSize); // convert from network to host byte order

        std::cerr << "payload size recieved: " << payloadSize << std::endl;;
        if (buffer.size() < 5 + payloadSize) {
            throw std::runtime_error("Buffer does not contain full payload");
        }

        // 3. Extract payload
        pkt.payload.insert(pkt.payload.end(), buffer.begin() + 5, buffer.begin() + 5 + payloadSize);

        return pkt;
    }

    // std::variant<Matrix<int>, std::string> interpret() const { // TODO is this smart?
    //     switch (type) {
    //         case MessageType::GAME_STATE_UPDATE:
    //             return Serializer::deserializeGameState(payload);
    //         case MessageType::TEXT_MESSAGE:
    //             return Serializer::deserializeText(payload);
    //         default:
    //             throw std::runtime_error("Unknown or unsupported MessageType in interpret()");
    //     }
    // }
};


// this converts the message into events
inline Package steamMessageToPackage(SteamNetworkingMessage_t* msg) {
    assert(msg && msg->m_pData && msg->m_cbSize > 0 && "Invalid SteamNetworkingMessage_t* or empty data");

    // Deserialize payload (m_pData -> Packet)
    const uint8_t* rawData = static_cast<const uint8_t*>(msg->m_pData);
    std::vector<uint8_t> buffer(rawData, rawData + msg->m_cbSize);
    Package package = Package::deserialize(buffer);

    return package;
    //TODO add back releaseMessage
}
