#pragma once

#include <algorithm>
#include <map>
#include <unordered_map>

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/cxx11/none_of.hpp>

namespace elle
{
  /// Whether \a e is in \a v.
  template <typename Container, typename T>
  bool
  contains(Container const& c, T const& e)
  {
    return ::boost::algorithm::any_of_equal(c, e);
  }

  /// Whether \a e is in \a v.
  template <typename... Args, typename T>
  bool
  contains(std::unordered_map<Args...> const& c, T const& e)
  {
    return c.find(e) != c.end();
  }

  /// Whether \a e is in \a v.
  template <typename... Args, typename T>
  bool
  contains(std::map<Args...> const& c, T const& e)
  {
    return c.find(e) != c.end();
  }

  /// Emplace back only if not already in the vector.
  ///
  /// The need for this function is somewhat a smell: wouldn't a set
  /// or a flat_set do better?
  template <typename Container, typename T>
  void
  push_back_if_missing(Container& c, T const& e)
  {
    if (::boost::algorithm::none_of_equal(c, e))
      c.push_back(e);
  }
}