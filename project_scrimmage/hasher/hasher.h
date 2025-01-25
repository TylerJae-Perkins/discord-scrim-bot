#include <iostream>
#include <string>
#include <concepts>

template<class T>
concept valid_hash_type = std::same_as<T, uint32_t> || std::same_as<T, uint64_t>;

template<valid_hash_type T>
class fnv1a
{
public:
    static constexpr T basis = std::same_as<T, uint64_t> ? 0xCBF29CE484222325 : 0x811C9DC5;
    static constexpr T prime = std::same_as<T, uint64_t> ? 0x100000001B3 : 0x1000193;

    // Hashes a string type.
    constexpr fnv1a(std::string_view str)
    {
        m_value = basis;
        for (const auto& c : str)
        {
            m_value = (c ^ m_value) * prime;
        }
    }

    // Returns the hash value.
    constexpr T value() const
    {
        return m_value;
    }

    // Returns the hash value.
    constexpr operator T() const
    {
        return m_value;
    }

private:
    T m_value;
};

#ifdef WIN32
using FNV1A = fnv1a<uint32_t>;
#else
using FNV1A = fnv1a<uint64_t>;
#endif
// Force context to compile-time... A stupid and ugly hack.
#define CT_HASH(str) []() { constexpr auto res = FNV1A(str); return res; }()