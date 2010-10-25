/* $Id: metadefs.h 3186 2006-02-15 18:17:33Z slbrow $
 * $Log$
 * Revision 1.1  2006/02/15 18:14:43  slbrow
 * This is the commit of the DAKOTA 'autoconfiscation' I've been working on, which
 * enables builds using the GNU Autotools.  I have been merging in changes to the
 * mainline the entire time I've been working on these changes to my working copy
 * (in ~slbrow/projects/Dakota), so none of your changes to DAKOTA will be lost.
 * In addition, the final pre-autotools version of DAKOTA has been tagged with:
 *
 * Version-3-3-Plus-pre-autotools
 *
 * The GNU autotools will be needed by all developers who check DAKOTA out from
 * CVS with intentions to build; this is because the version of DAKOTA in the
 * repository after this commit does not have any configure or Makefile.in files
 * (since they are now machine-generated).  All the source tarballs rolled for
 * external consumption (both release and votd) will automatically include all the
 * configure and Makefile.in files our non-autotools savvy users will need, while
 * still omitting those proprietary sources we may not distribute (DOT, NPSOL).
 *
 * To re-generate the missing configure and Makefile.in files in a checkout of
 * DAKOTA, only one GNU Autotools script is necessary to run from the
 * command-line:
 *
 * $ autoreconf --install
 *
 * This script (and all the scripts it runs in sequence on a copy of DAKOTA
 * checked out of the repository) can be found on the SCICO LAN in the directory:
 *
 * /usr/netpub/autotools/bin
 *
 * This directory exists on all five of the DAKOTA nightly build platforms (AIX,
 * IRIX, Linux, Solaris and Tru64), and should be added to the beginning of your
 * $PATH if you are on the SCICO LAN.  If you are not, the versions of the tools
 * you need can be easily downloaded from the GNU website.  The versions we use:
 *
 * autoconf 2.59  (http://ftp.gnu.org/gnu/autoconf/autoconf-2.59.tar.gz)
 * automake 1.9.6 (http://ftp.gnu.org/gnu/automake/automake-1.9.6.tar.gz)
 * libtool 1.5.22 (http://ftp.gnu.org/gnu/libtool/libtool-1.5.22.tar.gz)
 * m4 1.4.3       (http://ftp.gnu.org/gnu/m4/m4-1.4.3.tar.gz)
 *
 * Here is a "short" summary of the major changes made in this )commit:
 *
 * 1. All configure.in and Makefile.in files are no longer in the CVS repository.
 *    They are replaced by configure.ac and Makefile.am files, respectively.  When
 *    you run autoreconf, the configure and Makefile.in files are regenerated
 *    automatically and recursively.  No $DAKOTA environment variable needs to be
 *    set in order to build DAKOTA.
 *
 * 2. Build directories as they were in DAKOTA using the Cygnus configure system
 *    no longer exist.  Executables built by the GNU Autotools (e.g., the dakota*
 *    binaries which were built in Dakota/src/i686-unknown-linux on Linux) are now
 *    built in Dakota/src; the autotools provide deployment targets for installing
 *    binaries and libraries in a deployment directory, as well as a simple means
 *    of maintaining more than one build of Dakota for multiple platforms (i.e.,
 *    using VPATH builds and source- and build-tree separation).
 *
 *    Suppose you have in your home directory a checked-out and 'autoreconf'd copy
 *    of Dakota, and you want to simultaneously build versions of DAKOTA for Linux
 *    and Solaris.  You would create two uniquely named directories in your home
 *    directory, called 'my_linux_build' and 'my_solaris_build', respectively.
 *    Assuming two terminals are open on both machines, you would 'cd' into the
 *    appropriate build directory on each platform and run this command on both:
 *
 *    $ ../Dakota/configure -C
 *
 *    This will create a directory tree which is identical to the source tree, but
 *    containing only Makefiles.  Then, when you run 'make', it will traverse the
 *    build tree, referencing the sources by relative paths to create objects and
 *    binaries.  At your option, you can then delete these build directories
 *    without touching your source tree (the 'autoreconf'd checkout of Dakota).  I
 *    should note here that the GNU Autotools do not _force_ this separation of
 *    trees; you can do a 'make distclean' after building for each platform in
 *    your 'autoreconf'd copy of Dakota with no problems.
 *
 * 3. All the configure flags which were supported in the old build system (i.e.,
 *    --without-acro, --enable-debugging, etc.), are supported in the new system
 *    with the following changes and additions:
 *
 *    --with-mpi/--without-mpi           -> --enable-mpi/--disable-mpi
 *    --with-graphics/--without-graphics -> --enable-graphics/--disable-graphics
 *    --enable-docs (default is off)
 *
 *    I should note that all the default settings have been preserved.  Further,
 *    since ACRO, OPT++ and DDACE haven't been integrated yet (see item 9), their
 *    flags are still commented out in 'Dakota/config/dak_vendoroptimizers.m4'.
 *
 * 4. The epetra and plplot directories under VendorPackages have been completely
 *    replaced with the versions available online (versions 3.3 and 5.5.2,
 *    respectively), which were originally flattened to provide support for our
 *    build system.  Epetra has been altered from the version distributed by the
 *    Trilinos team to provide full libtool support as that is necessary for
 *    building mixed-language archives under Solaris, while plplot has had all
 *    language-bindings except C++ turned off by default.  Additionally, GSL has
 *    been updated from version 1.7 available online, but preserving the flattened
 *    structure it had prior to autoconfiscation.
 *
 * 5. All other packages in the DAKOTA repository under VendorPackages and
 *    VendorOptimizers have been 'autoconfiscated' separately from DAKOTA.  To
 *    avoid duplicated configure-time probes, run configure from the top-level
 *    DAKOTA directory with the '-C' argument; this will cache the probe results
 *    from each subpackage, removing the need to do anything other than a lookup
 *    the next time the probe runs.
 *
 * 6. No 'mpi' symbolic link needs to be made from VendorPackages/mpi to the MPI
 *    implementation you wish to build against; MPI wrapper scripts for the C++
 *    compiler are used instead; if you wish to change the default choices for the
 *    MPI implementation to build against, you can specify where the wrapper
 *    scripts you wish to use are via environment variables, which may be set
 *    on separate lines (using setenv in CSH), or with env on the configure line:
 *
 *    $ env MPICC=mpicc MPICXX=mpiCC MPIF77=mpif77 ./configure -C
 *
 * 7. All the clean-up make targets ('clean', 'distclean' and a new one for those
 *    unfamiliar with the GNU Autotools, 'maintainer-clean') are now machine-
 *    generated.  The targets remove increasing numbers of files as you would
 *    expect; the last one removes all the autotools-generated files, returning
 *    you to a pristine CVS checkout.
 *
 * 8. Many changes have been made to the sources in Dakota/src:
 *
 *    A) Fortran calls are now done using FC_FUNC and F77_FUNC macro wrappers,
 *       removing the need for #ifdef RS6K preprocessor checks for underscores.
 *    B) Platform testing preprocessor conditionals have been replaced* with the
 *       feature that was assumed to be supported on that platform 'a priori'
 *       * mostly: MPI implementation choice is still done by platform.
 *         See ParallelLibrary.C for details.
 *    C) TFLOPS and COUGAR #ifdef's have been removed, leaving their #else
 *       branches to execute unconditionally (as those machines have gone away).
 *
 * 9. Since the GNU Autotools preferentially look for GCC compilers first; if you
 *    wish to build on Solaris with the Sun compilers you must set the compilers
 *    to use in environment variables in the same way the MPI wrappers must be set
 *    (see item 5 above):
 *
 *     $ env CC=cc CXX=CC F77=f77 FC=f90 ./configure ...
 *
 * Additionally, there are some minor problems to fix with the new build system:
 *
 * 10. This autoconfiscation covers Dakota, as integration with ACRO, OPT++ and
 *     DDACE has yet to be done.  I have integrated Surfpack and removed the
 *     separate ann and kriging packages from VendorPackages, based on positive
 *     results from some integration testing I've done on the Surfpack versions of
 *     ann and kriging against those in the separate versions in VendorPackages.
 *
 * 11. Purely static-linked executables have proved difficult with libtool.  The
 *     way libtool sets things up, all the DAKOTA package libraries are static and
 *     linked as such, but system libraries are linked against dynamically (the
 *     way it is currently done on Solaris).  Strangely though, the MPI libraries
 *     aren't linked against dynamically, so this is an area for investigation
 *     (especially as I planned on having it working for the ASC White/Purple
 *     deployment of Dakota 4.0).
 *
 * 12. Running 'make -j <n>' with positive integer n for parallel builds is not
 *     yet supported, as there are a couple packages with dependency requirements
 *     (LHS and IDR, namely) which preclude compiling in parallel until their
 *     requirements are met.  In LHS, those requirements arise from the need for
 *     all the *.mod module files to exist prior to anything which USE's them, and
 *     in IDR, they arise from the lex/yacc generated source dependencies.
 *
 * 13. Disabling F90 support (via the --disable-f90 configure flag) doesn't work
 *     yet.  GNU Automake isn't quite smart enough yet to accept arbitrary
 *     suffixes for F90 files, so it expects that the suffixes will be .f90 or
 *     .f95, _not_ .F!  The Automake developers are working on this, so in the
 *     meantime, I'm using a hack.  I put that hack into Dakota/src/Makefile.am to
 *     address this situation (the same one I use in LHS, by the way, which sets
 *     F77 and FFLAGS to their F90-equivalents) doesn't work when F90 is disabled.
 *
 * 14. The nightly build and test system (in Dakota/test/sqa) will need to be
 *     modified to use the new system, so the nightlies will be broken until I can
 *     get the scripts modified to use the new system.  That will be my next CVS
 *     checkin, and I don't expect it to take very long to make those mods.
 *
 * Revision 1.1  1996/10/22 18:22:09  furnish
 * Update the OS/2 driver with fixes supplied by Thorsten Feiweier.
 *
 * Revision 1.10  1995/03/16  23:23:45  mjl
 * Obsoleted old metafile types and other cleaning up.
 *
 * Revision 1.9  1993/11/15  08:31:37  mjl
 * Bumped metafile version number since now write pages info to header.
 *
 * Revision 1.8  1993/08/09  22:16:39  mjl
 * Miscellaneous cleaning up.
 *
 * Revision 1.7  1993/07/31  08:08:40  mjl
 * Added macro CHANGE_STATE for new state driver function.
 *
 * Revision 1.6  1993/07/02  07:21:12  mjl
 * Added header definitions for use by TK driver.  Also new macros for plplot
 * bop/eop commands.
*/

/*
	metadefs.h
	Geoffrey Furnish
	5 May 1991
	
	This file contains definitions of constants and structures which
	are needed by the PLplot metafile writer and renderer.
*/

/*
* PLMETA_HEADER holds the magic string at head of metafile.
* PLMETA_VERSION holds the version number (year & letter).
* 
* Note: All strings written into the file header are limited to a maximum
* of 80 characters.
*/

#define PLMETA_HEADER	"PLPLOT"
#define PLMETA_VERSION	"1993c"

/* These are used by the TK driver client/server code */

#define PLSERV_HEADER	"PLPLOT"
#define PLSERV_VERSION	"1993b"

/* Symbolic constants for old metafile versions (prior to 1992a). */
/* Now these are stored in the metafile header. */

#define PLMETA_X_OLD	10000
#define PLMETA_Y_OLD	10000

/* Virtual dots/mm for our virtual display space. */

#define PIXEL_RES_X_OLD		42
#define PIXEL_RES_Y_OLD		56

/* Macros to make it easier to abort on nonzero return code */
/* Can't call plexit on a write failure since that would be circular */

#define plm_wr(code) \
    if (code) { fprintf(stderr, "Unable to write to MetaFile\n"); exit(1); }

#define plm_rd(code) \
    if (code) plexit( "Unable to read from MetaFile" )

/*
   Metafile commands.

   *** NOTICE !!! ***
   If you change ANY of the following, you will wreck backward
   backward compatibility with old metafiles.  You may add, but do
   NOT delete !!!
*/

#define INITIALIZE	1
#define CLOSE		2
#define SWITCH_TO_TEXT	3	/* Obsolete, replaced by ESCAPE */
#define SWITCH_TO_GRAPH	4	/* Obsolete, replaced by ESCAPE */
#define EOP		5
#define BOP		6
#define NEW_COLOR	7	/* Obsolete, replaced by CHANGE_STATE */
#define NEW_WIDTH	8	/* Obsolete, replaced by CHANGE_STATE */
#define LINE		9
#define LINETO		10
#define ESCAPE		11
#define ADVANCE		12	/* Obsolete, BOP/EOP used instead */
#define POLYLINE	13
#define NEW_COLOR0 	NEW_COLOR
#define NEW_COLOR1	14
#define CHANGE_STATE	15
#define END_OF_FIELD	255
