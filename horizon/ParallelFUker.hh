//
// ---------- header ----------------------------------------------------------
//
// project       pig
//
// license       infinit
//
// file          /home/mycure/infinit/pig/ParallelFUker.hh
//
// created       julien quintard   [tue jul 26 15:33:15 2011]
// updated       julien quintard   [thu sep  8 07:49:50 2011]
//

#ifndef PIG_PARALLELFUKER_HH
#define PIG_PARALLELFUKER_HH

//
// ---------- macros ----------------------------------------------------------
//

#ifndef FUSE_USE_VERSION
# define FUSE_USE_VERSION		26
#endif

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

#include <pig/FUker.hh>

namespace pig
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// XXX to implement.
  ///
  class ParallelFUker:
    public FUker
  {
  public:
    //
    // constructors & destructors
    //
    ParallelFUker();
    ~ParallelFUker();

    //
    // interfaces
    //

    // fuker
    elle::Status	Setup();

    //
    // callbacks
    //
    elle::Status	Event(const elle::Natural16&);
  };

}

#endif
