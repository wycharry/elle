//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// file          /home/mycure/infinit/etoile/gear/Gear.hh
//
// created       julien quintard   [fri jun  3 10:58:40 2011]
// updated       julien quintard   [thu jun 16 09:39:01 2011]
//

#ifndef ETOILE_GEAR_GEAR_HH
#define ETOILE_GEAR_GEAR_HH

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/gear/Identifier.hh>

#include <elle/idiom/Close.hh>
# include <map>
#include <elle/idiom/Open.hh>

namespace etoile
{
  ///
  /// this namespace contains everything for recording operations on
  /// the file system so that such operations could be replayed for instance.
  ///
  namespace gear
  {

//
// ---------- forward declarations --------------------------------------------
//

    ///
    /// XXX
    ///
    template <typename T>
    class Scope;

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// XXX
    ///
    class Gear
    {
    public:
      //
      // types
      //
      typedef std::map<const Identifier,
		       elle::Base<Scope>*>		Container;
      typedef Container::iterator			Iterator;
      typedef Container::const_iterator			Scoutor;

      //
      // static methods
      //
      static elle::Status	Add(const Identifier&,
				    elle::Base<Scope>*);
      static elle::Status	Select(const Identifier&,
				       elle::Base<Scope>*&);
      static elle::Status	Remove(const Identifier&);

      //
      // static attributes
      //
      static Container		Scopes;
    };

  }
}

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/gear/Identifier.hh>
#include <etoile/gear/Scope.hh>

#endif
