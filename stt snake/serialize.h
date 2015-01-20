#pragma once

#include <ostream>

/**
    Interface for an object that can be serialized.
*/
template <typename>
struct Serialize;

/**
    Serializes a list of elements, seperating neighboring elements using `joiner`.
*/
template <char joiner, typename... list>
struct Join;

template <char joiner, typename first, typename second, typename... rest>
struct Join<joiner, first, second, rest...>
{
    static std::ostream& Write(std::ostream& output)
    {
        Serialize<first>::Write(output);
        output << joiner;
        return Join<joiner, second, rest...>::Write(output);
    }
};

template <char joiner, typename first>
struct Join<joiner, first>
{
    static std::ostream& Write(std::ostream& output)
    {
        return Serialize<first>::Write(output);
    }
};

template <char joiner>
struct Join<joiner>
{
    static std::ostream& Write(std::ostream& output)
    {
        return output;
    }
};

/*------------------------------------------------------------------------------
    Basic Type Serialization
*/
template <>
struct Serialize<bool> { static std::ostream& Write(std::ostream& output) { return output << "bool"; } };

template <>
struct Serialize<int> { static std::ostream& Write(std::ostream& output) { return output << "int"; } };

template <>
struct Serialize<unsigned> { static std::ostream& Write(std::ostream& output) { return output << "unsigned"; } };

template <>
struct Serialize<size_t> { static std::ostream& Write(std::ostream& output) { return output << "size_t"; } };

/*------------------------------------------------------------------------------
    Value Type Serialization
*/

/**
    Interface for a value that can be serialized.
*/
template <typename T, T x>
struct SerializeValue
{
     static std::ostream& Write(std::ostream& output) { return output << x; };
};

template <bool x>
struct SerializeValue<bool, x> { static std::ostream& Write(std::ostream& output) { return output << (x ? "true" : "false"); } };

/**
    Serializes a list of values, seperating neighboring elements using `joiner`.
*/
template <char joiner, typename T, T... list>
struct JoinValue;

template <char joiner, typename T, T first, T second, T... rest>
struct JoinValue<joiner, T, first, second, rest...>
{
    static std::ostream& Write(std::ostream& output)
    {
        SerializeValue<T, first>::Write(output);
        output << joiner;
        return JoinValue<joiner, T, second, rest...>::Write(output);
    }
};

template <char joiner, typename T, T first>
struct JoinValue<joiner, T, first>
{
    static std::ostream& Write(std::ostream& output)
    {
        return SerializeValue<T, first>::Write(output);
    }
};

template <char joiner, typename T>
struct JoinValue<joiner, T>
{
    static std::ostream& Write(std::ostream& output)
    {
        return output;
    }
};

/**
    Serializes an integer_sequence.
*/
template <typename T, T x, T... xs>
struct Serialize<std::integer_sequence<T, x, xs...>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "std::integer_sequence<";
        Serialize<T>::Write(output) << ",";
        return JoinValue<',', T, x, xs...>::Write(output) << ">";
    }
    
};
template <typename T>
struct Serialize<std::integer_sequence<T>>
{
    static std::ostream& Write(std::ostream& output)
    {
        output << "std::integer_sequence<";
        return Serialize<T>::Write(output) << ">";
    }
};
