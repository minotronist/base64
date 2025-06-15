#pragma once
#include <string>
#include <vector>
#include <span>
#include <expected>

namespace minotronist {

namespace base64 {
    enum class DecodeError {
        InvalidByte
    };
    auto encode(std::span<std::uint8_t const> bytes) -> std::string;
    auto decode(std::string_view encoded)
        -> std::expected<std::vector<std::uint8_t>, DecodeError>;
};
}
