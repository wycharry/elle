//
// ---------- header ----------------------------------------------------------
//
// project       facade
//
// license       infinit
//
// author        julien quintard   [tue may 31 10:31:35 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <facade/unix/UNIX.hh>
#include <facade/unix/Crux.hh>
#include <facade/Facade.hh>

#include <elle/Elle.hh>
#include <agent/Agent.hh>
#include <Infinit.hh>

namespace facade
{
  namespace unix
  {

//
// ---------- definitions -----------------------------------------------------
//

    ///
    /// this variable contains the UID of the 'somebody' user, user which
    /// is used whenever the system cannot map the Infinit user on a local
    /// user.
    ///
    uid_t                               UNIX::Somebody::UID;

    ///
    /// this variable contains the GID of the 'somebody' group.
    ///
    gid_t                               UNIX::Somebody::GID;

    ///
    /// this varaible contains the mappings between local user/group
    /// identities and Infinit identities.
    ///
    lune::Dictionary                    UNIX::Dictionary;

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method initializes UNIX.
    ///
    elle::Status        UNIX::Initialize()
    {
      enter();

      //
      // initialize the 'somebody' entity.
      //
      {
        struct ::passwd*        passwd;

        // retrieve the passwd structure related to the user 'somebody'.
        // if NULL, try to fallback to 'nobody'.
        if ((passwd = ::getpwnam("somebody")) == NULL &&
            (passwd = ::getpwnam("nobody")) == NULL)
          escape("it seems that the user 'somebody' does not exist");

        // set the uid and gid.
        UNIX::Somebody::UID = passwd->pw_uid;
        UNIX::Somebody::GID = passwd->pw_gid;
      }

      //
      // load the user/group maps which will be used to translate Infinit
      // user/group identifiers into local identifiers.
      //
      {
        // if the dictionary exist.
        if (UNIX::Dictionary.Exist() == elle::StatusTrue)
          {
            // load the dictionary file.
            if (UNIX::Dictionary.Load() == elle::StatusError)
              escape("unable to load the dictionary");
          }
      }

      //
      // initialize FUSE.
      //
      {
        struct ::fuse_operations        operations;

        // set all the pointers to zero.
        ::memset(&operations, 0x0, sizeof (::fuse_operations));

        // operations.statfs: not supported
        operations.getattr = Crux::Getattr;
        operations.fgetattr = Crux::Fgetattr;
        operations.utimens = Crux::Utimens;

        operations.opendir = Crux::Opendir;
        operations.readdir = Crux::Readdir;
        operations.releasedir = Crux::Releasedir;
        operations.mkdir = Crux::Mkdir;
        operations.rmdir = Crux::Rmdir;

        operations.access = Crux::Access;
        operations.chmod = Crux::Chmod;
        operations.chown = Crux::Chown;
#ifdef HAVE_SETXATTR
        operations.setxattr = Crux::Setxattr;
        operations.getxattr = Crux::Getxattr;
        operations.listxattr = Crux::Listxattr;
        operations.removexattr = Crux::Removexattr;
#endif

        // operations.link: not supported
        operations.readlink = Crux::Readlink;
        operations.symlink = Crux::Symlink;

        operations.create = Crux::Create;
        // operations.mknod: not supported
        operations.open = Crux::Open;
        operations.write = Crux::Write;
        operations.read = Crux::Read;
        operations.truncate = Crux::Truncate;
        operations.ftruncate = Crux::Ftruncate;
        operations.release = Crux::Release;

        operations.rename = Crux::Rename;
        operations.unlink = Crux::Unlink;

#if defined(INFINIT_UNIX)
        // the following flag being activated prevents the path argument
        // to be passed for functions which take a file descriptor.
        operations.flag_nullpath_ok = 1;
#elif defined(INFINIT_MACOSX)
	// nothing to do.
#endif

        if (FUSE::Initialize(operations) == elle::StatusError)
          escape("unable to initialize FUSE");
      }

      leave();
    }

    ///
    /// this method sets up the UNIX implementation.
    ///
    elle::Status        UNIX::Setup()
    {
      enter();

      // set up FUSE.
      if (FUSE::Setup(Infinit::Mountpoint) == elle::StatusError)
        escape("unable to set up FUSE");

      leave();
    }

    ///
    /// this method cleans UNIX.
    ///
    elle::Status        UNIX::Clean()
    {
      enter();

      // clean FUSE.
      if (FUSE::Clean() == elle::StatusError)
        escape("unable to clean FUSE");

      leave();
    }

  }
}
