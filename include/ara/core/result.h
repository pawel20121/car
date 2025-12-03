#ifndef ARA_CORE_RESULT_H
#define ARA_CORE_RESULT_H

#include <type_traits>
#include <utility>
#include <stdexcept>

/**
 * @file result.h
 * @brief Result<T, E> type for AUTOSAR Adaptive Platform
 * 
 * Simplified implementation of ara::core::Result for error handling.
 * Based on AUTOSAR AP Core Specification.
 * 
 * This is a minimal implementation for the State Management project.
 * Production code should use the full AUTOSAR ara::core implementation.
 */

namespace ara {
namespace core {

/**
 * @brief Error info class
 * 
 * Wraps an error code and provides error information.
 */
template<typename ErrorEnum>
class ErrorCode {
public:
    using ValueType = ErrorEnum;

    /**
     * @brief Constructs an ErrorCode
     * 
     * @param value Error enumeration value
     */
    explicit ErrorCode(ErrorEnum value) : value_(value) {}

    /**
     * @brief Gets the error value
     * 
     * @return Error enum value
     */
    ErrorEnum Value() const noexcept {
        return value_;
    }

    /**
     * @brief Conversion operator to ErrorEnum
     */
    operator ErrorEnum() const noexcept {
        return value_;
    }

    /**
     * @brief Equality comparison
     */
    bool operator==(const ErrorCode& other) const noexcept {
        return value_ == other.value_;
    }

    /**
     * @brief Inequality comparison
     */
    bool operator!=(const ErrorCode& other) const noexcept {
        return value_ != other.value_;
    }

private:
    ErrorEnum value_;
};

/**
 * @brief Result type for operations that can fail
 * 
 * @tparam T Value type (use void for operations without return value)
 * @tparam E Error type (default: ara::sm::StateManagementErrc)
 * 
 * Result can contain either a value (success) or an error (failure).
 * Similar to std::expected (C++23) or Rust's Result<T, E>.
 * 
 * Usage:
 * @code
 * Result<void> DoSomething() {
 *     if (success) {
 *         return Result<void>();  // Success
 *     } else {
 *         return Result<void>(StateManagementErrc::kOperationFailed);
 *     }
 * }
 * 
 * auto result = DoSomething();
 * if (result.HasValue()) {
 *     // Success
 * } else {
 *     auto error = result.Error();
 *     // Handle error
 * }
 * @endcode
 */
template<typename T = void, typename E = int>
class Result {
public:
    using ValueType = T;
    using ErrorType = E;

    /**
     * @brief Constructs a Result with a value (success)
     * 
     * @param value The success value
     */
    explicit Result(const T& value) 
        : hasValue_(true), value_(value) {}

    /**
     * @brief Constructs a Result with a value (success, move)
     * 
     * @param value The success value
     */
    explicit Result(T&& value) 
        : hasValue_(true), value_(std::move(value)) {}

    /**
     * @brief Constructs a Result with an error (failure)
     * 
     * @param error The error code
     */
    explicit Result(const ErrorCode<E>& error) 
        : hasValue_(false), error_(error.Value()) {}

    /**
     * @brief Constructs a Result with an error (failure)
     * 
     * @param error The error enum value
     */
    explicit Result(E error) 
        : hasValue_(false), error_(error) {}

    /**
     * @brief Checks if Result contains a value
     * 
     * @return true if success, false if error
     */
    bool HasValue() const noexcept {
        return hasValue_;
    }

    /**
     * @brief Boolean conversion operator
     * 
     * @return true if success, false if error
     */
    explicit operator bool() const noexcept {
        return HasValue();
    }

    /**
     * @brief Gets the value (throws if error)
     * 
     * @return Reference to the value
     * @throws std::logic_error if Result contains an error
     */
    const T& Value() const {
        if (!hasValue_) {
            throw std::logic_error("Result contains an error, not a value");
        }
        return value_;
    }

    /**
     * @brief Gets the value (throws if error)
     * 
     * @return Reference to the value
     * @throws std::logic_error if Result contains an error
     */
    T& Value() {
        if (!hasValue_) {
            throw std::logic_error("Result contains an error, not a value");
        }
        return value_;
    }

    /**
     * @brief Gets the error (throws if value)
     * 
     * @return The error code
     * @throws std::logic_error if Result contains a value
     */
    E Error() const {
        if (hasValue_) {
            throw std::logic_error("Result contains a value, not an error");
        }
        return error_;
    }

    /**
     * @brief Gets the value or a default
     * 
     * @param defaultValue Default value if Result contains an error
     * @return The value or default
     */
    T ValueOr(const T& defaultValue) const {
        return hasValue_ ? value_ : defaultValue;
    }

private:
    bool hasValue_;
    union {
        T value_;
        E error_;
    };
};

/**
 * @brief Result specialization for void (operations with no return value)
 * 
 * @tparam E Error type
 * 
 * Used for operations that only indicate success/failure without returning data.
 */
template<typename E>
class Result<void, E> {
public:
    using ErrorType = E;

    /**
     * @brief Constructs a successful Result (no error)
     */
    Result() : hasValue_(true), error_() {}

    /**
     * @brief Constructs a Result with an error
     * 
     * @param error The error code
     */
    explicit Result(const ErrorCode<E>& error) 
        : hasValue_(false), error_(error.Value()) {}

    /**
     * @brief Constructs a Result with an error
     * 
     * @param error The error enum value
     */
    explicit Result(E error) 
        : hasValue_(false), error_(error) {}

    /**
     * @brief Checks if Result is successful
     * 
     * @return true if success, false if error
     */
    bool HasValue() const noexcept {
        return hasValue_;
    }

    /**
     * @brief Boolean conversion operator
     * 
     * @return true if success, false if error
     */
    explicit operator bool() const noexcept {
        return HasValue();
    }

    /**
     * @brief Gets the error (throws if success)
     * 
     * @return The error code
     * @throws std::logic_error if Result is successful
     */
    E Error() const {
        if (hasValue_) {
            throw std::logic_error("Result is successful, no error");
        }
        return error_;
    }

private:
    bool hasValue_;
    E error_;
};

/**
 * @brief Helper function to create a successful Result
 * 
 * @tparam T Value type
 * @param value The value
 * @return Result containing the value
 */
template<typename T>
Result<T> MakeResult(T&& value) {
    return Result<T>(std::forward<T>(value));
}

/**
 * @brief Helper function to create an error Result
 * 
 * @tparam E Error type
 * @tparam T Value type (deduced)
 * @param error The error code
 * @return Result containing the error
 */
template<typename T = void, typename E>
Result<T, E> MakeError(E error) {
    return Result<T, E>(error);
}

} // namespace core
} // namespace ara

#endif // ARA_CORE_RESULT_H