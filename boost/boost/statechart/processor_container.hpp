#ifndef BOOST_FSM_PROCESSOR_CONTAINER_HPP_INCLUDED
#define BOOST_FSM_PROCESSOR_CONTAINER_HPP_INCLUDED
//////////////////////////////////////////////////////////////////////////////
// Copyright (c) Andreas Huber Doenni 2002-2004.
// Use, modification and distribution are subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <boost/fsm/event_base.hpp>
#include <boost/fsm/event_processor.hpp>

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function/function0.hpp>
#include <boost/bind.hpp>

#include <set>
#include <memory>   // std::allocator, std::auto_ptr



namespace boost
{
namespace fsm
{



template< class Scheduler, class Allocator = std::allocator< void > >
class processor_container : noncopyable
{
  typedef event_processor< Scheduler > processor_base_type;
  typedef std::auto_ptr< processor_base_type > processor_holder_type;
  typedef shared_ptr< processor_holder_type > processor_holder_ptr_type;
  public:
    //////////////////////////////////////////////////////////////////////////
    typedef weak_ptr< processor_holder_type > processor_handle;
    typedef function0< void, Allocator > work_item;

    class processor_context
    {
      processor_context(
        Scheduler & scheduler, const processor_handle & handle
      ) :
        scheduler_( scheduler ),
        handle_( handle )
      {
      }

      Scheduler & my_scheduler() const { return scheduler_; }
      const processor_handle & my_handle() const { return handle_; }

      Scheduler & scheduler_;
      const processor_handle handle_;

      friend class processor_container;
      friend class processor_base_type;
    };

    template< class Processor >
    work_item create_processor( processor_handle & handle, Scheduler & scheduler )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &, const processor_context & );
      impl_fun_ptr pImpl =
        &processor_container::create_processor_impl0< Processor >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ) );
    }

    template< class Processor, typename Arg1 >
    work_item create_processor(
      processor_handle & handle, Scheduler & scheduler, Arg1 arg1 )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &, const processor_context &, Arg1 );
      impl_fun_ptr pImpl =
        &processor_container::create_processor_impl1< Processor, Arg1 >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ), arg1 );
    }

    template< class Processor, typename Arg1, typename Arg2 >
    work_item create_processor(
      processor_handle & handle, Scheduler & scheduler, Arg1 arg1, Arg2 arg2 )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &,
        const processor_context &, Arg1, Arg2 );
      impl_fun_ptr pImpl =
        &processor_container::create_processor_impl2< Processor, Arg1, Arg2 >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ),
        arg1, arg2 );
    }

    template< class Processor, typename Arg1, typename Arg2, typename Arg3 >
    work_item create_processor(
      processor_handle & handle, Scheduler & scheduler,
      Arg1 arg1, Arg2 arg2, Arg3 arg3 )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &,
        const processor_context &,
        Arg1, Arg2, Arg3 );
      impl_fun_ptr pImpl = &processor_container::create_processor_impl3<
        Processor, Arg1, Arg2, Arg3 >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ),
        arg1, arg2, arg3 );
    }

    template<
      class Processor, typename Arg1, typename Arg2,
      typename Arg3, typename Arg4 >
    work_item create_processor(
      processor_handle & handle, Scheduler & scheduler,
      Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &,
        const processor_context &,
        Arg1, Arg2, Arg3, Arg4 );
      impl_fun_ptr pImpl = &processor_container::create_processor_impl4<
        Processor, Arg1, Arg2, Arg3, Arg4 >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ),
        arg1, arg2, arg3, arg4 );
    }

    template<
      class Processor, typename Arg1, typename Arg2,
      typename Arg3, typename Arg4, typename Arg5 >
    work_item create_processor(
      processor_handle & handle, Scheduler & scheduler,
      Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5 )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &,
        const processor_context &,
        Arg1, Arg2, Arg3, Arg4, Arg5 );
      impl_fun_ptr pImpl = &processor_container::create_processor_impl5<
        Processor, Arg1, Arg2, Arg3, Arg4, Arg5 >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ),
        arg1, arg2, arg3, arg4, arg5 );
    }

    template<
      class Processor, typename Arg1, typename Arg2,
      typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
    work_item create_processor(
      processor_handle & handle, Scheduler & scheduler,
      Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6 )
    {
      processor_holder_ptr_type pProcessor = make_processor_holder();
      handle = pProcessor;
      typedef void ( processor_container::*impl_fun_ptr )(
        const processor_holder_ptr_type &,
        const processor_context &,
        Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 );
      impl_fun_ptr pImpl = &processor_container::create_processor_impl6<
        Processor, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6 >;
      return bind(
        pImpl, this, pProcessor, processor_context( scheduler, handle ),
        arg1, arg2, arg3, arg4, arg5, arg6 );
    }

    work_item destroy_processor( const processor_handle & processor )
    {
      return bind(
        &processor_container::destroy_processor_impl, this, processor );
    }

    work_item initiate_processor( const processor_handle & processor )
    {
      return bind(
        &processor_container::initiate_processor_impl, this, processor );
    }

    work_item terminate_processor( const processor_handle & processor )
    {
      return bind(
        &processor_container::terminate_processor_impl, this, processor );
    }

    typedef intrusive_ptr< const event_base > event_ptr_type;

    work_item queue_event(
      const processor_handle & processor, const event_ptr_type & pEvent )
    {
      BOOST_ASSERT( pEvent.get() != 0 );

      return bind(
        &processor_container::queue_event_impl, this, processor, pEvent );
    }

  private:
    //////////////////////////////////////////////////////////////////////////
    processor_holder_ptr_type make_processor_holder()
    {
      return processor_holder_ptr_type( new processor_holder_type() );
    }

    template< class Processor >
    void create_processor_impl0(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset( new Processor( context ) );
    }

    template< class Processor, typename Arg1 >
    void create_processor_impl1(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context, Arg1 arg1 )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset( new Processor( context, arg1 ) );
    }

    template< class Processor, typename Arg1, typename Arg2 >
    void create_processor_impl2(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context, Arg1 arg1, Arg2 arg2 )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset( new Processor( context, arg1, arg2 ) );
    }

    template< class Processor, typename Arg1, typename Arg2, typename Arg3 >
    void create_processor_impl3(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context, Arg1 arg1, Arg2 arg2, Arg3 arg3 )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset( new Processor( context, arg1, arg2, arg3 ) );
    }

    template<
      class Processor, typename Arg1, typename Arg2,
      typename Arg3, typename Arg4 >
    void create_processor_impl4(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context,
      Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4 )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset( new Processor( context, arg1, arg2, arg3, arg4 ) );
    }

    template<
      class Processor, typename Arg1, typename Arg2,
      typename Arg3, typename Arg4, typename Arg5 >
    void create_processor_impl5(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context,
      Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5 )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset(
        new Processor( context, arg1, arg2, arg3, arg4, arg5 ) );
    }

    template<
      class Processor, typename Arg1, typename Arg2,
      typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
    void create_processor_impl6(
      const processor_holder_ptr_type & pProcessor,
      const processor_context & context,
      Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6 )
    {
      processorSet_.insert( pProcessor );
      pProcessor->reset(
        new Processor( context, arg1, arg2, arg3, arg4, arg5, arg6 ) );
    }

    void destroy_processor_impl( const processor_handle & processor )
    {
      const processor_holder_ptr_type pProcessor = processor.lock();

      if ( pProcessor != 0 )
      {
        processorSet_.erase( pProcessor );
      }
    }

    void initiate_processor_impl( const processor_handle & processor )
    {
      const processor_holder_ptr_type pProcessor = processor.lock();

      if ( pProcessor != 0 )
      {
        ( *pProcessor )->initiate();
      }
    }

    void terminate_processor_impl( const processor_handle & processor )
    {
      const processor_holder_ptr_type pProcessor = processor.lock();

      if ( pProcessor != 0 )
      {
        ( *pProcessor )->terminate();
      }
    }

    void queue_event_impl(
      const processor_handle & processor, const event_ptr_type & pEvent )
    {
      const processor_holder_ptr_type pProcessor = processor.lock();

      if ( pProcessor != 0 )
      {
        ( *pProcessor )->process_event( *pEvent );
      }
    }

    typedef std::set< 
      processor_holder_ptr_type, 
      std::less< processor_holder_ptr_type >,
      typename Allocator::template rebind< processor_holder_ptr_type >::other
    > event_processor_set_type;

    event_processor_set_type processorSet_;
};



} // namespace fsm
} // namespace boost



#endif
