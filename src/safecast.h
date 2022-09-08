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

#include <string>
#include <cstdint>
#include <exception>
#include <limits>
#include <type_traits>

class SafeCastException : virtual public std::exception
{

private:
    std::string m_message;   ///< Conversion type
    long long int m_number;  ///< Checked number
    long long int m_limit;   ///< Limit value
    mutable std::string tmp; ///< internal storage

public:
    /** Constructor (C++ STL string, int, int).
     *  @param msg Conversion decription
     *  @param num origial value
     *  @param limit limit value
     */
    explicit SafeCastException( const std::string& msg, long long int num, long long int limit )
        : m_message( msg )
        , m_number( num )
        , m_limit( limit )
    {
    }

    SafeCastException( const SafeCastException& ) = default;

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~SafeCastException() noexcept {}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *  is in possession of the SafeCastException object. Callers must
     *  not attempt to free the memory.
     */
    virtual const char* what() const noexcept;
};

const char* SafeCastException::what() const noexcept
{
    tmp = m_message + " " + std::to_string( m_number ) + " limit: " + std::to_string( m_limit );
    return tmp.c_str();
}

/*
 * define NO_SAFECAST if you want switch off the checking and logging from safe_cast
 * or if your toolchain can not stomach advanced template features :-(
 */
#ifdef NO_SAFECAST

/** dummy template without overflow checks, static_cast only */
template <typename To, typename From> To safe_cast( From f ) { return static_cast<To>( f ); }

#else /*  NO_SAFECAST */

/* full template with overflow checks and logs*/

/** C1 */
/* usual arith. conversions for ints (pre-condition: A, B differ) */

/* helper template to find an underlying type in case of enum */
template <typename T, typename = typename std::is_enum<T>::type> struct safe_underlying_type {
    using type = T;
};

template <typename T> struct safe_underlying_type<T, std::true_type> {
    using type = std::underlying_type_t<T>;
};

template <int> struct uac_at;
template <> struct uac_at<1> {
    using type = int;
};
template <> struct uac_at<2> {
    using type = unsigned int;
};
template <> struct uac_at<3> {
    using type = long;
};
template <> struct uac_at<4> {
    using type = unsigned long;
};
template <> struct uac_at<5> {
    using type = long long;
};
template <> struct uac_at<6> {
    using type = unsigned long long;
};

template <typename A, typename B> struct uac_type {
    static char ( &f( int ) )[1];
    static char ( &f( unsigned int ) )[2];
    static char ( &f( long ) )[3];
    static char ( &f( unsigned long ) )[4];
    static char ( &f( long long ) )[5];
    static char ( &f( unsigned long long ) )[6];
    using type = typename uac_at<static_cast<int>( sizeof f(
        false ? typename safe_underlying_type<A>::type() : typename safe_underlying_type<B>::type() ) )>::type;
};
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
    static To callAction( From f ) { return static_cast<To>( f ); }
};

template <typename To, typename From> struct do_conv<To, From, false, false, false> {
    static To callAction( From f )
    {
        using type = typename uac_type<To, From>::type;
        if( f > static_cast<type>( std::numeric_limits<To>::max() ) ) {
            throw SafeCastException( "unsigned to unsigned", static_cast<long long int>( f ),
                static_cast<type>( std::numeric_limits<To>::max() ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, false, true, true> {
    static To callAction( From f )
    {
        if( 0 > f ) {
            throw SafeCastException( "signed to unsigned", f, 0 );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, false, true, false> {
    static To callAction( From f )
    {
        using type = typename uac_type<To, From>::type;
        if( 0 > f ) {
            throw SafeCastException( "signed to unsigned", f, 0 );
        }
        if( static_cast<type>( f ) > static_cast<type>( std::numeric_limits<To>::max() ) ) {
            throw SafeCastException( "signed to unsigned", f, static_cast<type>( std::numeric_limits<To>::max() ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, true, false, false> {
    static To callAction( From f )
    {
        using type = typename uac_type<To, From>::type;
        if( f > static_cast<type>( std::numeric_limits<To>::max() ) ) {
            throw SafeCastException( "unsigned to signed", f, static_cast<type>( std::numeric_limits<To>::max() ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, true, false, true> {
    static To callAction( From f )
    {
        using type = typename uac_type<To, From>::type;
        if( static_cast<type>( f ) > static_cast<type>( std::numeric_limits<To>::max() ) ) {
            throw SafeCastException( "unsigned to signed", f, static_cast<type>( std::numeric_limits<To>::max() ) );
        }
        return static_cast<To>( f );
    }
};

template <typename To, typename From> struct do_conv<To, From, true, true, false> {
    static To callAction( From f )
    {
        if( std::numeric_limits<To>::min() > f ) {
            throw SafeCastException( "signed to signed", f, std::numeric_limits<To>::min() );
        }
        if( f > std::numeric_limits<To>::max() ) {
            throw SafeCastException( "signed to signed", f, std::numeric_limits<To>::max() );
        }
        return static_cast<To>( f );
    }
};
/** @endcond C2 */

/**
 * @brief safe conversion between integers
 * @tparam To destination type in conversion
 * @tparam From original type in conversion
 * @param f value to convert
 * @return converted vaule
 */
template <typename To, typename From> To safe_cast( From f ) { return do_conv<To, From>::callAction( f ); }

#endif /*  NO_SAFECAST */
#endif /* SAFECAST_H */
