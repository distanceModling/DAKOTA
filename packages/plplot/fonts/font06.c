/* $Id: font06.c 3186 2006-02-15 18:17:33Z slbrow $
   $Log$
   Revision 1.1  2006/02/15 18:15:44  slbrow
   This is the commit of the DAKOTA 'autoconfiscation' I've been working on, which
   enables builds using the GNU Autotools.  I have been merging in changes to the
   mainline the entire time I've been working on these changes to my working copy
   (in ~slbrow/projects/Dakota), so none of your changes to DAKOTA will be lost.
   In addition, the final pre-autotools version of DAKOTA has been tagged with:

   Version-3-3-Plus-pre-autotools

   The GNU autotools will be needed by all developers who check DAKOTA out from
   CVS with intentions to build; this is because the version of DAKOTA in the
   repository after this commit does not have any configure or Makefile.in files
   (since they are now machine-generated).  All the source tarballs rolled for
   external consumption (both release and votd) will automatically include all the
   configure and Makefile.in files our non-autotools savvy users will need, while
   still omitting those proprietary sources we may not distribute (DOT, NPSOL).

   To re-generate the missing configure and Makefile.in files in a checkout of
   DAKOTA, only one GNU Autotools script is necessary to run from the
   command-line:

   $ autoreconf --install

   This script (and all the scripts it runs in sequence on a copy of DAKOTA
   checked out of the repository) can be found on the SCICO LAN in the directory:

   /usr/netpub/autotools/bin

   This directory exists on all five of the DAKOTA nightly build platforms (AIX,
   IRIX, Linux, Solaris and Tru64), and should be added to the beginning of your
   $PATH if you are on the SCICO LAN.  If you are not, the versions of the tools
   you need can be easily downloaded from the GNU website.  The versions we use:

   autoconf 2.59  (http://ftp.gnu.org/gnu/autoconf/autoconf-2.59.tar.gz)
   automake 1.9.6 (http://ftp.gnu.org/gnu/automake/automake-1.9.6.tar.gz)
   libtool 1.5.22 (http://ftp.gnu.org/gnu/libtool/libtool-1.5.22.tar.gz)
   m4 1.4.3       (http://ftp.gnu.org/gnu/m4/m4-1.4.3.tar.gz)

   Here is a "short" summary of the major changes made in this )commit:

   1. All configure.in and Makefile.in files are no longer in the CVS repository.
      They are replaced by configure.ac and Makefile.am files, respectively.  When
      you run autoreconf, the configure and Makefile.in files are regenerated
      automatically and recursively.  No $DAKOTA environment variable needs to be
      set in order to build DAKOTA.

   2. Build directories as they were in DAKOTA using the Cygnus configure system
      no longer exist.  Executables built by the GNU Autotools (e.g., the dakota*
      binaries which were built in Dakota/src/i686-unknown-linux on Linux) are now
      built in Dakota/src; the autotools provide deployment targets for installing
      binaries and libraries in a deployment directory, as well as a simple means
      of maintaining more than one build of Dakota for multiple platforms (i.e.,
      using VPATH builds and source- and build-tree separation).

      Suppose you have in your home directory a checked-out and 'autoreconf'd copy
      of Dakota, and you want to simultaneously build versions of DAKOTA for Linux
      and Solaris.  You would create two uniquely named directories in your home
      directory, called 'my_linux_build' and 'my_solaris_build', respectively.
      Assuming two terminals are open on both machines, you would 'cd' into the
      appropriate build directory on each platform and run this command on both:

      $ ../Dakota/configure -C

      This will create a directory tree which is identical to the source tree, but
      containing only Makefiles.  Then, when you run 'make', it will traverse the
      build tree, referencing the sources by relative paths to create objects and
      binaries.  At your option, you can then delete these build directories
      without touching your source tree (the 'autoreconf'd checkout of Dakota).  I
      should note here that the GNU Autotools do not _force_ this separation of
      trees; you can do a 'make distclean' after building for each platform in
      your 'autoreconf'd copy of Dakota with no problems.

   3. All the configure flags which were supported in the old build system (i.e.,
      --without-acro, --enable-debugging, etc.), are supported in the new system
      with the following changes and additions:

      --with-mpi/--without-mpi           -> --enable-mpi/--disable-mpi
      --with-graphics/--without-graphics -> --enable-graphics/--disable-graphics
      --enable-docs (default is off)

      I should note that all the default settings have been preserved.  Further,
      since ACRO, OPT++ and DDACE haven't been integrated yet (see item 9), their
      flags are still commented out in 'Dakota/config/dak_vendoroptimizers.m4'.

   4. The epetra and plplot directories under VendorPackages have been completely
      replaced with the versions available online (versions 3.3 and 5.5.2,
      respectively), which were originally flattened to provide support for our
      build system.  Epetra has been altered from the version distributed by the
      Trilinos team to provide full libtool support as that is necessary for
      building mixed-language archives under Solaris, while plplot has had all
      language-bindings except C++ turned off by default.  Additionally, GSL has
      been updated from version 1.7 available online, but preserving the flattened
      structure it had prior to autoconfiscation.

   5. All other packages in the DAKOTA repository under VendorPackages and
      VendorOptimizers have been 'autoconfiscated' separately from DAKOTA.  To
      avoid duplicated configure-time probes, run configure from the top-level
      DAKOTA directory with the '-C' argument; this will cache the probe results
      from each subpackage, removing the need to do anything other than a lookup
      the next time the probe runs.

   6. No 'mpi' symbolic link needs to be made from VendorPackages/mpi to the MPI
      implementation you wish to build against; MPI wrapper scripts for the C++
      compiler are used instead; if you wish to change the default choices for the
      MPI implementation to build against, you can specify where the wrapper
      scripts you wish to use are via environment variables, which may be set
      on separate lines (using setenv in CSH), or with env on the configure line:

      $ env MPICC=mpicc MPICXX=mpiCC MPIF77=mpif77 ./configure -C

   7. All the clean-up make targets ('clean', 'distclean' and a new one for those
      unfamiliar with the GNU Autotools, 'maintainer-clean') are now machine-
      generated.  The targets remove increasing numbers of files as you would
      expect; the last one removes all the autotools-generated files, returning
      you to a pristine CVS checkout.

   8. Many changes have been made to the sources in Dakota/src:

      A) Fortran calls are now done using FC_FUNC and F77_FUNC macro wrappers,
         removing the need for #ifdef RS6K preprocessor checks for underscores.
      B) Platform testing preprocessor conditionals have been replaced* with the
         feature that was assumed to be supported on that platform 'a priori'
         * mostly: MPI implementation choice is still done by platform.
           See ParallelLibrary.C for details.
      C) TFLOPS and COUGAR #ifdef's have been removed, leaving their #else
         branches to execute unconditionally (as those machines have gone away).

   9. Since the GNU Autotools preferentially look for GCC compilers first; if you
      wish to build on Solaris with the Sun compilers you must set the compilers
      to use in environment variables in the same way the MPI wrappers must be set
      (see item 5 above):

       $ env CC=cc CXX=CC F77=f77 FC=f90 ./configure ...

   Additionally, there are some minor problems to fix with the new build system:

   10. This autoconfiscation covers Dakota, as integration with ACRO, OPT++ and
       DDACE has yet to be done.  I have integrated Surfpack and removed the
       separate ann and kriging packages from VendorPackages, based on positive
       results from some integration testing I've done on the Surfpack versions of
       ann and kriging against those in the separate versions in VendorPackages.

   11. Purely static-linked executables have proved difficult with libtool.  The
       way libtool sets things up, all the DAKOTA package libraries are static and
       linked as such, but system libraries are linked against dynamically (the
       way it is currently done on Solaris).  Strangely though, the MPI libraries
       aren't linked against dynamically, so this is an area for investigation
       (especially as I planned on having it working for the ASC White/Purple
       deployment of Dakota 4.0).

   12. Running 'make -j <n>' with positive integer n for parallel builds is not
       yet supported, as there are a couple packages with dependency requirements
       (LHS and IDR, namely) which preclude compiling in parallel until their
       requirements are met.  In LHS, those requirements arise from the need for
       all the *.mod module files to exist prior to anything which USE's them, and
       in IDR, they arise from the lex/yacc generated source dependencies.

   13. Disabling F90 support (via the --disable-f90 configure flag) doesn't work
       yet.  GNU Automake isn't quite smart enough yet to accept arbitrary
       suffixes for F90 files, so it expects that the suffixes will be .f90 or
       .f95, _not_ .F!  The Automake developers are working on this, so in the
       meantime, I'm using a hack.  I put that hack into Dakota/src/Makefile.am to
       address this situation (the same one I use in LHS, by the way, which sets
       F77 and FFLAGS to their F90-equivalents) doesn't work when F90 is disabled.

   14. The nightly build and test system (in Dakota/test/sqa) will need to be
       modified to use the new system, so the nightlies will be broken until I can
       get the scripts modified to use the new system.  That will be my next CVS
       checkin, and I don't expect it to take very long to make those mods.

   Revision 1.1  1992/05/20 21:33:25  furnish
   Initial checkin of the whole PLPLOT project.

*/

      short int subbuffer123[100] = {
       8891, 9022, 9025, 8899, 8772, 8517, 8261,   64, 8261, 8004,
       7745, 7614, 7610, 7736,   64, 8247, 8504, 8763, 8894, 8898,
       8772,    0,  -16, 7116, 6859, 8004, 7479,   64, 8004, 7607,
         64, 8772, 8759,   64, 8772, 8887,   64, 7106, 7364, 7749,
       9413,   64, 7106, 7363, 7748, 9412,    0,  -16, 7116, 6985,
       7484, 7609, 7736, 7991, 8247, 8632, 8891, 9022, 9025, 8899,
       8772, 8517, 8261, 7876, 7617, 7486, 6960,   64, 8247, 8504,
       8763, 8894, 8898, 8772,   64, 8261, 8004, 7745, 7614, 7088,
          0,  -16, 7116, 6987, 9413, 8133, 7748, 7489, 7358, 7355,
       7481, 7608, 7863, 8119, 8504, 8763, 8894, 8897, 8771, 8644
      };
      short int subbuffer124[100] = {
       8389,   64, 8133, 7876, 7617, 7486, 7482, 7608,   64, 8119,
       8376, 8635, 8766, 8770, 8644,   64, 8644, 9412,    0,  -16,
       7116, 6986, 8388, 7991,   64, 8388, 8119,   64, 7234, 7492,
       7877, 9285,   64, 7234, 7491, 7876, 9284,    0,  -16, 7116,
       6986, 7105, 7235, 7493, 7877, 8004, 8002, 7740, 7737, 7991,
         64, 7749, 7876, 7874, 7612, 7609, 7736, 7991, 8119, 8504,
       8762, 9021, 9152, 9155, 9029, 8900, 9027, 9152,   64, 9021,
       9155,    0,  -16, 7116, 6859, 7876, 7619, 7361, 7230, 7227,
       7353, 7480, 7735, 8119, 8504, 8890, 9149, 9280, 9283, 9029,
       8773, 8515, 8255, 7994, 7600,   64, 7227, 7481, 7736, 8120
      };
      short int subbuffer125[100] = {
       8505, 8891, 9149,   64, 9283, 9028, 8772, 8514, 8255, 7993,
       7728,    0,  -16, 7116, 7113, 7365, 7621, 7876, 8002, 8627,
       8753, 8880,   64, 7621, 7748, 7874, 8499, 8625, 8880, 9136,
         64, 9285, 9155, 8896, 7605, 7346, 7216,    0,  -16, 7116,
       6731, 8652, 7856,   64, 8780, 7728,   64, 6849, 6979, 7237,
       7621, 7748, 7746, 7613, 7610, 7864, 8248, 8505, 8892, 9151,
         64, 7493, 7620, 7618, 7485, 7482, 7608, 7863, 8247, 8504,
       8762, 9021, 9151, 9413,    0,  -16, 7116, 6731, 7233, 7491,
       7876, 7749, 7492, 7233, 7102, 7099, 7224, 7351, 7607, 7864,
       8123, 8254,   64, 7099, 7225, 7352, 7608, 7865, 8123,   64
      };
      short int subbuffer126[100] = {
       8126, 8123, 8248, 8375, 8631, 8888, 9147, 9278, 9281, 9156,
       9029, 8900, 9155, 9281,   64, 8123, 8249, 8376, 8632, 8889,
       9147,    0,  -16, 7116, 6987, 9029, 8766, 8634, 8632, 8759,
       9143, 9401, 9531,   64, 9157, 8894, 8762, 8760, 8887,   64,
       8766, 8769, 8644, 8389, 8133, 7748, 7489, 7358, 7355, 7481,
       7608, 7863, 8119, 8376, 8635, 8766,   64, 8133, 7876, 7617,
       7486, 7482, 7608,    0,  -16, 7116, 6985, 8012, 7487, 7484,
       7609, 7736,   64, 8140, 7615,   64, 7615, 7746, 8004, 8261,
       8517, 8772, 8899, 9025, 9022, 8891, 8632, 8247, 7991, 7736,
       7611, 7615,   64, 8772, 8898, 8894, 8763, 8504, 8247,   64
      };
      short int subbuffer127[100] = {
       7628, 8140,    0,  -16, 7116, 7113, 8898, 8897, 9025, 9026,
       8900, 8645, 8261, 7876, 7617, 7486, 7483, 7609, 7736, 7991,
       8247, 8632, 8891,   64, 8261, 8004, 7745, 7614, 7610, 7736,
          0,  -16, 7116, 6987, 9292, 8766, 8634, 8632, 8759, 9143,
       9401, 9531,   64, 9420, 8894, 8762, 8760, 8887,   64, 8766,
       8769, 8644, 8389, 8133, 7748, 7489, 7358, 7355, 7481, 7608,
       7863, 8119, 8376, 8635, 8766,   64, 8133, 7876, 7617, 7486,
       7482, 7608,   64, 8908, 9420,    0,  -16, 7116, 7113, 7612,
       8125, 8510, 8896, 9026, 8900, 8645, 8261, 7876, 7617, 7486,
       7483, 7609, 7736, 7991, 8247, 8632, 8890,   64, 8261, 8004
      };
      short int subbuffer128[100] = {
       7745, 7614, 7610, 7736,    0,  -16, 7116, 7368, 9291, 9162,
       9289, 9418, 9419, 9292, 9036, 8779, 8650, 8520, 8389, 7991,
       7859, 7729,   64, 9036, 8778, 8648, 8516, 8251, 8119, 7988,
       7858, 7729, 7472, 7216, 7089, 7090, 7219, 7346, 7217,   64,
       7877, 9157,    0,  -16, 7116, 6986, 9157, 8631, 8500, 8241,
       7856, 7472, 7217, 7090, 7091, 7220, 7347, 7218,   64, 9029,
       8503, 8372, 8113, 7856,   64, 8766, 8769, 8644, 8389, 8133,
       7748, 7489, 7358, 7355, 7481, 7608, 7863, 8119, 8376, 8635,
       8766,   64, 8133, 7876, 7617, 7486, 7482, 7608,    0,  -16,
       7116, 6987, 8012, 7223,   64, 8140, 7351,   64, 7614, 7874
      };
      short int subbuffer129[100] = {
       8132, 8389, 8645, 8900, 9027, 9025, 8763, 8760, 8887,   64,
       8645, 8899, 8897, 8635, 8632, 8759, 9143, 9401, 9531,   64,
       7628, 8140,    0,  -16, 7116, 7495, 8652, 8523, 8650, 8779,
       8652,   64, 7617, 7747, 8005, 8389, 8516, 8513, 8251, 8248,
       8375,   64, 8261, 8388, 8385, 8123, 8120, 8247, 8631, 8889,
       9019,    0,  -16, 7116, 7495, 8780, 8651, 8778, 8907, 8780,
         64, 7745, 7875, 8133, 8517, 8644, 8641, 8247, 8116, 7986,
       7857, 7600, 7344, 7217, 7218, 7347, 7474, 7345,   64, 8389,
       8516, 8513, 8119, 7988, 7858, 7600,    0,  -16, 7116, 6986,
       8012, 7223,   64, 8140, 7351,   64, 9028, 8899, 9026, 9155
      };
      short int subbuffer130[100] = {
       9156, 9029, 8901, 8644, 8128, 7871, 7615,   64, 7871, 8126,
       8376, 8503,   64, 7871, 7998, 8248, 8375, 8631, 8888, 9147,
         64, 7628, 8140,    0,  -16, 7116, 7623, 8652, 8126, 7994,
       7992, 8119, 8503, 8761, 8891,   64, 8780, 8254, 8122, 8120,
       8247,   64, 8268, 8780,    0,  -16, 7116, 6096, 6209, 6339,
       6597, 6981, 7108, 7106, 6974, 6711,   64, 6853, 6980, 6978,
       6846, 6583,   64, 6974, 7234, 7492, 7749, 8005, 8260, 8387,
       8385, 7991,   64, 8005, 8259, 8257, 7863,   64, 8254, 8514,
       8772, 9029, 9285, 9540, 9667, 9665, 9403, 9400, 9527,   64,
       9285, 9539, 9537, 9275, 9272, 9399, 9783,10041,10171,    0
      };
      short int subbuffer131[100] = {
        -16, 7116, 6731, 6849, 6979, 7237, 7621, 7748, 7746, 7614,
       7351,   64, 7493, 7620, 7618, 7486, 7223,   64, 7614, 7874,
       8132, 8389, 8645, 8900, 9027, 9025, 8763, 8760, 8887,   64,
       8645, 8899, 8897, 8635, 8632, 8759, 9143, 9401, 9531,    0,
        -16, 7116, 7113, 8261, 7876, 7617, 7486, 7483, 7609, 7736,
       7991, 8247, 8632, 8891, 9022, 9025, 8899, 8772, 8517, 8261,
         64, 8261, 8004, 7745, 7614, 7610, 7736,   64, 8247, 8504,
       8763, 8894, 8898, 8772,    0,  -16, 7116, 6858, 6977, 7107,
       7365, 7749, 7876, 7874, 7742, 7216,   64, 7621, 7748, 7746,
       7614, 7088,   64, 7742, 7873, 8132, 8389, 8645, 8900, 9027
      };
      short int subbuffer132[100] = {
       9153, 9150, 9019, 8760, 8375, 8119, 7864, 7739, 7742,   64,
       8900, 9026, 9022, 8891, 8632, 8375,   64, 6704, 7600,    0,
        -16, 7116, 6986, 9029, 8240,   64, 9157, 8368,   64, 8766,
       8769, 8644, 8389, 8133, 7748, 7489, 7358, 7355, 7481, 7608,
       7863, 8119, 8376, 8635, 8766,   64, 8133, 7876, 7617, 7486,
       7482, 7608,   64, 7856, 8752,    0,  -16, 7116, 7112, 7233,
       7363, 7621, 8005, 8132, 8130, 7998, 7735,   64, 7877, 8004,
       8002, 7870, 7607,   64, 7998, 8258, 8516, 8773, 9029, 9156,
       9155, 9026, 8899, 9028,    0,  -16, 7116, 7241, 9027, 9026,
       9154, 9155, 9028, 8645, 8261, 7876, 7747, 7745, 7872, 8764
      };
      short int subbuffer133[100] = {
       8891,   64, 7746, 7873, 8765, 8892, 8889, 8760, 8375, 7991,
       7608, 7481, 7482, 7610, 7609,    0,  -16, 7116, 7367, 8524,
       7998, 7866, 7864, 7991, 8375, 8633, 8763,   64, 8652, 8126,
       7994, 7992, 8119,   64, 7749, 8901,    0,  -16, 7116, 6731,
       6849, 6979, 7237, 7621, 7748, 7745, 7483, 7481, 7735,   64,
       7493, 7620, 7617, 7355, 7353, 7480, 7735, 7991, 8248, 8506,
       8766,   64, 9029, 8766, 8634, 8632, 8759, 9143, 9401, 9531,
         64, 9157, 8894, 8762, 8760, 8887,    0,  -16, 7116, 6986,
       7105, 7235, 7493, 7877, 8004, 8001, 7739, 7737, 7991,   64,
       7749, 7876, 7873, 7611, 7609, 7736, 7991, 8119, 8504, 8762
      };
      short int subbuffer134[100] = {
       9021, 9153, 9157, 9029, 9155,    0,  -16, 7116, 6350, 6465,
       6595, 6853, 7237, 7364, 7361, 7099, 7097, 7351,   64, 7109,
       7236, 7233, 6971, 6969, 7096, 7351, 7607, 7864, 8122, 8252,
         64, 8517, 8252, 8249, 8376, 8631, 8887, 9144, 9402, 9532,
       9664, 9669, 9541, 9667,   64, 8645, 8380, 8377, 8631,    0,
        -16, 7116, 6986, 7361, 7620, 7877, 8261, 8387, 8384,   64,
       8133, 8259, 8256, 8124, 7994, 7736, 7479, 7351, 7224, 7225,
       7354, 7481, 7352,   64, 8124, 8121, 8247, 8631, 8888, 9147,
         64, 9156, 9027, 9154, 9283, 9284, 9157, 9029, 8772, 8514,
       8384, 8252, 8249, 8375,    0,  -16, 7116, 6858, 6977, 7107
      };
      short int subbuffer135[100] = {
       7365, 7749, 7876, 7873, 7611, 7609, 7863,   64, 7621, 7748,
       7745, 7483, 7481, 7608, 7863, 8119, 8376, 8634, 8894,   64,
       9285, 8759, 8628, 8369, 7984, 7600, 7345, 7218, 7219, 7348,
       7475, 7346,   64, 9157, 8631, 8500, 8241, 7984,    0,  -16,
       7116, 6986, 9157, 9027, 8769, 7739, 7481, 7351,   64, 7489,
       7619, 7877, 8261, 8771,   64, 7619, 7876, 8260, 8771, 9027,
         64, 7481, 7737, 8248, 8632, 8889,   64, 7737, 8247, 8631,
       8889, 9019,    0,  -16, 7116, 6860, 8779, 8650, 8777, 8906,
       8907, 8652, 8268, 7883, 7625, 7494, 7479,   64, 8268, 8011,
       7753, 7622, 7607,   64, 9547, 9418, 9545, 9674, 9675, 9548
      };
      short int subbuffer136[100] = {
       9292, 9035, 8905, 8887,   64, 9292, 9163, 9033, 9015,   64,
       7109, 9413,   64, 7095, 7991,   64, 8503, 9399,    0,  -16,
       7116, 6859, 8907, 8778, 8905, 9034, 8907, 8652, 8268, 7883,
       7625, 7494, 7479,   64, 8268, 8011, 7753, 7622, 7607,   64,
       8901, 8887,   64, 9029, 9015,   64, 7109, 9029,   64, 7095,
       7991,   64, 8503, 9399,    0,  -16, 7116, 6859, 8779, 8650,
       8777, 8906, 8907, 8652,   64, 9036, 8268, 7883, 7625, 7494,
       7479,   64, 8268, 8011, 7753, 7622, 7607,   64, 8906, 8887,
         64, 9036, 9015,   64, 7109, 8901,   64, 7095, 7991,   64,
       8503, 9399,    0,  -16, 7116, 6225, 8267, 8138, 8265, 8394
      };
      short int subbuffer137[100] = {
       8267, 8012, 7628, 7243, 6985, 6854, 6839,   64, 7628, 7371,
       7113, 6982, 6967,   64, 9675, 9546, 9673, 9802, 9675, 9420,
       9036, 8651, 8393, 8262, 8247,   64, 9036, 8779, 8521, 8390,
       8375,   64, 9669, 9655,   64, 9797, 9783,   64, 6469, 9797,
         64, 6455, 7351,   64, 7863, 8759,   64, 9271,10167,    0,
        -16, 7116, 6225, 8267, 8138, 8265, 8394, 8267, 8012, 7628,
       7243, 6985, 6854, 6839,   64, 7628, 7371, 7113, 6982, 6967,
         64, 9547, 9418, 9545, 9674, 9675, 9420,   64, 9804, 9036,
       8651, 8393, 8262, 8247,   64, 9036, 8779, 8521, 8390, 8375,
         64, 9674, 9655,   64, 9804, 9783,   64, 6469, 9669,   64
      };
      short int subbuffer138[100] = {
       6455, 7351,   64, 7863, 8759,   64, 9271,10167,    0,  -16,
       7116, 7622, 8261, 8247,   64, 8389, 8375,   64, 7877, 8389,
         64, 7863, 8759,    0,  -16, 7116, 7112, 8900, 8645, 8261,
       7876, 7618, 7487, 7484, 7609, 7736, 8119, 8503, 8760,   64,
       8261, 8004, 7746, 7615, 7612, 7737, 7864, 8119,   64, 7614,
       8638,    0,  -16, 7116, 7114, 8524, 8139, 7880, 7750, 7619,
       7486, 7482, 7608, 7863, 8119, 8504, 8763, 8893, 9024, 9157,
       9161, 9035, 8780, 8524,   64, 8524, 8267, 8008, 7878, 7747,
       7614, 7610, 7736, 7863,   64, 8119, 8376, 8635, 8765, 8896,
       9029, 9033, 8907, 8780,   64, 7746, 8898,    0,  -16, 7116
      };
      short int subbuffer139[100] = {
       6859, 8652, 7856,   64, 8780, 7728,   64, 8133, 7620, 7362,
       7231, 7228, 7354, 7608, 7991, 8375, 8888, 9146, 9277, 9280,
       9154, 8900, 8517, 8133,   64, 8133, 7748, 7490, 7359, 7356,
       7482, 7736, 7991,   64, 8375, 8760, 9018, 9149, 9152, 9026,
       8772, 8517,    0,  -16, 7116, 7113, 8517, 8772, 9026, 9027,
       8900, 8517, 8133, 7748, 7619, 7489, 7487, 7613, 7867, 8376,
         64, 8133, 7876, 7747, 7617, 7615, 7741, 8376, 8502, 8500,
       8371, 8115,    0,  -16, 7116, 6859, 7493, 7364, 7234, 7232,
       7357, 7865, 7991,   64, 7232, 7358, 7866, 7991, 7989, 7858,
       7600, 7472, 7345, 7219, 7222, 7354, 7614, 7873, 8260, 8517
      };
      short int subbuffer140[100] = {
       8773, 9156, 9282, 9278, 9146, 8888, 8631, 8503, 8376, 8378,
       8507, 8634, 8505,   64, 8773, 9028, 9154, 9150, 9018, 8888,
          0,  -16, 7116, 6605, 9163, 9034, 9161, 9290, 9163, 8908,
       8524, 8139, 7881, 7751, 7620, 7488, 7223, 7091, 6961,   64,
       8524, 8267, 8009, 7879, 7748, 7483, 7351, 7220, 7090, 6961,
       6704, 6448, 6321, 6322, 6451, 6578, 6449,   64, 9931, 9802,
       9929,10058,10059, 9932, 9676, 9419, 9290, 9160, 9029, 8631,
       8499, 8369,   64, 9676, 9418, 9288, 9156, 8891, 8759, 8628,
       8498, 8369, 8112, 7856, 7729, 7730, 7859, 7986, 7857,   64,
       7109, 9797,    0,  -16, 7116, 6732, 9419, 9290, 9417, 9546
      };
      short int subbuffer141[100] = {
       9419, 9036, 8652, 8267, 8009, 7879, 7748, 7616, 7351, 7219,
       7089,   64, 8652, 8395, 8137, 8007, 7876, 7611, 7479, 7348,
       7218, 7089, 6832, 6576, 6449, 6450, 6579, 6706, 6577,   64,
       9157, 8894, 8762, 8760, 8887, 9271, 9529, 9659,   64, 9285,
       9022, 8890, 8888, 9015,   64, 7237, 9285,    0,  -16, 7116,
       6732, 9163, 9034, 9161, 9290, 9291, 9036,   64, 9548, 8652,
       8267, 8009, 7879, 7748, 7616, 7351, 7219, 7089,   64, 8652,
       8395, 8137, 8007, 7876, 7611, 7479, 7348, 7218, 7089, 6832,
       6576, 6449, 6450, 6579, 6706, 6577,   64, 9420, 8894, 8762,
       8760, 8887, 9271, 9529, 9659,   64, 9548, 9022, 8890, 8888
      };
      short int subbuffer142[100] = {
       9015,   64, 7237, 9157,    0,  -16, 7116, 5969, 8523, 8394,
       8521, 8650, 8523, 8268, 7884, 7499, 7241, 7111, 6980, 6848,
       6583, 6451, 6321,   64, 7884, 7627, 7369, 7239, 7108, 6843,
       6711, 6580, 6450, 6321, 6064, 5808, 5681, 5682, 5811, 5938,
       5809,   64,10059, 9930,10057,10186,10059, 9676, 9292, 8907,
       8649, 8519, 8388, 8256, 7991, 7859, 7729,   64, 9292, 9035,
       8777, 8647, 8516, 8251, 8119, 7988, 7858, 7729, 7472, 7216,
       7089, 7090, 7219, 7346, 7217,   64, 9797, 9534, 9402, 9400,
       9527, 9911,10169,10299,   64, 9925, 9662, 9530, 9528, 9655,
         64, 6469, 9925,    0,  -16, 7116, 5969, 8523, 8394, 8521
      };
      short int subbuffer143[100] = {
       8650, 8523, 8268, 7884, 7499, 7241, 7111, 6980, 6848, 6583,
       6451, 6321,   64, 7884, 7627, 7369, 7239, 7108, 6843, 6711,
       6580, 6450, 6321, 6064, 5808, 5681, 5682, 5811, 5938, 5809,
         64, 9803, 9674, 9801, 9930, 9931, 9676,   64,10188, 9292,
       8907, 8649, 8519, 8388, 8256, 7991, 7859, 7729,   64, 9292,
       9035, 8777, 8647, 8516, 8251, 8119, 7988, 7858, 7729, 7472,
       7216, 7089, 7090, 7219, 7346, 7217,   64,10060, 9534, 9402,
       9400, 9527, 9911,10169,10299,   64,10188, 9662, 9530, 9528,
       9655,   64, 6469, 9797,    0,  -16, 7116, 7495, 7617, 7747,
       8005, 8389, 8516, 8513, 8251, 8248, 8375,   64, 8261, 8388
      };
      short int subbuffer144[100] = {
       8385, 8123, 8120, 8247, 8631, 8889, 9019,    0,  -16, 7116,
       8256,   64,    0,  -16, 7116, 7748,   64,    0,  -16, 7116,
       7240,   64,    0,  -16, 7116, 6986, 8140, 7755, 7496, 7363,
       7360, 7483, 7736, 8119, 8375, 8760, 9019, 9152, 9155, 9032,
       8779, 8396, 8140,   64, 8140, 7883, 7754, 7624, 7491, 7488,
       7611, 7737, 7864, 8119,   64, 8375, 8632, 8761, 8891, 9024,
       9027, 8904, 8778, 8651, 8396,    0,  -16, 7116, 6986, 7752,
       8009, 8396, 8375,   64, 8267, 8247,   64, 7735, 8887,    0,
        -16, 7116, 6986, 7496, 7623, 7494, 7367, 7368, 7498, 7627,
       8012, 8524, 8907, 9034, 9160, 9158, 9028, 8642, 8000, 7743
      };
      short int subbuffer145[100] = {
       7485, 7354, 7351,   64, 8524, 8779, 8906, 9032, 9030, 8900,
       8514, 8000,   64, 7353, 7482, 7738, 8376, 8760, 9017, 9146,
         64, 7738, 8375, 8887, 9016, 9146, 9148,    0,  -16, 7116,
       6986, 7496, 7623, 7494, 7367, 7368, 7498, 7627, 8012, 8524,
       8907, 9033, 9030, 8900, 8515, 8131,   64, 8524, 8779, 8905,
       8902, 8772, 8515,   64, 8515, 8770, 9024, 9150, 9147, 9017,
       8888, 8503, 7991, 7608, 7481, 7355, 7356, 7485, 7612, 7483,
         64, 8897, 9022, 9019, 8889, 8760, 8503,    0,  -16, 7116,
       6986, 8522, 8503,   64, 8652, 8631,   64, 8652, 7229, 9277,
         64, 8119, 9015,    0,  -16, 7116, 6986, 7628, 7362,   64
      };
      short int subbuffer146[100] = {
       7362, 7620, 8005, 8389, 8772, 9026, 9151, 9149, 9018, 8760,
       8375, 7991, 7608, 7481, 7355, 7356, 7485, 7612, 7483,   64,
       8389, 8644, 8898, 9023, 9021, 8890, 8632, 8375,   64, 7628,
       8908,   64, 7627, 8267, 8908,    0,  -16, 7116, 6986, 8905,
       8776, 8903, 9032, 9033, 8907, 8652, 8268, 7883, 7625, 7495,
       7363, 7357, 7482, 7736, 8119, 8375, 8760, 9018, 9149, 9150,
       9025, 8771, 8388, 8260, 7875, 7617, 7486,   64, 8268, 8011,
       7753, 7623, 7491, 7485, 7610, 7864, 8119,   64, 8375, 8632,
       8890, 9021, 9022, 8897, 8643, 8388,    0,  -16, 7116, 6986,
       7372, 7366,   64, 7368, 7498, 7756, 8012, 8649, 8905, 9034
      };
      short int subbuffer147[100] = {
       9164,   64, 7498, 7755, 8011, 8649,   64, 9164, 9161, 9030,
       8513, 8383, 8252, 8247,   64, 9030, 8385, 8255, 8124, 8119,
          0,  -16, 7116, 6986, 8012, 7627, 7497, 7494, 7620, 8003,
       8515, 8900, 9030, 9033, 8907, 8524, 8012,   64, 8012, 7755,
       7625, 7622, 7748, 8003,   64, 8515, 8772, 8902, 8905, 8779,
       8524,   64, 8003, 7618, 7489, 7359, 7355, 7481, 7608, 7991,
       8503, 8888, 9017, 9147, 9151, 9025, 8898, 8515,   64, 8003,
       7746, 7617, 7487, 7483, 7609, 7736, 7991,   64, 8503, 8760,
       8889, 9019, 9023, 8897, 8770, 8515,    0,  -16, 7116, 6986,
       9029, 8898, 8640, 8255, 8127, 7744, 7490, 7365, 7366, 7497
      };
      short int subbuffer148[100] = {
       7755, 8140, 8396, 8779, 9033, 9158, 9152, 9020, 8890, 8632,
       8247, 7863, 7608, 7482, 7483, 7612, 7739, 7610,   64, 8127,
       7872, 7618, 7493, 7494, 7625, 7883, 8140,   64, 8396, 8651,
       8905, 9030, 9024, 8892, 8762, 8504, 8247,    0,  -16, 7116,
       7621, 8249, 8120, 8247, 8376, 8249,    0,  -16, 7116, 7621,
       8247, 8120, 8249, 8376, 8374, 8244, 8115,    0,  -16, 7116,
       7621, 8261, 8132, 8259, 8388, 8261,   64, 8249, 8120, 8247,
       8376, 8249,    0,  -16, 7116, 7621, 8261, 8132, 8259, 8388,
       8261,   64, 8247, 8120, 8249, 8376, 8374, 8244, 8115,    0,
        -16, 7116, 7621, 8268, 8138, 8254, 8394, 8268,   64, 8266
      };
