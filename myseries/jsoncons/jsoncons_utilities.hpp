// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSONCONS_UTILITIES_HPP
#define JSONCONS_JSONCONS_UTILITIES_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <ostream>
#include <cmath>
#include <algorithm> // std::find, std::min, std::reverse
#include <memory>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <istream> // std::basic_istream
#include <jsoncons/config/jsoncons_config.hpp>
#include <jsoncons/byte_string.hpp>
#include <jsoncons/json_exception.hpp>
#if !defined(JSONCONS_NO_TO_CHARS)
#include <charconv>
#endif

namespace jsoncons
{

template <class CharT>
class basic_null_istream : public std::basic_istream<CharT>
{
    class null_buffer : public std::basic_streambuf<CharT>
    {
    public:
        using typename std::basic_streambuf<CharT>::int_type;
        using typename std::basic_streambuf<CharT>::traits_type;
        int_type overflow( int_type ch = traits_type::eof() ) override
        {
            return ch;
        }
    } nb_;
public:
    basic_null_istream()
      : std::basic_istream<CharT>(&nb_)
    {
    }
};

#if !defined(JSONCONS_HAS_STRING_VIEW)
template <class CharT, class Traits = std::char_traits<CharT>>
class Basic_string_view_
{
private:
    const CharT* data_;
    size_t length_;
public:
    typedef CharT value_type;
    typedef const CharT& const_reference;
    typedef Traits traits_type;
    typedef std::size_t size_type;
    static const size_type npos = size_type(-1);
    typedef const CharT* const_iterator;
    typedef const CharT* iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    Basic_string_view_()
        : data_(nullptr), length_(0)
    {
    }
    Basic_string_view_(const CharT* data, size_t length)
        : data_(data), length_(length)
    {
    }
    Basic_string_view_(const CharT* data)
        : data_(data), length_(Traits::length(data))
    {
    }
    Basic_string_view_(const Basic_string_view_& other) = default;

    template <class Allocator>
    Basic_string_view_(const std::basic_string<CharT,Traits,Allocator>& s)
        : data_(s.data()), length_(s.length())
    {
    }

    template <class Allocator>
    explicit operator std::basic_string<CharT,Traits,Allocator>() const
    { 
        return std::basic_string<CharT,Traits>(data_,length_); 
    }

    // iterator support 
    const_iterator begin() const noexcept
    {
        return data_;
    }
    const_iterator end() const noexcept
    {
        return data_ + length_;
    }
    const_iterator cbegin() const noexcept 
    { 
        return data_; 
    }
    const_iterator cend() const noexcept 
    { 
        return data_ + length_; 
    }
    const_reverse_iterator rbegin() const noexcept 
    { 
        return const_reverse_iterator(end()); 
    }
    const_reverse_iterator rend() const noexcept 
    { 
        return const_reverse_iterator(begin()); 
    }
    const_reverse_iterator crbegin() const noexcept 
    { 
        return const_reverse_iterator(end()); 
    }
    const_reverse_iterator crend() const noexcept 
    { 
        return const_reverse_iterator(begin()); 
    }

    // capacity

    size_t size() const
    {
        return length_;
    }

    size_t length() const
    {
        return length_;
    }
    size_type max_size() const noexcept 
    { 
        return length_; 
    }
    bool empty() const noexcept 
    { 
        return length_ == 0; 
    }

    // element access

    const_reference operator[](size_type pos) const 
    { 
        return data_[pos]; 
    }

    const_reference at(size_t pos) const 
    {
        if (pos >= length_)
        {
            JSONCONS_THROW(json_exception_impl<std::out_of_range>("pos exceeds length"));
        }
        return data_[pos];
    }

    const_reference front() const                
    { 
        return data_[0]; 
    }
    const_reference back()  const                
    { 
        return data_[length_-1]; 
    }

    const CharT* data() const
    {
        return data_;
    }

    // string operations

    Basic_string_view_ substr(size_type pos, size_type n=npos) const 
    {
        if (pos > length_)
        {
            JSONCONS_THROW(json_exception_impl<std::out_of_range>("pos exceeds size"));
        }
        if (n == npos || pos + n > length_)
        {
            n = length_ - pos;
        }
        return Basic_string_view_(data_ + pos, n);
    }

    int compare(Basic_string_view_ s) const 
    {
        const int rc = Traits::compare(data_, s.data_, (std::min)(length_, s.length_));
        return rc != 0 ? rc : (length_ == s.length_ ? 0 : length_ < s.length_ ? -1 : 1);
    }

    int compare(const CharT* data) const 
    {
        const size_t length = Traits::length(data);
        const int rc = Traits::compare(data_, data, (std::min)(length_, length));
        return rc != 0 ? rc : (length_ == length? 0 : length_ < length? -1 : 1);
    }

    template <class Allocator>
    int compare(const std::basic_string<CharT,Traits,Allocator>& s) const 
    {
        const int rc = Traits::compare(data_, s.data(), (std::min)(length_, s.length()));
        return rc != 0 ? rc : (length_ == s.length() ? 0 : length_ < s.length() ? -1 : 1);
    }

    size_type find(Basic_string_view_ s, size_type pos = 0) const noexcept 
    {
        if (pos > length_)
        {
            return npos;
        }
        if (s.length_ == 0)
        {
            return pos;
        }
        const_iterator it = std::search(cbegin() + pos, cend(),
                                        s.cbegin(), s.cend(), Traits::eq);
        return it == cend() ? npos : std::distance(cbegin(), it);
    }
    size_type find(CharT ch, size_type pos = 0) const noexcept
    { 
        return find(Basic_string_view_(&ch, 1), pos); 
    }
    size_type find(const CharT* s, size_type pos, size_type n) const noexcept
    { 
        return find(Basic_string_view_(s, n), pos); 
    }
    size_type find(const CharT* s, size_type pos = 0) const noexcept
    { 
        return find(Basic_string_view_(s), pos); 
    }

    size_type rfind(Basic_string_view_ s, size_type pos = npos) const noexcept 
    {
        if (length_ < s.length_)
        {
            return npos;
        }
        if (pos > length_ - s.length_)
        {
            pos = length_ - s.length_;
        }
        if (s.length_ == 0) 
        {
            return pos;
        }
        for (const CharT* p = data_ + pos; true; --p) 
        {
            if (Traits::compare(p, s.data_, s.length_) == 0)
            {
                return p - data_;
            }
            if (p == data_)
            {
                return npos;
            }
         };
    }
    size_type rfind(CharT ch, size_type pos = npos) const noexcept
    { 
        return rfind(Basic_string_view_(&ch, 1), pos); 
    }
    size_type rfind(const CharT* s, size_type pos, size_type n) const noexcept
    { 
        return rfind(Basic_string_view_(s, n), pos); 
    }
    size_type rfind(const CharT* s, size_type pos = npos) const noexcept
    { 
        return rfind(Basic_string_view_(s), pos); 
    }

    size_type find_first_of(Basic_string_view_ s, size_type pos = 0) const noexcept 
    {
        if (pos >= length_ || s.length_ == 0)
        {
            return npos;
        }
        const_iterator it = std::find_first_of
            (cbegin() + pos, cend(), s.cbegin(), s.cend(), Traits::eq);
        return it == cend() ? npos : std::distance (cbegin(), it);
    }
    size_type find_first_of(CharT ch, size_type pos = 0) const noexcept
    {
         return find_first_of(Basic_string_view_(&ch, 1), pos); 
    }
    size_type find_first_of(const CharT* s, size_type pos, size_type n) const noexcept
    { 
        return find_first_of(Basic_string_view_(s, n), pos); 
    }
    size_type find_first_of(const CharT* s, size_type pos = 0) const noexcept
    { 
        return find_first_of(Basic_string_view_(s), pos); 
    }

    size_type find_last_of(Basic_string_view_ s, size_type pos = npos) const noexcept 
    {
        if (s.length_ == 0)
        {
            return npos;
        }
        if (pos >= length_)
        {
            pos = 0;
        }
        else
        {
            pos = length_ - (pos+1);
        }
        const_reverse_iterator it = std::find_first_of
            (crbegin() + pos, crend(), s.cbegin(), s.cend(), Traits::eq);
        return it == crend() ? npos : (length_ - 1 - std::distance(crbegin(), it));
    }
    size_type find_last_of(CharT ch, size_type pos = npos) const noexcept
    { 
        return find_last_of(Basic_string_view_(&ch, 1), pos); 
    }
    size_type find_last_of(const CharT* s, size_type pos, size_type n) const noexcept
    { 
        return find_last_of(Basic_string_view_(s, n), pos); 
    }
    size_type find_last_of(const CharT* s, size_type pos = npos) const noexcept
    { 
        return find_last_of(Basic_string_view_(s), pos); 
    }

    size_type find_first_not_of(Basic_string_view_ s, size_type pos = 0) const noexcept 
    {
        if (pos >= length_)
            return npos;
        if (s.length_ == 0)
            return pos;

        const_iterator it = cend();
        for (auto p = cbegin()+pos; p != cend(); ++p)
        {
            if (Traits::find(s.data_, s.length_, *p) == 0)
            {
                it = p;
                break;
            }
        }
        return it == cend() ? npos : std::distance (cbegin(), it);
    }
    size_type find_first_not_of(CharT ch, size_type pos = 0) const noexcept
    { 
        return find_first_not_of(Basic_string_view_(&ch, 1), pos); 
    }
    size_type find_first_not_of(const CharT* s, size_type pos, size_type n) const noexcept
    { 
        return find_first_not_of(Basic_string_view_(s, n), pos); 
    }
    size_type find_first_not_of(const CharT* s, size_type pos = 0) const noexcept
    { 
        return find_first_not_of(Basic_string_view_(s), pos); 
    }

    size_type find_last_not_of(Basic_string_view_ s, size_type pos = npos) const noexcept 
    {
        if (pos >= length_)
        {
            pos = length_ - 1;
        }
        if (s.length_ == 0)
        {
            return pos;
        }
        pos = length_ - (pos+1);

        const_iterator it = crend();
        for (auto p = crbegin()+pos; p != crend(); ++p)
        {
            if (Traits::find(s.data_, s.length_, *p) == 0)
            {
                it = p;
                break;
            }
        }
        return it == crend() ? npos : (length_ - 1 - std::distance(crbegin(), it));
    }
    size_type find_last_not_of(CharT ch, size_type pos = npos) const noexcept
    { 
        return find_last_not_of(Basic_string_view_(&ch, 1), pos); 
    }
    size_type find_last_not_of(const CharT* s, size_type pos, size_type n) const noexcept
    { 
        return find_last_not_of(Basic_string_view_(s, n), pos); 
    }
    size_type find_last_not_of(const CharT* s, size_type pos = npos) const noexcept
    { 
        return find_last_not_of(Basic_string_view_(s), pos); 
    }

    friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const Basic_string_view_& sv)
    {
        os.write(sv.data_,sv.length_);
        return os;
    }
};

// ==
template<class CharT,class Traits>
bool operator==(const Basic_string_view_<CharT,Traits>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return lhs.compare(rhs) == 0;
}
template<class CharT,class Traits,class Allocator>
bool operator==(const Basic_string_view_<CharT,Traits>& lhs, 
                const std::basic_string<CharT,Traits,Allocator>& rhs)
{
    return lhs.compare(rhs) == 0;
}
template<class CharT,class Traits,class Allocator>
bool operator==(const std::basic_string<CharT,Traits,Allocator>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) == 0;
}
template<class CharT,class Traits>
bool operator==(const Basic_string_view_<CharT,Traits>& lhs, 
                const CharT* rhs)
{
    return lhs.compare(rhs) == 0;
}
template<class CharT,class Traits>
bool operator==(const CharT* lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) == 0;
}

// !=
template<class CharT,class Traits>
bool operator!=(const Basic_string_view_<CharT,Traits>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return lhs.compare(rhs) != 0;
}
template<class CharT,class Traits,class Allocator>
bool operator!=(const Basic_string_view_<CharT,Traits>& lhs, 
                const std::basic_string<CharT,Traits,Allocator>& rhs)
{
    return lhs.compare(rhs) != 0;
}
template<class CharT,class Traits,class Allocator>
bool operator!=(const std::basic_string<CharT,Traits,Allocator>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) != 0;
}
template<class CharT,class Traits>
bool operator!=(const Basic_string_view_<CharT,Traits>& lhs, 
                const CharT* rhs)
{
    return lhs.compare(rhs) != 0;
}
template<class CharT,class Traits>
bool operator!=(const CharT* lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) != 0;
}

// <=
template<class CharT,class Traits>
bool operator<=(const Basic_string_view_<CharT,Traits>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return lhs.compare(rhs) <= 0;
}
template<class CharT,class Traits,class Allocator>
bool operator<=(const Basic_string_view_<CharT,Traits>& lhs, 
                const std::basic_string<CharT,Traits,Allocator>& rhs)
{
    return lhs.compare(rhs) <= 0;
}
template<class CharT,class Traits,class Allocator>
bool operator<=(const std::basic_string<CharT,Traits,Allocator>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) >= 0;
}

// <
template<class CharT,class Traits>
bool operator<(const Basic_string_view_<CharT,Traits>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return lhs.compare(rhs) < 0;
}
template<class CharT,class Traits,class Allocator>
bool operator<(const Basic_string_view_<CharT,Traits>& lhs, 
                const std::basic_string<CharT,Traits,Allocator>& rhs)
{
    return lhs.compare(rhs) < 0;
}
template<class CharT,class Traits,class Allocator>
bool operator<(const std::basic_string<CharT,Traits,Allocator>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) > 0;
}

// >=
template<class CharT,class Traits>
bool operator>=(const Basic_string_view_<CharT,Traits>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return lhs.compare(rhs) >= 0;
}
template<class CharT,class Traits,class Allocator>
bool operator>=(const Basic_string_view_<CharT,Traits>& lhs, 
                const std::basic_string<CharT,Traits,Allocator>& rhs)
{
    return lhs.compare(rhs) >= 0;
}
template<class CharT,class Traits,class Allocator>
bool operator>=(const std::basic_string<CharT,Traits,Allocator>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) <= 0;
}

// >
template<class CharT,class Traits>
bool operator>(const Basic_string_view_<CharT,Traits>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return lhs.compare(rhs) > 0;
}
template<class CharT,class Traits,class Allocator>
bool operator>(const Basic_string_view_<CharT,Traits>& lhs, 
                const std::basic_string<CharT,Traits,Allocator>& rhs)
{
    return lhs.compare(rhs) > 0;
}
template<class CharT,class Traits,class Allocator>
bool operator>(const std::basic_string<CharT,Traits,Allocator>& lhs, 
                const Basic_string_view_<CharT,Traits>& rhs)
{
    return rhs.compare(lhs) < 0;
}
#endif

#if !defined(JSONCONS_HAS_STRING_VIEW)
template <class CharT, class Traits = std::char_traits<CharT>>
using basic_string_view = Basic_string_view_<CharT, Traits>;
using string_view = Basic_string_view_<char, std::char_traits<char>>;
using wstring_view = Basic_string_view_<wchar_t, std::char_traits<wchar_t>>;
#else
#include <string_view>
template <class CharT, class Traits = std::char_traits<CharT>>
using basic_string_view = std::basic_string_view<CharT, Traits>;
using string_view = std::string_view;
using wstring_view = std::wstring_view;
#endif

static const char* base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                           "abcdefghijklmnopqrstuvwxyz"
                                           "0123456789+/"
                                           "=";
static const char base64url_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                              "abcdefghijklmnopqrstuvwxyz"
                                              "0123456789-_"
                                              "\0";


template <class Container>
size_t encode_base16(const uint8_t* data, size_t length, Container& result)
{
    static const char* lut = "0123456789ABCDEF";

    for (size_t i = 0; i < length; ++i)
    {
        uint8_t c = data[i];
        result.push_back(lut[c >> 4]);
        result.push_back(lut[c & 15]);
    }
    return length*2;
}

template <class Container>
size_t encode_base64_generic(const uint8_t* first, size_t length, const char* alphabet, Container& result)
{
    size_t count = 0;
    const uint8_t* last = first + length;
    unsigned char a3[3];
    unsigned char a4[4];
    unsigned char fill = alphabet[64];
    int i = 0;
    int j = 0;

    while (first != last)
    {
        a3[i++] = *first++;
        if (i == 3)
        {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
            a4[3] = a3[2] & 0x3f;

            for (i = 0; i < 4; i++) 
            {
                result.push_back(alphabet[a4[i]]);
                ++count;
            }
            i = 0;
        }
    }

    if (i > 0)
    {
        for (j = i; j < 3; ++j) 
        {
            a3[j] = 0;
        }

        a4[0] = (a3[0] & 0xfc) >> 2;
        a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
        a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; ++j) 
        {
            result.push_back(alphabet[a4[j]]);
            ++count;
        }

        if (fill != 0)
        {
            while (i++ < 3) 
            {
                result.push_back(fill);
                ++count;
            }
        }
    }

    return count;
}

template <class Container>
size_t encode_base64url(const uint8_t* first, size_t length, Container& result)
{
    return encode_base64_generic(first, length, base64url_alphabet, result);
}

template <class Container>
size_t encode_base64(const uint8_t* first, size_t length, Container& result)
{
    return encode_base64_generic(first, length, base64_alphabet, result);
}

inline 
static bool is_base64(uint8_t c) 
{
    return isalnum(c) || c == '+' || c == '/';
}

inline 
static bool is_base64url(uint8_t c) 
{
    return isalnum(c) || c == '-' || c == '_';
}

template <class CharT, class F>
std::vector<uint8_t> decode_base64_generic(const std::basic_string<CharT>& base64_string, const char* alphabet, F f)
{
    const char* alphabet_end = alphabet + 65;
    std::vector<uint8_t> result;
    uint8_t a4[4], a3[3];
    uint8_t i = 0;
    uint8_t j = 0;

    auto first = base64_string.begin();
    auto last = base64_string.end();

    while (first != last && *first != '=')
    {
        if (!f(*first))
        {
            throw std::invalid_argument("Invalid encoded string");
        }

        a4[i++] = *first++; 
        if (i == 4)
        {
            for (i = 0; i < 4; ++i) 
            {
                auto p = std::find(base64_alphabet,alphabet_end,a4[i]);
                if (p == alphabet_end)
                {
                    a4[i] = 0xff;
                }
                else
                {
                    a4[i] = static_cast<uint8_t>(p - base64_alphabet);
                }
            }

            a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
            a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
            a3[2] = ((a4[2] & 0x3) << 6) +   a4[3];

            for (i = 0; i < 3; i++) 
            {
                result.push_back(a3[i]);
            }
            i = 0;
        }
    }

    if (i > 0)
    {
        for (j = 0; j < i; ++j) 
        {
            auto p = std::find(base64_alphabet,alphabet_end,a4[j]);
            if (p == alphabet_end)
            {
                a4[j] = 0xff;
            }
            else
            {
                a4[j] = static_cast<uint8_t>(p - base64_alphabet);
            }
        }

        a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
        a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; ++j) 
        {
            result.push_back(a3[j]);
        }
    }

    return result;
}

template <class CharT>
std::vector<uint8_t> decode_base64(const std::basic_string<CharT>& base64_string)
{
    return decode_base64_generic(base64_string, base64_alphabet, is_base64);
}

template <class CharT>
std::vector<uint8_t> decode_base64url(const std::basic_string<CharT>& base64_string)
{
    return decode_base64_generic(base64_string, base64url_alphabet, is_base64url);
}

template <class CharT>
std::vector<uint8_t> decode_base16(const std::basic_string<CharT>& input)
{
    static const char* const alphabet = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) 
    {
        throw std::invalid_argument("odd length");
    }

    std::vector<uint8_t> result;
    result.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = (char)input[i];
        const char* p = std::lower_bound(alphabet, alphabet + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = (char)input[i + 1];
        const char* q = std::lower_bound(alphabet, alphabet + 16, b);
        if (*q != b) 
        {
            throw std::invalid_argument("not a hex digit");
        }

        result.push_back(((p - alphabet) << 4) | (q - alphabet));
    }
    return result;
}


// json_literals

namespace detail {

template <class CharT>
basic_string_view<CharT> null_literal()
{
    static const CharT chars[] = {'n','u','l', 'l'};
    return basic_string_view<CharT>(chars,sizeof(chars));
}

template <class CharT>
basic_string_view<CharT> true_literal()
{
    static const CharT chars[] = {'t','r','u', 'e'};
    return basic_string_view<CharT>(chars,sizeof(chars));
}

template <class CharT>
basic_string_view<CharT> false_literal()
{
    static const CharT chars[] = {'f','a','l', 's', 'e'};
    return basic_string_view<CharT>(chars,sizeof(chars));
}

}

}

#endif
