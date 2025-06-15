#include "base64.hpp"

#include <optional>
#include <numeric>

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

namespace minotronist {
auto get_b64_code(std::uint8_t at) -> char;
auto lookup_b64_code(std::uint8_t at) -> std::optional<char>;
auto get_b64_index(char code) noexcept(false) -> std::uint8_t;
auto lookup_b64_index(char code) -> std::optional<std::uint8_t>;

constexpr size_t bit_byte = 8uz;
constexpr size_t bit_b64 = 6uz;

auto base64::encode(std::span<std::uint8_t const> bytes) -> std::string {
    auto encoded = ""s;
    auto const size_encoded = bytes.size();
    auto quot = size_encoded / 3;
    auto rem = size_encoded % 3;
    encoded.reserve((rem == 0 ? quot : (quot + 1))*4);

    auto iter = std::begin(bytes);
    for (auto end = std::next(iter, quot*3);
        iter < end;
    ) {
        auto next = std::next(iter, 3);
        auto buf_bit = std::accumulate(
            std::next(iter),
            next,
            std::uint32_t{*iter},
            [](auto acc, auto byte) {
                return (acc << 8) | byte;
            }
        );
        encoded.push_back(get_b64_code(buf_bit >> 18));
        encoded.push_back(get_b64_code(buf_bit << 14 >> 26));
        encoded.push_back(get_b64_code(buf_bit << 20 >> 26));
        encoded.push_back(get_b64_code(buf_bit << 26 >> 26));

        iter = next;
    }

    if (rem == 0) {
        return encoded;
    }

    auto buf_bit = std::accumulate(
        std::next(iter),
        std::end(bytes),
        std::uint32_t{*iter},
        [](auto acc, auto byte) {
            return (acc << 8) | byte;
        }
    );
    for (auto i = 0uz, rep = rem + 1; i < rep; i += 1) {
        encoded.push_back(get_b64_code(buf_bit << (32 - rem*8 + i*6) >> 26));
    }
    encoded.append(4 - (rem + 1), '=');

    return encoded;
}

auto base64::decode(std::string_view encoded)
    -> std::expected<std::vector<std::uint8_t>, DecodeError> {
    auto trimmed = encoded.substr(0, encoded.find_first_of("="sv));
    auto size = trimmed.size();
    auto indexes = std::vector<std::uint8_t>{};
    indexes.reserve(size);
    try {
        std::transform(
            std::begin(trimmed),
            std::end(trimmed),
            std::back_inserter(indexes),
            get_b64_index
        );
    } catch (std::bad_optional_access &e) {
        std::fprintf(stderr, "%s\n", e.what());
        return std::unexpected{DecodeError::InvalidByte};
    }

    auto bytes = std::vector<std::uint8_t>{};
    auto quot = size / 4;
    auto rem = size % 4;
    bytes.reserve((rem == 0 ? quot : (quot + 1))*3);
    auto iter = std::begin(indexes);
    for (auto end = std::next(iter, quot*4);
        iter < end;
    ) {
        auto next = std::next(iter, 4);
        auto buf_bit = std::accumulate(
            std::next(iter),
            next,
            std::uint32_t{*iter},
            [](auto acc, auto index) {
                return (acc << 6) | index;
            }
        );
        bytes.emplace_back(buf_bit >> 16);
        bytes.emplace_back(buf_bit << 16 >> 24);
        bytes.emplace_back(buf_bit << 24 >> 24);

        iter = next;
    }

    if (rem == 0) {
        return bytes;
    }

    auto buf_bit = std::accumulate(
        std::next(iter),
        std::end(indexes),
        std::uint32_t{*iter},
        [](auto acc, auto index) {
            return (acc << 6) | index;
        }
    );
    for (auto i = 0uz, rep = rem - 1; i < rep; i += 1) {
        bytes.emplace_back(buf_bit << (32 - rem*6 + i*8) >> 24);
    }

    return bytes;
}

auto get_b64_code(std::uint8_t at) -> char {
    return *lookup_b64_code(at);
}

auto lookup_b64_code(std::uint8_t at) -> std::optional<char> {
    switch (at) {
    case 0u:
        return 'A';
    case 1u:
        return 'B';
    case 2u:
        return 'C';
    case 3u:
        return 'D';
    case 4u:
        return 'E';
    case 5u:
        return 'F';
    case 6u:
        return 'G';
    case 7u:
        return 'H';
    case 8u:
        return 'I';
    case 9u:
        return 'J';
    case 10u:
        return 'K';
    case 11u:
        return 'L';
    case 12u:
        return 'M';
    case 13u:
        return 'N';
    case 14u:
        return 'O';
    case 15u:
        return 'P';
    case 16u:
        return 'Q';
    case 17u:
        return 'R';
    case 18u:
        return 'S';
    case 19u:
        return 'T';
    case 20u:
        return 'U';
    case 21u:
        return 'V';
    case 22u:
        return 'W';
    case 23u:
        return 'X';
    case 24u:
        return 'Y';
    case 25u:
        return 'Z';
    case 26u:
        return 'a';
    case 27u:
        return 'b';
    case 28u:
        return 'c';
    case 29u:
        return 'd';
    case 30u:
        return 'e';
    case 31u:
        return 'f';
    case 32u:
        return 'g';
    case 33u:
        return 'h';
    case 34u:
        return 'i';
    case 35u:
        return 'j';
    case 36u:
        return 'k';
    case 37u:
        return 'l';
    case 38u:
        return 'm';
    case 39u:
        return 'n';
    case 40u:
        return 'o';
    case 41u:
        return 'p';
    case 42u:
        return 'q';
    case 43u:
        return 'r';
    case 44u:
        return 's';
    case 45u:
        return 't';
    case 46u:
        return 'u';
    case 47u:
        return 'v';
    case 48u:
        return 'w';
    case 49u:
        return 'x';
    case 50u:
        return 'y';
    case 51u:
        return 'z';
    case 52u:
        return '0';
    case 53u:
        return '1';
    case 54u:
        return '2';
    case 55u:
        return '3';
    case 56u:
        return '4';
    case 57u:
        return '5';
    case 58u:
        return '6';
    case 59u:
        return '7';
    case 60u:
        return '8';
    case 61u:
        return '9';
    case 62u:
        return '+';
    case 63u:
        return '/';
    }

    return std::nullopt;
}

auto get_b64_index(char code) -> std::uint8_t {
    return lookup_b64_index(code).value();
}

auto lookup_b64_index(char code) -> std::optional<std::uint8_t> {
    switch (code) {
    case 'A':
        return 0u;
    case 'B':
        return 1u;
    case 'C':
        return 2u;
    case 'D':
        return 3u;
    case 'E':
        return 4u;
    case 'F':
        return 5u;
    case 'G':
        return 6u;
    case 'H':
        return 7u;
    case 'I':
        return 8u;
    case 'J':
        return 9u;
    case 'K':
        return 10u;
    case 'L':
        return 11u;
    case 'M':
        return 12u;
    case 'N':
        return 13u;
    case 'O':
        return 14u;
    case 'P':
        return 15u;
    case 'Q':
        return 16u;
    case 'R':
        return 17u;
    case 'S':
        return 18u;
    case 'T':
        return 19u;
    case 'U':
        return 20u;
    case 'V':
        return 21u;
    case 'W':
        return 22u;
    case 'X':
        return 23u;
    case 'Y':
        return 24u;
    case 'Z':
        return 25u;
    case 'a':
        return 26u;
    case 'b':
        return 27u;
    case 'c':
        return 28u;
    case 'd':
        return 29u;
    case 'e':
        return 30u;
    case 'f':
        return 31u;
    case 'g':
        return 32u;
    case 'h':
        return 33u;
    case 'i':
        return 34u;
    case 'j':
        return 35u;
    case 'k':
        return 36u;
    case 'l':
        return 37u;
    case 'm':
        return 38u;
    case 'n':
        return 39u;
    case 'o':
        return 40u;
    case 'p':
        return 41u;
    case 'q':
        return 42u;
    case 'r':
        return 43u;
    case 's':
        return 44u;
    case 't':
        return 45u;
    case 'u':
        return 46u;
    case 'v':
        return 47u;
    case 'w':
        return 48u;
    case 'x':
        return 49u;
    case 'y':
        return 50u;
    case 'z':
        return 51u;
    case '0':
        return 52u;
    case '1':
        return 53u;
    case '2':
        return 54u;
    case '3':
        return 55u;
    case '4':
        return 56u;
    case '5':
        return 57u;
    case '6':
        return 58u;
    case '7':
        return 59u;
    case '8':
        return 60u;
    case '9':
        return 61u;
    case '+':
        return 62u;
    case '/':
        return 63u;
    }

    return std::nullopt;
}
}
