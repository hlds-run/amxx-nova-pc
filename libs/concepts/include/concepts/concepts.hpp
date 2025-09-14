#pragma once

#include <concepts>
#include <cstddef>
#include <ranges>
#include <span>
#include <type_traits>

namespace Concepts {
    /**
     * @brief Fundamental types that model a single byte of memory.
     */
    template <typename T>
    concept ByteLike = std::same_as<std::remove_cv_t<T>, char> || std::same_as<std::remove_cv_t<T>, unsigned char> ||
                       std::same_as<std::remove_cv_t<T>, std::byte>;

    /**
     * @brief Contiguous containers with byte-like elements.
     *
     * A container satisfying this provides a contiguous block of memory
     * where its elements are fundamental byte types.
     */
    template <typename T>
    concept ContiguousBytesContainer =
        std::ranges::contiguous_range<T> && std::ranges::sized_range<T> && requires(T& container) {
            { std::as_bytes(std::span{std::ranges::data(container), std::ranges::size(container)}) };
            requires ByteLike<std::ranges::range_value_t<T>>;
        };

    /**
     * @brief Contiguous containers with byte-like elements that can be resized or constructed with a size.
     *
     * A container satisfying this provides a contiguous block of memory
     * where its elements are fundamental byte types.
     */
    template <typename T>
     concept ResizableContiguousBytesContainer =
        ContiguousBytesContainer<T> &&
        (
            requires(T& container, std::size_t size) { container.resize(size); } ||
            requires(T& container, std::size_t size) { container.reserve(size); } ||
            requires(std::size_t size) { T(size); } 
        );

    /**
     * @brief Types that can be safely copied as a block of bytes.
     *
     * A type satisfying this is trivially copyable and has a size of at least one byte.
     */
    template <typename T>
    concept ByteSafe = std::is_trivially_copyable_v<T> && sizeof(T) >= sizeof(std::byte);

    /**
     * @brief Contiguous containers with byte-safe elements.
     *
     * A container satisfying this provides a contiguous block of memory
     * where its elements are byte-safe types that can be safely copied as bytes.
     */
    template <typename T>
    concept ContiguousByteSafeContainer =
        std::ranges::contiguous_range<T> && std::ranges::sized_range<T> && requires(T& container) {
            { std::as_bytes(std::span{std::ranges::data(container), std::ranges::size(container)}) };
            requires ByteSafe<std::ranges::range_value_t<T>>;
        };

    /**
     * @brief Contiguous containers with byte-safe elements that can be resized or constructed with a size.
     *
     * A container satisfying this provides a contiguous block of memory
     * where its elements are byte-safe types that can be safely copied as bytes.
     */
    template <typename T>
    concept ResizableContiguousByteSafeContainer =
        ContiguousByteSafeContainer<T> &&
        (requires(std::size_t size) { T(size); } || requires(T& container, std::size_t size) { container.resize(size); });
}
