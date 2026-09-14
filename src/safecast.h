/*
 * Copyright (C) 2014-2019 Miroslav Fontan
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/**
 * @file safecast.h
 * @brief simple templates for runtime integer conversion checking
 *        ideal case: replace all reported places from -Wconversion
 **/

#ifndef SAFECAST_H
#define SAFECAST_H

#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

/**
 * @class SafeCastException
 * @brief thrown when a value does not fit into the destination type
 *
 * The message is composed in the constructor, so what() neither allocates
 * nor throws.
 */
class SafeCastException : public std::runtime_error
{
public:
    /** Constructor.
     *  @param msg conversion description
     *  @param num original value, already converted to text
     *  @param limit violated limit value, already converted to text
     */
    SafeCastException( const char* msg, const std::string& num, const std::string& limit )
        : std::runtime_error( std::string( msg ) + " " + num + " limit: " + limit )
    {
    }
};

/*
 * define NO_SAFECAST if you want switch off the checking and logging from safe_cast
 * or if your toolchain can not stomach advanced template features :-(
 */
#ifdef NO_SAFECAST

/** dummy template without overflow checks, static_cast only */
template <typename To, typename From> To safe_cast( From f ) noexcept { return static_cast<To>( f ); }

#else /*  NO_SAFECAST */

/* full template with overflow checks and logs*/

/** C1 */
namespace safecast_detail {

/* helper template to find an underlying type in case of enum */
template <typename T, typename = typename std::is_enum<T>::type> struct safe_underlying_type {
    using type = T;
};

template <typename T> struct safe_underlying_type<T, std::true_type> {
    using type = std::underlying_type_t<T>;
};

template <typename T> using safe_underlying_type_t = typename safe_underlying_type<T>::type;

/* usual arithmetic conversions for ints, std::common_type_t models the ternary operator */
template <typename A, typename B> using uac_type_t = std::common_type_t<A, B>;

/** @endcond C1 */

/**
 * @class do_conv
 * @brief wokrhorse template to make conversion check by specialization
 * @tparam To destination type in conversion
 * @tparam From original type in conversion
 * @tparam to_signed destination type is signed
 * @tparam from_signed original type is signed
 * @tparam rank_fine if destination type is wide enough
 */
template <typename To,
    typename From,
    bool to_signed = std::is_signed<To>::value,
    bool from_signed = std::is_signed<From>::value,
    bool rank_fine = ( std::numeric_limits<To>::digits + std::is_signed<To>::value >=
        std::numeric_limits<From>::digits + std::is_signed<From>::value )>
struct do_conv;

/** C2 */
/** these conversions never overflow, like int -> int, or  int -> long. */
template <typename To, typename From, bool Sign> struct do_conv<To, From, Sign, Sign, true> {
    static To callAction( From f ) noexcept { return static_cast<To>( f ); }
};

template <typename To, typename From> struct do_conv<To, From, false, false, false> {
    static To callAction( From f )
    {
        using type = uac_type_t<To, From>;
        const type limit = static_cast<type>( std::numeric_limits<To>::max() );
        if( static_cast<type>( f ) > limit ) {
            throw SafeCastException( "unsigned to unsigned", std::to_string( f ), std::to_string( limit ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, false, true, true> {
    static To callAction( From f )
    {
        if( f < 0 ) {
            throw SafeCastException( "signed to unsigned", std::to_string( f ), "0" );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, false, true, false> {
    static To callAction( From f )
    {
        using type = uac_type_t<To, From>;
        if( f < 0 ) {
            throw SafeCastException( "signed to unsigned", std::to_string( f ), "0" );
        }
        const type limit = static_cast<type>( std::numeric_limits<To>::max() );
        if( static_cast<type>( f ) > limit ) {
            throw SafeCastException( "signed to unsigned", std::to_string( f ), std::to_string( limit ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From, bool Rank> struct do_conv<To, From, true, false, Rank> {
    static To callAction( From f )
    {
        using type = uac_type_t<To, From>;
        const type limit = static_cast<type>( std::numeric_limits<To>::max() );
        if( static_cast<type>( f ) > limit ) {
            throw SafeCastException( "unsigned to signed", std::to_string( f ), std::to_string( limit ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, true, true, false> {
    static To callAction( From f )
    {
        if( f < std::numeric_limits<To>::min() ) {
            throw SafeCastException( "signed to signed", std::to_string( f ),
                std::to_string( std::numeric_limits<To>::min() ) );
        }
        if( f > std::numeric_limits<To>::max() ) {
            throw SafeCastException( "signed to signed", std::to_string( f ),
                std::to_string( std::numeric_limits<To>::max() ) );
        }
        return static_cast<To>( f );
    }
};
/** @endcond C2 */

} // namespace safecast_detail

/**
 * @brief safe conversion between integers
 * @tparam To destination type in conversion
 * @tparam From original type in conversion
 * @param f value to convert
 * @return converted vaule
 * @throws SafeCastException when the value does not fit into To
 *
 * Enums are resolved to their underlying type on both sides, so the signedness
 * and the range of the real representation are taken into account.
 */
template <typename To, typename From> To safe_cast( From f )
{
    using to_type = safecast_detail::safe_underlying_type_t<To>;
    using from_type = safecast_detail::safe_underlying_type_t<From>;

    static_assert( std::is_integral<to_type>::value, "safe_cast: To must be an integral or enum type" );
    static_assert( std::is_integral<from_type>::value, "safe_cast: From must be an integral or enum type" );

    return static_cast<To>(
        safecast_detail::do_conv<to_type, from_type>::callAction( static_cast<from_type>( f ) ) );
}

#endif /*  NO_SAFECAST */
#endif /* SAFECAST_H */
