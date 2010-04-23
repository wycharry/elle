//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// file          /home/mycure/infinit/etoile/path/Venue.cc
//
// created       julien quintard   [sat aug  8 17:51:22 2009]
// updated       julien quintard   [wed apr 14 12:55:52 2010]
//

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/path/Venue.hh>

namespace etoile
{
  namespace path
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// this constant is used a null definition.
    ///
    const Venue			Venue::Null;

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method records the next step of the venue.
    ///
    Status		Venue::Record(const hole::Address&	address)
    {
      enter();

      // store the address in the container.
      this->elements.push_back(address);

      leave();
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps a venue.
    ///
    Status		Venue::Dump(const Natural32		margin) const
    {
      String		alignment(margin, ' ');
      Venue::Scoutor	scoutor;

      enter();

      std::cout << alignment << "[Venue]" << std::endl;

      // for every element.
      for (scoutor = this->elements.begin();
	   scoutor != this->elements.end();
	   scoutor++)
	if (scoutor->Dump(margin + 2) == StatusError)
	  escape("unable to dump the address");

      leave();
    }

//
// ---------- entity ----------------------------------------------------------
//

    ///
    /// this operator compares two objects.
    ///
    Boolean		Venue::operator==(const Venue&		element) const
    {
      Venue::Scoutor	s;
      Venue::Scoutor	t;

      enter();

      // check the address as this may actually be the same object.
      if (this == &element)
	true();

      // compare the size.
      if (this->elements.size() != element.elements.size())
	false();

      // for every element.
      for (s = this->elements.begin(), t = element.elements.begin();
	   s != this->elements.end();
	   s++, t++)
	if (*s != *t)
	  false();

      true();
    }

    ///
    /// this macro-function call generates the entity.
    ///
    embed(Entity, Venue);

  }
}
