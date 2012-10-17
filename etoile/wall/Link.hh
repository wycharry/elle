#ifndef ETOILE_WALL_LINK_HH
# define ETOILE_WALL_LINK_HH

# include <elle/types.hh>

# include <etoile/path/fwd.hh>
# include <etoile/gear/fwd.hh>

namespace etoile
{
  namespace wall
  {

    ///
    /// this class provides functionalities for managing links.
    ///
    class Link
    {
    public:
      //
      // static methods
      //
      static elle::Status       Create(gear::Identifier&);
      static elle::Status       Load(const path::Chemin&,
                                     gear::Identifier&);

      static elle::Status       Lock(const gear::Identifier&);
      static elle::Status       Release(const gear::Identifier&);
      static elle::Status       Bind(const gear::Identifier&,
                                     const path::Way&);
      static elle::Status       Resolve(const gear::Identifier&,
                                        path::Way&);

      static elle::Status       Discard(const gear::Identifier&);

      /// Commit the pending modifications by placing the scope in the journal.
      static
      void
      store(gear::Identifier const& identifier);

      static elle::Status       Destroy(const gear::Identifier&);
      static elle::Status       Purge(const gear::Identifier&);
    };

  }
}

#endif
