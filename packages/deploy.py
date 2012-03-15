#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

"""Deployment script.
"""

import argparse
import os
import sys
import libpkg

parser = argparse.ArgumentParser(description="Deploy infinit packages")
parser.add_argument(
    "--match", '-m',
    help="filter with a simple pattern",
    action="append"
)
parser.add_argument(
    '--last',
    help="Install the last tarball in the results",
    action='store_true',
    default=False
)
parser.add_argument(
    '--print', '-p',
    help="Only print matching releases",
    action='store_true',
    default=False
)
parser.add_argument(
    '--yes', '-y',
    help="Assume yes to all question",
    action='store_true',
    default=False,
)
parser.add_argument(
    '--local',
    help="Use local builds",
    action='store_true',
    default=False
)
parser.add_argument(
    '--local-client',
    help="Use local client builds",
    action='store_true',
    default=False
)
parser.add_argument(
    '--local-server',
    help="Use local server builds",
    action='store_true',
    default=False
)
parser.add_argument(
    '--dest-dir', '-d',
    help="Set destination directory for built packages",
    action='store',
    default='.'
)


def dumpReleases(releases):
    for hash, tarballs in releases.items():
        print('-', hash, ':', tarballs)

def yesno(s, default=False):
    res = None
    if default:
        s += ' (Y/n): '
    else:
        s += ' (y/N): '
    while res is None:
        v = input(s).strip()
        if not v: return default
        elif v in ['y', 'Y', 'o', 'O']:
            return True
        elif v in ['n', 'N']:
            return False


#def deployClientTarball(tarball, platform, arch):
#    os.system('scp %s oracle@infinit.im:www/infinit.im/downloads' % tarball)
#    cmd = ' && '.join([
#        "cd www/infinit.im/downloads",
#        "mkdir -p %(dir)s",
#        "tar --extract --file=%(tarball)s --strip-components=1 --directory=%(dir)s",
#        "rm %(tarball)s",
#        "chmod -R a+rX %(dir)s",
#    ]) % {
#        'dir': platform + arch,
#        'tarball': tarball,
#    }
#    os.system('ssh oracle@infinit.im "%s"' % cmd)
#
#def deployServerTarball(tarball):
#    os.system('scp  %s oracle@infinit.im:www/infinit.im/' % tarball)
#    cmd = ' && '.join([
#        "cd www/infinit.im",
#        "tar --extract --file=%(tarball)s --strip-components=1 --directory=.",
#        "rm %(tarball)s",
#    ]) % {
#        'tarball': tarball,
#    }
#    os.system('ssh oracle@infinit.im "%s"' % cmd)
#
#
def deployTarball(tarball):
    print("Deploying", tarball)
    if 'x86_64' in tarball:
        arch = '64'
    else:
        arch = '32'

    if 'linux' in tarball:
        platform = 'linux'
    else:
        raise Exception("CHECK FOR OTHER PLATFORM")

    if tarball.startswith("infinit-server"):
        assert 'x86_64' in tarball
        assert 'linux' in tarball
        deployServerTarball(tarball)
    if tarball.startswith("infinit-client"):
        deployClientTarball(tarball, platform, arch)
    else:
        raise Exception("Unknown tarball type!")
    os.system('rm -rf "%s"' % tarball)

def deployPackage(path):
    if path.endswith('.tbz'):
        deployTarball(path)

def getFarmBuild(infos, args):
    if args.last:
        tarballs = libpkg.farm.getLastTarballs(args.match)
    else:
        tarballs = libpkg.farm.getTarballs(args.match)

    releases = {}
    for t in tarballs:
        h = libpkg.farm.getTarballHash(t)
        releases.setdefault(h, [])
        releases[h].append(t)


    to_install = None
    if not len(releases):
        print("No release found")
    elif len(releases) == 1:
        to_install = list(releases.keys())[0]
    else:
        print("More than one release match:")
        dumpReleases(releases)
        print("Use --match with a pattern or --last")

    if not to_install:
        sys.exit(not args.print and 1 or 0)

    return libpkg.FarmBuild(infos, to_install, releases[to_install])

def preparePackages(args, build, packagers):
    if not os.path.exists(args.dest_dir):
        os.mkdir(args.dest_dir)
    assert os.path.isdir(args.dest_dir)
    packages = []
    if build.has_client:
        for packager in packagers:
            packages.extend(
                packager.buildClientPackages(build, args.dest_dir)
            )
    return packages

if __name__ == '__main__':
    args = parser.parse_args()

    infos = {
        'version': '0.2',
        'version_name': 'alpha'
    }

    if args.local:
        args.local_client = True
        args.local_server = True
    if args.local_client or args.local_server:
        build = libpkg.LocalBuild(infos, os.path.join(os.path.dirname(__file__), '../build'))
    else:
        build = getFarmBuild(infos, args)


    print("Selected build (", build.hash, "):")
    print("\t- Server build:", build.has_server)
    print("\t- Client build:", build.has_client)
    print("\t- Architecture(s):", ', '.join(build.architectures_strings))
    print("\t- Platform(s):", ', '.join(build.platforms_strings))
    print("\t- Status:", build.is_available and "Working" or "Not working")


    print()

    packagers = []
    print("Packagers found:")
    for packager_cls in libpkg.PACKAGERS:
        packager = packager_cls(build.architectures, build.platforms)
        print("\t-", packager.name,':', packager.status)
        if packager.is_available:
            packagers.append(packager)

    if args.print:
        sys.exit(0)

    if not packagers:
        print("No packager available")
        sys.exit(1)

    if not (args.yes or yesno("Proceed ?", True)):
        sys.exit(1)

    with build:
        packages = preparePackages(args, build, packagers)

    print()
    print("Built packages:")
    for package in packages:
        print('\t-', package)

    if not (args.yes or yesno("Deploy these packages ?", True)):
        sys.exit(1)

    #for package in packages:
    #    path = os.path.join(args.dest_dir, package)
    #    print("Deploying `%s':" % path)
    #    deployPackage(path)
