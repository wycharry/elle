//
// ---------- header ----------------------------------------------------------
//
// project       pig
//
// license       infinit
//
// file          /home/mycure/infinit/pig/Janitor.hh
//
// created       julien quintard   [wed sep  7 12:37:56 2011]
// updated       julien quintard   [wed sep  7 12:58:56 2011]
//

#ifndef PIG_JANITOR_HH
#define PIG_JANOTIR_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>
#include <etoile/Etoile.hh>

namespace pig
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// the janitor is responsible for discarding the identifiers it is
  /// given.
  ///
  /// the janitor is called whenever an unexpected error occurs in the
  /// crux.
  ///
  class Janitor
  {
  public:
    //
    // static methods
    //
    static elle::Void	Clear();

    template <typename T>
    static elle::Void	Clear(T&);
    template <typename T,
	      typename... TT>
    static elle::Void	Clear(T&,
			      TT&...);
  };

}

//
// ---------- templates -------------------------------------------------------
//

#include <pig/Janitor.hxx>

#endif
