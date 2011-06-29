//
// ---------- header ----------------------------------------------------------
//
// project       8diary
//
// license       infinit
//
// file          /home/mycure/infinit/applications/8diary/Diary.cc
//
// created       julien quintard   [mon jun 27 22:38:51 2011]
// updated       julien quintard   [wed jun 29 18:03:43 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <applications/8diary/Diary.hh>
#include <applications/8diary/Crux.hh>

#include <pig/PIG.hh>

namespace application
{

//
// ---------- definitions -----------------------------------------------------
//

  ///
  /// this value defines the component's name.
  ///
  const elle::Character		Component[] = "8diary";

//
// ---------- functions -------------------------------------------------------
//

  ///
  /// the main function.
  ///
  elle::Status		Main(elle::Natural32			argc,
			     elle::Character*			argv[])
  {
    Diary::Operation	operation;

    enter(instance(Infinit::Parser));

    // initialize the Elle library.
    if (elle::Elle::Initialize() == elle::StatusError)
      escape("unable to initialize Elle");

    // initialize the nucleus library.
    if (nucleus::Nucleus::Initialize() == elle::StatusError)
      escape("unable to initialize Nucleus");

    // initialize the Lune library.
    if (lune::Lune::Initialize() == elle::StatusError)
      escape("unable to initialize Lune");

    // initialize Infinit.
    if (Infinit::Initialize() == elle::StatusError)
      escape("unable to initialize Infinit");

    // initialize the Etoile library.
    if (etoile::Etoile::Initialize() == elle::StatusError)
      escape("unable to initialize Etoile");

    // initialize the operation.
    operation = Diary::OperationUnknown;

    // set up the program.
    if (elle::Program::Setup() == elle::StatusError)
      escape("unable to set up the program");

    // allocate a new parser.
    Infinit::Parser = new elle::Parser(argc, argv);

    // specify a program description.
    if (Infinit::Parser->Description(Infinit::Copyright) == elle::StatusError)
      escape("unable to set the description");

    // register the options.
    if (Infinit::Parser->Register(
          "Help",
	  'h',
	  "help",
	  "display the help",
	  elle::Parser::KindNone) == elle::StatusError)
      escape("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Record",
	  'c',
	  "record",
	  "activate the event recording",
	  elle::Parser::KindNone) == elle::StatusError)
      escape("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Replay",
	  'y',
	  "replay",
	  "activate the event replaying",
	  elle::Parser::KindNone) == elle::StatusError)
      escape("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Path",
	  'p',
	  "path",
	  "specify the path to the diary file to be recording or replayed",
	  elle::Parser::KindRequired) == elle::StatusError)
      escape("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Mountpoint",
	  'm',
	  "mounpoint",
	  "specify the path to the file system mounpoint",
	  elle::Parser::KindRequired) == elle::StatusError)
      escape("unable to register the option");

    // register the options.
    if (Infinit::Parser->Register(
          "Mirror",
	  'i',
	  "mirror",
	  "specify the path to the directory which must be mirrored through "
	  "the mounpoint",
	  elle::Parser::KindRequired) == elle::StatusError)
      escape("unable to register the option");

    // parse.
    if (Infinit::Parser->Parse() == elle::StatusError)
      escape("unable to parse the command line");

    // test the option.
    if (Infinit::Parser->Test("Help") == elle::StatusTrue)
      {
	// display the usage.
	Infinit::Parser->Usage();

	// quit.
	leave();
      }

    // check the mutually exclusive options.
    if ((Infinit::Parser->Test("Record") == elle::StatusTrue) &&
	(Infinit::Parser->Test("Replay") == elle::StatusTrue))
      {
	// display the usage.
	Infinit::Parser->Usage();

	escape("the record and replay options are mutually exclusive");
      }

    // test the option.
    if (Infinit::Parser->Test("Record") == elle::StatusTrue)
      operation = Diary::OperationRecord;

    // test the option.
    if (Infinit::Parser->Test("Replay") == elle::StatusTrue)
      operation = Diary::OperationReplay;

    // trigger a command.
    switch (operation)
      {
      case Diary::OperationRecord:
	{
	  elle::String		mountpoint;
	  elle::String		mirror;
	  pig::Diary		diary;
	  elle::String		string;
	  elle::Path		path;

	  // retrieve the mirror.
	  if (Infinit::Parser->Value("Path",
				     string) == elle::StatusError)
	    escape("unable to retrieve the path value");

	  // create the path.
	  if (path.Create(string) == elle::StatusError)
	    escape("unable to create the path");

	  // retrieve the mountpoint.
	  if (Infinit::Parser->Value("Mountpoint",
				     mountpoint) == elle::StatusError)
	    escape("unable to retrieve the mountpoint value");

	  // retrieve the mirror.
	  if (Infinit::Parser->Value("Mirror",
				     mirror) == elle::StatusError)
	    escape("unable to retrieve the mirror value");

	  // set up the crux.
	  if (Crux::Setup(mirror) == elle::StatusError)
	    escape("unable to set up the crux");

	  // set up the diary.
	  if (diary.Setup(Crux::Operations) == elle::StatusError)
	    escape("unable to set up the diary");

	  // record the diary.
	  if (diary.Record(mountpoint) == elle::StatusError)
	    escape("unable to record the diary");

	  // store the diary.
	  if (diary.Store(path) == elle::StatusError)
	    escape("unable to store the diary");

	  break;
	}
      case Diary::OperationReplay:
	{
	  // XXX

	  break;
	}
      case Diary::OperationUnknown:
      default:
	{
	  // display the usage.
	  Infinit::Parser->Usage();

	  escape("please specify an operation to perform");
	}
      }

    // clean the Etoile.
    if (etoile::Etoile::Clean() == elle::StatusError)
      escape("unable to clean Etoile");

    // clean Infinit.
    if (Infinit::Clean() == elle::StatusError)
      escape("unable to clean Infinit");

    // clean Lune
    if (lune::Lune::Clean() == elle::StatusError)
      escape("unable to clean Lune");

    // clean the nucleus library.
    if (nucleus::Nucleus::Clean() == elle::StatusError)
      escape("unable to clean Nucleus");

    // clean Elle.
    if (elle::Elle::Clean() == elle::StatusError)
      escape("unable to clean Elle");

    // delete the parser.
    delete Infinit::Parser;

    // waive.
    waive(Infinit::Parser);

    leave();
  }

}

//
// ---------- main ------------------------------------------------------------
//

///
/// this is the program entry point.
///
int			main(int				argc,
                             char**				argv)
{
  application::Main(argc, argv);

  expose();

  return (0);
}
