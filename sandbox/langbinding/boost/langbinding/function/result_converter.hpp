// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef RESULT_CONVERTER_040925_HPP
#define RESULT_CONVERTER_040925_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/get_pointer.hpp>
#include <boost/pointee.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/langbinding/classes/holder.hpp>
#include <boost/langbinding/backend/class_instance.hpp>
#include <boost/langbinding/converter/registry.hpp>
#include <boost/langbinding/converter/registered.hpp>

//
// A typical backend holder converter could look something like:
//
//   void* to_xxx_instance(void* src, holder_installer const& installer)
//   {
//       /* create language specific instance of the given class */
//       my_class const* class_ = (my_class const*)installer.class_();
//       my_instance* x = class_->create(installer.sizeof_());
//
//       /* create language neutral instance object and install holder */
//       class_instance instance(class_, x);
//       installer.install(src, instance, x->holder_storage);
//       return x;
//   }
//

namespace boost { namespace langbinding { namespace function {

namespace aux { namespace get_pointer_ {

    struct any { template<class T> any(T const&); };
    struct no_overload_tag {};
    typedef char (&yes)[1];
    typedef char (&no)[2];

    no_overload_tag operator,(no_overload_tag, int);

    no_overload_tag get_pointer(any);
    yes check(int);
    no check(no_overload_tag);

    using boost::get_pointer;

    template<class T>
    struct has_get_pointer
    {
        static typename add_reference<T>::type x;

        BOOST_STATIC_CONSTANT(bool,
            value = sizeof(get_pointer_::check( (get_pointer(x), 0) )) == 1
        );

        typedef mpl::bool_<value> type;
    };

}} // namespace aux::get_pointer_

typedef void (*installer_function)(
     void* src, backend::class_instance instance, void* storage);

namespace aux {

    using get_pointer_::has_get_pointer;

    // T is a smart pointer of some sort. Uses get_pointer() to extract
    // the pointee type.
    template<class T>
    struct ptr_holder_installer
    {
        static void install(
            void* src
          , backend::class_instance instance
          , void* storage)
        {
            T& ptr = *static_cast<T*>(src);
            new (storage) classes::instance_holder<T>(instance, ptr, 0);
        }

        static installer_function get()
        {
            return &ptr_holder_installer<T>::install;
        }

        static std::size_t sizeof_()
        {
            return sizeof(classes::instance_holder<T>);
        }
        
        template<class U>
        static backend::class_ const* class_aux(
            backend::id backend_, const volatile U*)
        {
            return converter::registered<U>::instance.get(backend_).class_;
        }

        template<class U>
        static backend::class_ const* class_(backend::id backend_, U& x)
        {
            return class_aux(backend_, get_pointer(x));
        }
    };

    // T is a value type
    template<class T>
    struct value_holder_installer
    {
        static void install(
            void* src
          , backend::class_instance instance
          , void* storage)
        {
            typedef boost::shared_ptr<T> ptr_type;
            ptr_type ptr(new T(*static_cast<T*>(src)));
            new (storage) classes::instance_holder<ptr_type>(instance, ptr, 0);
        }

        static installer_function get()
        {
            return &value_holder_installer<T>::install;
        }

        static std::size_t sizeof_()
        {
            return sizeof(classes::instance_holder<boost::shared_ptr<T> >);
        }

        template<class U>
        static backend::class_ const* class_(backend::id backend_, U& x)
        {
            return converter::registered<U>::instance.get(backend_).class_;
        }
    };

    template<class T>
    struct no_installer
    {
        static installer_function get()
        {
            return 0;
        }
    };

} // namespace aux

// An instance of this is passed to the back end to_xxx converter.
// The back end can then use this to instantiate a holder somewhere
// in allocated memory.
struct holder_installer
{
    holder_installer();
    holder_installer(
        installer_function installer
      , backend::class_ const* class_
      , std::size_t sizeof_);

    // installs a holder in the given storage.
    void install(void* src, backend::class_instance instance, void* storage) const
    {
        assert(m_installer);
        m_installer(src, instance, storage);
    }

    // returns the class_ object representing the held class.
    backend::class_ const* class_() const
    {
        return m_class;
    }

    // returns the size of the holder.
    std::size_t sizeof_() const
    {
        return m_sizeof;
    }

    typedef installer_function const holder_installer::* safe_bool;

    operator safe_bool() const
    {
        return m_installer ? &holder_installer::m_installer : 0;
    }

private:
    installer_function m_installer;
    backend::class_ const* const m_class;
    std::size_t m_sizeof;
};

inline holder_installer::holder_installer()
    : m_installer(0)
    , m_class(0)
    , m_sizeof(0)
{
}

inline holder_installer::holder_installer(
    installer_function installer
  , backend::class_ const* class_
  , std::size_t sizeof_
  )
  : m_installer(installer)
  , m_class(class_)
  , m_sizeof(sizeof_)
{
}

template<class T>
struct result_converter
{
    explicit result_converter(
        converter::to_xxx_function callback
      , backend::plugin const& backend_
      )
      : callback(callback)
      , backend_(backend_)
    {
    }

    // The default result converter can only handle value and smart pointer
    // conversions. This should be factored out to some installer factory
    // metafunction.
    typedef typename mpl::if_<
        aux::has_get_pointer<T>
      , aux::ptr_holder_installer<T>
      , aux::value_holder_installer<T>
    >::type installer;

    // Don't generate a holder-installer if this is a fundamental type,
    // or a pointer or reference. We need explicit return value policies
    // for pointers and references.
    typedef mpl::or_<
        aux::has_get_pointer<T>
      , mpl::not_<
            mpl::or_<
                is_fundamental<T>
              , is_reference<T>
              , is_pointer<T>
            >
        >
    > has_installer;

    // auxiliry function responsible for creating holder_installer
    template<class U>
    holder_installer make_installer(mpl::false_, U&) const
    {
        return holder_installer();
    }

    template<class U>
    holder_installer make_installer(mpl::true_, U& x) const
    {
        return holder_installer(
            installer::get()
          , installer::class_(backend_.id(), x)
          , installer::sizeof_()
        );
    }

    // Convert X using the callback from the registry and
    // possibly a generated holder-installer.
    template<class U>
    void* operator()(U const& x) const
    //                 ^^^^^ what about auto_ptr<>?
    {
        return callback(
            classes::aux::arg_ptr(x)
          , make_installer(
                typename has_installer::type()
              , x
            )
        );
    }

    // JHM: I decided to pass auto_ptr<>s by value here.  This may 
    //      not be the right thing to do.  This works, but it moves 
    //      ownership of the "pointee" inside of the call.  However, 
    //      this is what's usually expected, and the installer will 
    //      move ownership anyway if invoked.
    //
    //      Passing by reference doesn't work, because an auto_ptr<> 
    //      returned from a function is usually an rvalue, and a non-
    //      const reference can't be bound to an rvalue.
    template <class U>
    void* operator()(std::auto_ptr<U> x) const
    {
        return callback(
            &x
          , make_installer(
                typename has_installer::type()
              , x
            )
        );
    }

private:
    converter::to_xxx_function callback;
    backend::plugin const& backend_;
};

template<>
struct result_converter<void>
{
    result_converter(converter::to_xxx_function, backend::plugin const&)
    {
    }

    void* operator()() const
    {
        return 0;
    }
};

}}} // namespace boost::langbinding::function

#endif // RESULT_CONVERTER_040925_HPP

