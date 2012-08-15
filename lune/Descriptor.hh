#ifndef LUNE_DESCRIPTOR_HH
# define LUNE_DESCRIPTOR_HH

# include <elle/serialize/fwd.hh>
# include <elle/cryptography/Signature.hh>
# include <elle/idiom/Open.hh>

# include <lune/fwd.hh>

# include <hole/Model.hh>
# include <hole/fwd.hh>

# include <nucleus/proton/Address.hh>
# include <nucleus/neutron/Group.hh>

# include <horizon/Policy.hh>

namespace lune
{

  ///
  /// this class represents a network descriptor.
  ///
  /// note that the network name is supposed to be unique as it plays the
  /// role of identifier.
  ///
  class Descriptor:
    public elle::radix::Object,
    public elle::concept::MakeFileable<Descriptor>,
    public elle::concept::MakeUniquable<Descriptor>
  {
  public:
    //
    // constants
    //
    static const elle::String           Extension;

    static const elle::Boolean          History;
    static const elle::Natural32        Extent;
    static const elle::Real             Contention;
    static const elle::Real             Balancing;

    //
    // methods
    //
    elle::Status
    Create(const elle::String id,
           const elle::String&,
           const hole::Model&,
           hole::Openness const& openness,
           const nucleus::proton::Address& root,
           nucleus::neutron::Group::Identity const& everybody,
           const elle::Boolean,
           const elle::Natural32,
           const elle::Real&,
           const elle::Real&,
           horizon::Policy const& policy);

    elle::Status        Seal(const Authority&);
    elle::Status        Validate(const Authority&) const;

    /// XXX
    elle::String const&
    id() const;
    /// XXX[is the setter really used? this is probably abnormal]
    void
    id(elle::String const& id);
    /// XXX
    hole::Openness const&
    openness() const;
    /// XXX
    horizon::Policy const&
    policy() const;

    //
    // interfaces
    //

    // object
    declare(Descriptor);

    // dumpable
    elle::Status        Dump(const elle::Natural32 = 0) const;

    // serializable
    ELLE_SERIALIZE_FRIEND_FOR(Descriptor);

    // fileable
    ELLE_CONCEPT_FILEABLE_METHODS();

    elle::Status        Load(const elle::String&);
    elle::Status        Store(const elle::String&) const;
    elle::Status        Erase(const elle::String&) const;
    elle::Status        Exist(const elle::String&) const;

    //
    // attributes
    //
  private:
    elle::String        _id;

  public: // XXX
    elle::String        name;
    hole::Model         model;
    hole::Openness _openness;
    nucleus::proton::Address root;
    nucleus::neutron::Group::Identity everybody;
    elle::Boolean       history;
    elle::Natural32     extent;
    elle::Real          contention;
    elle::Real          balancing;
    horizon::Policy _policy;
    elle::cryptography::Signature     signature;
  };

}

#include <lune/Descriptor.hxx>

#endif
