// (C) Copyright Jonathan Turkanis 2003.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
// See http://www.boost.org/libs/iostreams for documentation.

// This material is heavily indebted to the discussion and code samples in
// A. Langer and K. Kreft, "Standard C++ IOStreams and Locales",
// Addison-Wesley, 2000, pp. 228-43.

#ifndef BOOST_IOSTREAMS_DETAIL_INDIRECT_STREAMBUF_HPP_INCLUDED
#define BOOST_IOSTREAMS_DETAIL_INDIRECT_STREAMBUF_HPP_INCLUDED

#include <algorithm>                             // min, max.
#include <cassert>
#include <exception>
#include <typeinfo>
#include <boost/config.hpp>                      // Member template friends.
#include <boost/detail/workaround.hpp>
#include <boost/iostreams/constants.hpp>
#include <boost/iostreams/detail/adapter/concept_adapter.hpp>
#include <boost/iostreams/detail/buffer.hpp>
#include <boost/iostreams/detail/config/wide_streams.hpp>
#include <boost/iostreams/detail/double_object.hpp> 
#include <boost/iostreams/detail/ios.hpp>
#include <boost/iostreams/detail/optional.hpp>
#include <boost/iostreams/detail/push.hpp>
#include <boost/iostreams/detail/streambuf/linked_streambuf.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/positioning.hpp>
#include <boost/iostreams/traits.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>

// Must come last.
#include <boost/iostreams/detail/config/disable_warnings.hpp>  // MSVC, BCC 5.x

namespace boost { namespace iostreams { namespace detail {

//
// Description: The implementation of basic_streambuf used by chains.
//
template<typename T, typename Tr, typename Alloc, typename Mode>
class indirect_streambuf
    : public linked_streambuf<BOOST_DEDUCED_TYPENAME io_char<T>::type, Tr>
{
public:
    typedef typename io_char<T>::type                         char_type;
    BOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(Tr)
private:
    typedef typename category_of<T>::type                     category;
    typedef concept_adapter<T>                                wrapper;
    typedef detail::basic_buffer<char_type, Alloc>            buffer_type;
    typedef indirect_streambuf<T, Tr, Alloc, Mode>            my_type;
    typedef detail::linked_streambuf<char_type, traits_type>  base_type;
    typedef linked_streambuf<char_type, Tr>                   streambuf_type;
public:
    indirect_streambuf();

    void open(const T& t BOOST_IOSTREAMS_PUSH_PARAMS());
    bool is_open();
    void close();
    bool auto_close() const;
    void set_auto_close(bool close);
    bool strict_sync();

    // Declared in linked_streambuf.
    T* component() { return &*obj(); }
protected:
#if !BOOST_WORKAROUND(__GNUC__, == 2)
    BOOST_IOSTREAMS_USING_PROTECTED_STREAMBUF_MEMBERS(base_type)
#endif

    //----------virtual functions---------------------------------------------//

#ifndef BOOST_IOSTREAMS_NO_LOCALE
    void imbue(const std::locale& loc);
#endif
    int_type underflow();
    int_type pbackfail(int_type c);
    int_type overflow(int_type c);
    int sync();
    pos_type seekoff( off_type off, BOOST_IOS::seekdir way,
                      BOOST_IOS::openmode which );
    pos_type seekpos(pos_type sp, BOOST_IOS::openmode which);

    // Declared in linked_streambuf.
    void set_next(streambuf_type* next);
    void close(BOOST_IOS::openmode m);
    const std::type_info& component_type() const { return typeid(T); }
    void* component_impl() { return component(); }
private:

    //----------Accessor functions--------------------------------------------//

    wrapper& obj() { return *storage_; }
    streambuf_type* next() const { return next_; }
    buffer_type& in() { return buffer_.first(); }
    buffer_type& out() { return buffer_.second(); }
    bool can_read() const { return is_convertible<Mode, input>::value; }
    bool can_write() const { return is_convertible<Mode, output>::value; }
    bool output_buffered() const { return (flags_ & f_output_buffered) != 0; }
    bool shared_buffer() const { return is_convertible<Mode, seekable>::value; }
    void set_flags(int f) { flags_ = f; }

    //----------State changing functions--------------------------------------//

    virtual void init_get_area();
    virtual void init_put_area();

    //----------Utility function----------------------------------------------//

    pos_type seek_impl( stream_offset off, BOOST_IOS::seekdir way,
                        BOOST_IOS::openmode which );
    void sync_impl();
    void close_impl(BOOST_IOS::openmode);

    enum {
        f_open             = 1,
        f_input_closed     = f_open << 1,
        f_output_closed    = f_input_closed << 1,
        f_output_buffered  = f_output_closed << 1,
        f_auto_close       = f_output_buffered << 1
    };

    optional<wrapper>           storage_;
    streambuf_type*             next_;
    double_object<
        buffer_type,
        is_convertible<
            Mode,
            two_sequence
        >
    >                           buffer_;
    std::streamsize             pback_size_;
    int                         flags_;
};

//--------------Implementation of indirect_streambuf--------------------------//

template<typename T, typename Tr, typename Alloc, typename Mode>
indirect_streambuf<T, Tr, Alloc, Mode>::indirect_streambuf()
    : next_(0), pback_size_(0), flags_(f_auto_close) { }

//--------------Implementation of open, is_open and close---------------------//

template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::open
    (const T& t, int buffer_size, int pback_size)
{
    using namespace std;

    // Normalize buffer sizes.
    buffer_size =
        (buffer_size != -1) ?
        buffer_size :
        iostreams::optimal_buffer_size(t);
    pback_size =
        (pback_size != -1) ?
        pback_size :
        default_pback_buffer_size;

    // Construct input buffer.
    if (can_read()) {
        pback_size_ = (std::max)(2, pback_size); // STLPort needs 2.
        streamsize size =
            pback_size_ +
            ( buffer_size ? buffer_size: 1 );
        in().resize(size);
        if (!shared_buffer())
            init_get_area();
    }

    // Construct output buffer.
    if (can_write() && !shared_buffer()) {
        if (buffer_size != 0)
            out().resize(buffer_size);
        init_put_area();
    }

    storage_.reset(wrapper(t));
    flags_ |= f_open;
    if (can_write() && buffer_size > 1)
        flags_ |= f_output_buffered;
    this->set_true_eof(false);
}

template<typename T, typename Tr, typename Alloc, typename Mode>
inline bool indirect_streambuf<T, Tr, Alloc, Mode>::is_open()
{ return (flags_ & f_open) != 0; }

template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::close()
{
    using namespace std;
    try { close(BOOST_IOS::in); } catch (std::exception&) { }
    try { close(BOOST_IOS::out); } catch (std::exception&) { }
    storage_.reset();
    flags_ = 0;
}

template<typename T, typename Tr, typename Alloc, typename Mode>
bool indirect_streambuf<T, Tr, Alloc, Mode>::auto_close() const
{ return (flags_ & f_auto_close) != 0; }

template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::set_auto_close(bool close)
{ flags_ = (flags_ & ~f_auto_close) | (close ? f_auto_close : 0); }

//--------------Implementation virtual functions------------------------------//

#ifndef BOOST_IOSTREAMS_NO_LOCALE
template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::imbue(const std::locale& loc)
{
    if (is_open()) {
        obj().imbue(loc);
        if (next_)
            next_->pubimbue(loc);
    }
}
#endif

template<typename T, typename Tr, typename Alloc, typename Mode>
typename indirect_streambuf<T, Tr, Alloc, Mode>::int_type
indirect_streambuf<T, Tr, Alloc, Mode>::underflow()
{
    using namespace std;
    if (!gptr()) init_get_area();
    buffer_type& buf = in();
    if (gptr() < egptr()) return traits_type::to_int_type(*gptr());

    // Fill putback buffer.
    streamsize keep = (std::min)( static_cast<streamsize>(gptr() - eback()),
                                  pback_size_ );
    if (keep)
        traits_type::move( buf.data() + (pback_size_ - keep),
                           gptr() - keep, keep );

    // Set pointers to reasonable values in case read throws.
    setg( buf.data() + pback_size_ - keep,
          buf.data() + pback_size_,
          buf.data() + pback_size_ );

    // Read from source.
    streamsize chars =
        obj().read(buf.data() + pback_size_, buf.size() - pback_size_, next_);
    if (chars == -1) {
        this->set_true_eof(true);
        chars = 0;
    }
    setg(eback(), gptr(), buf.data() + pback_size_ + chars);
    return chars != 0 ?
        traits_type::to_int_type(*gptr()) :
        traits_type::eof();
}

template<typename T, typename Tr, typename Alloc, typename Mode>
typename indirect_streambuf<T, Tr, Alloc, Mode>::int_type
indirect_streambuf<T, Tr, Alloc, Mode>::pbackfail(int_type c)
{
    if (gptr() != eback()) {
        gbump(-1);
        if (!traits_type::eq_int_type(c, traits_type::eof()))
            *gptr() = traits_type::to_char_type(c);
        return traits_type::not_eof(c);
    } else {
        throw bad_putback();
    }
}

template<typename T, typename Tr, typename Alloc, typename Mode>
typename indirect_streambuf<T, Tr, Alloc, Mode>::int_type
indirect_streambuf<T, Tr, Alloc, Mode>::overflow(int_type c)
{
    if ( output_buffered() && pptr() == 0 ||
         shared_buffer() && gptr() != 0 )
    {
        init_put_area();
    }
    if (!traits_type::eq_int_type(c, traits_type::eof())) {
        if (output_buffered()) {
            if (pptr() == epptr()) {
                sync_impl();
                if (pptr() == epptr())
                    return traits_type::eof();
            }
            *pptr() = traits_type::to_char_type(c);
            pbump(1);
        } else {
            char_type d = traits_type::to_char_type(c);
            if (obj().write(&d, 1, next_) != 1)
                return traits_type::eof();
        }
    }
    return traits_type::not_eof(c);
}

template<typename T, typename Tr, typename Alloc, typename Mode>
int indirect_streambuf<T, Tr, Alloc, Mode>::sync()
{
    try { // sync() is no-throw.
        sync_impl();
        obj().flush(next_);
        return 0;
    } catch (std::exception&) { return -1; }
}

template<typename T, typename Tr, typename Alloc, typename Mode>
bool indirect_streambuf<T, Tr, Alloc, Mode>::strict_sync()
{
    try { // sync() is no-throw.
        sync_impl();
        return obj().flush(next_);
    } catch (std::exception&) { return false; }
}

template<typename T, typename Tr, typename Alloc, typename Mode>
inline typename indirect_streambuf<T, Tr, Alloc, Mode>::pos_type
indirect_streambuf<T, Tr, Alloc, Mode>::seekoff
    (off_type off, BOOST_IOS::seekdir way, BOOST_IOS::openmode which)
{ return seek_impl(off, way, which); }

template<typename T, typename Tr, typename Alloc, typename Mode>
inline typename indirect_streambuf<T, Tr, Alloc, Mode>::pos_type
indirect_streambuf<T, Tr, Alloc, Mode>::seekpos
    (pos_type sp, BOOST_IOS::openmode)
{ return seek_impl(sp, BOOST_IOS::beg, BOOST_IOS::in | BOOST_IOS::out); }

template<typename T, typename Tr, typename Alloc, typename Mode>
typename indirect_streambuf<T, Tr, Alloc, Mode>::pos_type
indirect_streambuf<T, Tr, Alloc, Mode>::seek_impl
    (stream_offset off, BOOST_IOS::seekdir way, BOOST_IOS::openmode which)
{
    if (pptr() != 0) sync();
    if (way == BOOST_IOS::cur && gptr())
        off -= static_cast<off_type>(egptr() - gptr());
    setg(0, 0, 0);
    setp(0, 0);
    return obj().seek(off, way, which, next_);
}

template<typename T, typename Tr, typename Alloc, typename Mode>
inline void indirect_streambuf<T, Tr, Alloc, Mode>::set_next
    (streambuf_type* next)
{ next_ = next; }

template<typename T, typename Tr, typename Alloc, typename Mode>
inline void indirect_streambuf<T, Tr, Alloc, Mode>::close
    (BOOST_IOS::openmode which)
{
    close_impl(which);
    try { obj().close(which, next_); } catch (std::exception&) { }
}

//----------State changing functions------------------------------------------//

template<typename T, typename Tr, typename Alloc, typename Mode>
inline void indirect_streambuf<T, Tr, Alloc, Mode>::close_impl
    (BOOST_IOS::openmode which)
{
    if (which == BOOST_IOS::in && (flags_ & f_input_closed) == 0) {
        setg(0, 0, 0);
        flags_ |= f_input_closed;
    }
    if (which == BOOST_IOS::out && (flags_ & f_output_closed) == 0) {
        sync();
        setp(0, 0);
        flags_ |= f_output_closed;
    }
}

template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::sync_impl()
{
    std::streamsize avail, amt;
    if ((avail = static_cast<std::streamsize>(pptr() - pbase())) > 0) {
        if ((amt = obj().write(pbase(), avail, next())) == avail)
            setp(out().begin(), out().end());
        else {
            const char_type* ptr = pptr();
            setp(out().begin() + amt, out().end());
            pbump(ptr - pptr());
        }
    }
}

template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::init_get_area()
{
    if (shared_buffer() && pptr() != 0) {
        sync_impl();
        setp(0, 0);
    }
    setg(in().begin(), in().begin(), in().begin());
}

template<typename T, typename Tr, typename Alloc, typename Mode>
void indirect_streambuf<T, Tr, Alloc, Mode>::init_put_area()
{
    using namespace std;
    if (shared_buffer() && gptr() != 0)
        setg(0, 0, 0);
    if (output_buffered())
        setp(out().begin(), out().end());
    else
        setp(0, 0);
}

//----------------------------------------------------------------------------//

} } } // End namespaces detail, iostreams, boost.

#include <boost/iostreams/detail/config/enable_warnings.hpp> // MSVC, BCC 5.x

#endif // #ifndef BOOST_IOSTREAMS_DETAIL_INDIRECT_STREAMBUF_HPP_INCLUDED
