/*
 *          Copyright Andrey Semashev 2007 - 2013.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   keywords/log_source.hpp
 * \author Andrey Semashev
 * \date   14.03.2009
 *
 * The header contains the \c log_source keyword declaration.
 */

#ifndef BOOST_LOG_KEYWORDS_LOG_SOURCE_HPP_INCLUDED_
#define BOOST_LOG_KEYWORDS_LOG_SOURCE_HPP_INCLUDED_

#include <boost/parameter/keyword.hpp>
#include <boost/log/detail/config.hpp>

#ifdef BOOST_LOG_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace boost {

BOOST_LOG_OPEN_NAMESPACE

namespace keywords {

//! The keyword is used to pass event log source name to a sink backend
BOOST_PARAMETER_KEYWORD(tag, log_source)

} // namespace keywords

BOOST_LOG_CLOSE_NAMESPACE // namespace log

} // namespace boost

#endif // BOOST_LOG_KEYWORDS_LOG_SOURCE_HPP_INCLUDED_
