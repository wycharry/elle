#include <etoile/gear/Object.hh>
#include <etoile/gear/Nature.hh>

#include <nucleus/neutron/Role.hh>

#include <elle/idiom/Open.hh>

namespace etoile
{
  namespace gear
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// the default constructor.
    ///
    Object::Object():
      Context(NatureObject),

      access(NULL)
    {
      // initialize the rights structure.
      this->rights.role = nucleus::neutron::RoleUnknown;
      this->rights.permissions = nucleus::neutron::PermissionNone;
    }

    ///
    /// the nature-specific constructor.
    ///
    Object::Object(const Nature                                 nature):
      Context(nature),

      access(NULL)
    {
      // initialize the rights structure.
      this->rights.role = nucleus::neutron::RoleUnknown;
      this->rights.permissions = nucleus::neutron::PermissionNone;
    }

    ///
    /// the destructor.
    ///
    Object::~Object()
    {
      // delete the access.
      if (this->access != NULL)
        delete this->access;
    }

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the object.
    ///
    elle::Status        Object::Dump(const elle::Natural32      margin) const
    {
      elle::String      alignment(margin, ' ');

      std::cout << alignment << "[Object]" << std::endl;

      // dump the parent context.
      if (Context::Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the parent context");

      // dump the location.
      if (this->location.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the location");

      // dump the object.
      if (this->object.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the object");

      // dump the access, if present.
      if (this->access != NULL)
        {
          // dump the access block.
          if (this->access->Dump(margin + 2) == elle::Status::Error)
            escape("unable to dump the access");
        }
      else
        {
          // dump none.
          std::cout << alignment << elle::io::Dumpable::Shift
                    << "[Access] " << elle::none << std::endl;
        }

      // dump the rights.
      std::cout << alignment << elle::io::Dumpable::Shift
                << "[Rights]" << std::endl;

      // dump the role.
      std::cout << alignment << elle::io::Dumpable::Shift
                << elle::io::Dumpable::Shift
                << "[Role] "
                << this->rights.role << std::endl;

      // dump the permissions.
      std::cout << alignment << elle::io::Dumpable::Shift
                << elle::io::Dumpable::Shift
                << "[Permissions] "
                << (int)this->rights.permissions
                << std::endl;

      // dump the key.
      if (this->rights.key.Dump(margin + 4) == elle::Status::Error)
        escape("unable to dump the key");

      // dump the record.
      if (this->rights.record.Dump(margin + 4) == elle::Status::Error)
        escape("unable to dump the record");

      // dump the author.
      if (this->author.Dump(margin + 2) == elle::Status::Error)
        escape("unable to dump the author");

      return elle::Status::Ok;
    }

  }
}
