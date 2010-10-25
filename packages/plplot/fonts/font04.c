/* $Id: font04.c 3186 2006-02-15 18:17:33Z slbrow $
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

   Revision 1.1  1992/05/20 21:33:23  furnish
   Initial checkin of the whole PLPLOT project.

*/

      short int subbuffer067[100] = {
       8384, 8255, 8127, 8000, 8001, 8130, 8258, 8385, 8383, 8253,
       8124,   64, 8129, 8128, 8256, 8257, 8129,    0,  -16, 7116,
       8002, 8130, 8001, 7999, 8126, 8382, 8511, 8513, 8386, 8130,
         64, 8257, 8128, 8255, 8384, 8257,    0,  -16, 7116, 7748,
       8003, 8509,   64, 8515, 7997,   64, 7744, 8768,    0,  -16,
       7116, 7621, 8263, 8133, 7874, 7616,   64, 8263, 8389, 8642,
       8896,   64, 8261, 7873,   64, 8261, 8641,   64, 8259, 8001,
         64, 8259, 8513,   64, 8129, 8385,   64, 7616, 8896,    0,
        -16, 7116, 7621, 7616, 7617, 7747, 7876, 8133, 8389, 8644,
       8771, 8897, 8896,   64, 8004, 8516,   64, 7875, 8643,   64
      };
      short int subbuffer068[100] = {
       7746, 8770,   64, 7745, 8769,   64, 7616, 8896,    0,  -16,
       7116, 7488, 7500, 7488, 8256, 7500,   64, 7497, 8001,   64,
       7494, 7872,   64, 7491, 7617,    0,  -16, 7116, 7621, 8263,
       8133, 7874, 7616,   64, 8263, 8389, 8642, 8896,    0,  -16,
       7116, 7621, 8896, 8897, 8771, 8644, 8389, 8133, 7876, 7747,
       7617, 7616,    0,  -16, 7116, 6859, 9664, 9666, 9541, 9288,
       8906, 8523, 8011, 7626, 7240, 6981, 6850, 6848,    0,  -16,
       7116, 7621, 7616, 7615, 7741, 7868, 8123, 8379, 8636, 8765,
       8895, 8896,    0,  -16, 7116, 7494, 7490, 7744, 8127, 8383,
       8768, 9026,    0,  -16, 7116, 8259, 8253, 8510, 8640, 8514
      };
      short int subbuffer069[100] = {
       8259,    0,  -16, 7116, 8260, 8256, 8642, 8772, 8774, 8647,
       8519,    0,  -16, 7116, 7744, 8256, 7874, 7748, 7750, 7879,
       8007,    0,  -16, 7116, 7621, 8906, 8779, 8524, 8140, 7883,
       7754, 7624, 7622, 7748, 7875, 8639, 8766, 8892, 8890, 8760,
       8631, 8374, 7990, 7735, 7608,    0,  -16, 7116, 6859, 7099,
       6972, 6846, 6849, 6979, 7108, 7365, 7621, 7876, 8003, 8509,
       8636, 8891, 9147, 9404, 9533, 9663, 9666, 9540, 9413,    0,
        -16, 7116, 6859, 8256, 8509, 8636, 8891, 9147, 9404, 9533,
       9663, 9665, 9539, 9412, 9157, 8901, 8644, 8515, 7997, 7868,
       7611, 7355, 7100, 6973, 6847, 6849, 6979, 7108, 7365, 7621
      };
      short int subbuffer070[100] = {
       7876, 8003, 8256,    0,  -16, 7116, 6986, 7372, 7351,   64,
       6988, 9420, 8130, 9400,   64, 9276, 9401, 9527,   64, 9276,
       9273,   64, 8889, 9273,   64, 8889, 9272, 9527,    0,  -16,
       7116, 7367, 8657, 8272, 8015, 7757, 7498, 7366, 7360, 7485,
       7739, 8122, 8378, 8763, 9021, 9152,   64, 7362, 7493, 7751,
       8136, 8392, 8775, 9029, 9154, 9148, 9016, 8757, 8499, 8242,
       7857,    0,  -16, 7116, 5716, 5696,10816,    0,  -16, 7116,
       6478, 6450,10062,    0,  -16, 7116, 8256, 8276, 8236,    0,
        -16, 7116, 6478, 6478,10034,    0,  -16, 7116, 6478, 6464,
      10048,    0,  -16, 7116, 6732, 6713, 9799,    0,  -16, 7116
      };
      short int subbuffer071[100] = {
       7367, 7348, 9164,    0,  -16, 7116, 8256, 8270, 8242,    0,
        -16, 7116, 7367, 7372, 9140,    0,  -16, 7116, 6732, 6727,
       9785,    0,  -16, 7116, 7367, 7360, 9152,    0,  -16, 7116,
       7621, 7611, 8901,    0,  -16, 7116, 8256, 8263, 8249,    0,
        -16, 7116, 7621, 7621, 8891,    0,  -16, 7116, 6848, 8267,
       8011, 7626, 7240, 6981, 6850, 6848,    0,  -16, 7116, 6848,
       6848, 6846, 6971, 7224, 7606, 7989, 8245,    0,  -16, 7116,
       8267, 8245, 8501, 8886, 9272, 9531, 9662, 9664,    0,  -16,
       7116, 8267, 9664, 9666, 9541, 9288, 8906, 8523, 8267,    0,
        -16, 7116, 6478, 6467, 6849, 7359, 7998, 8510, 9151, 9665
      };
      short int subbuffer072[100] = {
      10051,    0,  -16, 7116, 8003, 8654, 8395, 8135, 8002, 7998,
       8121, 8373, 8626,    0,  -16, 7116, 7874, 7886, 8139, 8391,
       8514, 8510, 8377, 8117, 7858,    0,  -16, 7116, 6478, 6461,
       6847, 7361, 8002, 8514, 9153, 9663,10045,    0,  -16, 7116,
       7367, 8264, 9156, 7356, 8248,    0,  -16, 7116, 7240, 7232,
       7751, 8761, 9280,    0,  -16, 7116, 7367, 7356, 7364, 9148,
       9156,    0,  -16, 7116, 7240, 7482, 7234, 9278, 9030,    0,
        -16, 7116, 7240, 7221, 7477, 7862, 8119, 8506, 8636, 8767,
       8771, 8646, 8520, 8393, 8137, 8008, 7878, 7747, 7743, 7868,
       7994, 8375, 8630, 9013, 9269,    0,  -16, 7116, 7115, 9656
      };
      short int subbuffer073[100] = {
       9658, 9533, 9407, 9026, 8771, 8388, 7876, 7491, 7234, 7105,
       7103, 7230, 7485, 7868, 8380, 8765, 9022, 9409, 9539, 9670,
       9672,    0,  -16, 7116, 7240, 9291, 9035, 8650, 8393, 8006,
       7876, 7745, 7741, 7866, 7992, 8119, 8375, 8504, 8634, 8765,
       8769, 8644, 8518, 8137, 7882, 7499, 7243,    0,  -16, 7116,
       6857, 6856, 6854, 6979, 7105, 7486, 7741, 8124, 8636, 9021,
       9278, 9407, 9409, 9282, 9027, 8644, 8132, 7747, 7490, 7103,
       6973, 6842, 6840,    0,  -16, 7116, 6601, 6594, 6720, 6974,
       7229, 7612, 8124, 8637, 9023, 9282, 9412, 9286, 8902, 8389,
       8132, 7746, 7487, 7356, 7353, 7478, 7604,    0,  -16, 7116
      };
      short int subbuffer074[100] = {
       6599, 6590, 6972, 7355, 7995, 8380, 8766, 9025, 9156, 9158,
       9031, 8775, 8390, 8004, 7745, 7614, 7609, 7734, 7987,    0,
        -16, 7116, 7875, 8131, 7873, 7871, 8125, 8381, 8639, 8641,
       8387, 8131,   64, 8130, 8001, 7999, 8126, 8382, 8511, 8513,
       8386, 8130,   64, 8257, 8128, 8255, 8384, 8257,    0,  -16,
       7116, 8261, 8261, 8389, 8644, 8771, 8897, 8895, 8765, 8636,
       8379, 8251,    0,  -16, 7116, 6478, 6464, 7232,   64, 7872,
       8640,   64, 9280,10048,    0,  -16, 7116, 6478, 6461, 6467,
      10051,10045,    0,  -16, 7116, 7240, 8270, 7232,   64, 8270,
       9280,    0,  -16, 7116, 6478, 6464,10048,   64, 7225, 9273
      };
      short int subbuffer075[100] = {
         64, 7986, 8498,    0,  -16, 7116, 6478, 6464,10048,   64,
       6464, 8240,   64,10048, 8240,    0,  -16, 7116, 7367, 8135,
       7750, 7492, 7361, 7359, 7484, 7738, 8121, 8377, 8762, 9020,
       9151, 9153, 9028, 8774, 8391, 8135,    0,  -16, 7116, 7494,
       7494, 7482, 9018, 9030, 7494,    0,  -16, 7116, 7367, 8264,
       7356, 9148, 8264,    0,  -16, 7116, 7494, 8266, 7488, 8246,
       9024, 8266,    0,  -16, 7116, 7240, 8265, 8003, 7235, 7871,
       7609, 8253, 8889, 8639, 9283, 8515, 8265,    0,  -16, 7116,
       7367, 8263, 8249,   64, 7360, 9152,    0,  -16, 7116, 7621,
       7621, 8891,   64, 8901, 7611,    0,  -16, 7116, 7621, 8262
      };
      short int subbuffer076[100] = {
       8250,   64, 7619, 8893,   64, 8899, 7613,    0,  -16, 7116,
       7748, 8132, 7875, 7745, 7743, 7869, 8124, 8380, 8637, 8767,
       8769, 8643, 8388, 8132,   64, 7873, 7871,   64, 8002, 7998,
         64, 8131, 8125,   64, 8259, 8253,   64, 8387, 8381,   64,
       8514, 8510,   64, 8641, 8639,    0,  -16, 7116, 7748, 7748,
       7740, 8764, 8772, 7748,   64, 7875, 7869,   64, 8003, 7997,
         64, 8131, 8125,   64, 8259, 8253,   64, 8387, 8381,   64,
       8515, 8509,   64, 8643, 8637,    0,  -16, 7116, 7621, 8262,
       7613, 8893, 8262,   64, 8259, 7870,   64, 8259, 8638,   64,
       8256, 8126,   64, 8256, 8382,    0,  -16, 7116, 7491, 7488
      };
      short int subbuffer077[100] = {
       8635, 8645, 7488,   64, 7872, 8509,   64, 7872, 8515,   64,
       8256, 8511,   64, 8256, 8513,    0,  -16, 7116, 7621, 8250,
       8899, 7619, 8250,   64, 8253, 8642,   64, 8253, 7874,   64,
       8256, 8386,   64, 8256, 8130,    0,  -16, 7116, 7878, 9024,
       7877, 7867, 9024,   64, 8640, 8003,   64, 8640, 7997,   64,
       8256, 8001,   64, 8256, 7999,    0,  -16, 7116, 7494, 8262,
       7739, 9026, 7490, 8763, 8262,   64, 8256, 8262,   64, 8256,
       7490,   64, 8256, 7739,   64, 8256, 8763,   64, 8256, 9026,
          0,  -16, 7116, 8263, 8263, 8249,   64, 8263, 9156, 8257,
         64, 8389, 8772, 8387,    0,  -16, 7116, 7621, 8262, 8250
      };
      short int subbuffer078[100] = {
         64, 7875, 8643,   64, 7613, 7867, 8122, 8378, 8635, 8893,
          0,  -16, 7116, 7494, 8262, 8250,   64, 7489, 7619, 8899,
       9025,   64, 7995, 8507,    0,  -16, 7116, 7367, 7620, 8890,
         64, 8900, 7610,   64, 7878, 7491, 7361,   64, 8646, 9027,
       9153,    0,  -16, 7116, 7113, 7753, 7095,   64, 8777, 9399,
         64, 7621, 9399,   64, 8901, 7095,   64, 7753, 8777,   64,
       7621, 8901,    0,  -16, 7116, 7113, 8267, 8252,   64, 7624,
       8898,   64, 8904, 7618,   64, 7100, 7478,   64, 9404, 9014,
         64, 7100, 9404,   64, 7478, 9014,    0,  -16, 7116, 6857,
       7624, 8380,   64, 7362, 8390,   64, 6838, 9398, 9408, 6838
      };
      short int subbuffer079[100] = {
          0,  -16, 7116, 7494, 8006, 8002, 7490, 7486, 7998, 7994,
       8506, 8510, 9022, 9026, 8514, 8518, 8006,    0,  -16, 7116,
       7367, 9154, 9028, 8774, 8391, 8135, 7750, 7492, 7361, 7359,
       7484, 7738, 8121, 8377, 8762, 9020, 9150,   64, 9154, 8900,
       8645, 8389, 8132, 8003, 7873, 7871, 7997, 8124, 8379, 8635,
       8892, 9150,    0,  -16, 7116, 7367, 8264, 7356, 9148, 8264,
         64, 8248, 9156, 7364, 8248,    0,  -16, 7116, 6859, 8009,
       8011, 8140, 8396, 8523, 8521,   64, 6840, 6970, 7228, 7358,
       7490, 7495, 7624, 7881, 8649, 8904, 9031, 9026, 9150, 9276,
       9530, 9656,   64, 6840, 9656,   64, 8120, 7991, 8118, 8374
      };
      short int subbuffer080[100] = {
       8503, 8376,    0,  -16, 7116, 7240, 8261, 8255,   64, 8255,
       8118,   64, 8255, 8374,   64, 8118, 8374,   64, 8261, 8136,
       8010, 7755,   64, 8136, 7755,   64, 8261, 8392, 8522, 8779,
         64, 8392, 8779,   64, 8261, 7751, 7495, 7237,   64, 8006,
       7494, 7237,   64, 8261, 8775, 9031, 9285,   64, 8518, 9030,
       9285,   64, 8261, 8004, 7875, 7872,   64, 8261, 8003, 7872,
         64, 8261, 8516, 8643, 8640,   64, 8261, 8515, 8640,    0,
        -16, 7116, 7240, 8265, 8263,   64, 8260, 8258,   64, 8255,
       8253,   64, 8249, 8118,   64, 8249, 8374,   64, 8118, 8374,
         64, 8267, 8137, 8008,   64, 8267, 8393, 8520,   64, 8008
      };
      short int subbuffer081[100] = {
       8265, 8520,   64, 8263, 8004, 7747, 7620,   64, 8263, 8516,
       8771, 8900,   64, 7747, 8003, 8260, 8515, 8771,   64, 8258,
       7999, 7742, 7486, 7360, 7359, 7486,   64, 8258, 8511, 8766,
       9022, 9152, 9151, 9022,   64, 7742, 7998, 8255, 8510, 8766,
         64, 8253, 7994, 7865, 7608, 7480, 7353, 7227, 7225, 7480,
         64, 8253, 8506, 8633, 8888, 9016, 9145, 9275, 9273, 9016,
         64, 7608, 7864, 8249, 8632, 8888,    0,  -16, 7116, 7240,
       8249, 8118,   64, 8249, 8374,   64, 8118, 8374,   64, 8249,
       8632, 9016, 9274, 9277, 9150, 8894, 9152, 9283, 9157, 8902,
       8645, 8776, 8650, 8395, 8139, 7882, 7752, 7877, 7622, 7365
      };
      short int subbuffer082[100] = {
       7235, 7360, 7614, 7358, 7229, 7226, 7480, 7864, 8249,    0,
        -16, 7116, 7240, 8249, 8118,   64, 8249, 8374,   64, 8118,
       8374,   64, 8249, 8762, 8764, 9021, 9024, 9281, 9286, 9161,
       9034, 8778, 8523, 8011, 7754, 7498, 7369, 7238, 7233, 7488,
       7485, 7740, 7738, 8249,    0,  -16, 7116, 7113, 7106, 7360,
         64, 7495, 7746,   64, 8267, 8259,   64, 9031, 8770,   64,
       9410, 9152,    0,  -16, 7116, 8129, 8257, 8128, 8255, 8384,
       8257,    0,  -16, 7116, 8002, 8130, 8001, 7999, 8126, 8382,
       8511, 8513, 8386, 8130,    0,  -16, 7116, 7748, 8132, 7875,
       7745, 7743, 7869, 8124, 8380, 8637, 8767, 8769, 8643, 8388
      };
      short int subbuffer083[100] = {
       8132,    0,  -16, 7116, 7621, 8133, 7876, 7747, 7617, 7615,
       7741, 7868, 8123, 8379, 8636, 8765, 8895, 8897, 8771, 8644,
       8389, 8133,    0,  -16, 7116, 7367, 8135, 7750, 7492, 7361,
       7359, 7484, 7738, 8121, 8377, 8762, 9020, 9151, 9153, 9028,
       8774, 8391, 8135,    0,  -16, 7116, 6859, 8011, 7626, 7240,
       6981, 6850, 6846, 6971, 7224, 7606, 7989, 8501, 8886, 9272,
       9531, 9662, 9666, 9541, 9288, 8906, 8523, 8011,    0,  -16,
       7116, 6097, 8017, 7504, 7247, 6861, 6603, 6344, 6214, 6082,
       6078, 6202, 6328, 6581, 6835, 7217, 7472, 7983, 8495, 9008,
       9265, 9651, 9909,10168,10298,10430,10434,10310,10184, 9931
      };
      short int subbuffer084[100] = {
       9677, 9295, 9040, 8529, 8017,    0,  -16, 7116, 5462, 8022,
       7381, 6867, 6481, 6094, 5835, 5575, 5442, 5438, 5561, 5813,
       6066, 6447, 6829, 7339, 7978, 8490, 9131, 9645,10031,10418,
      10677,10937,11070,11074,10951,10699,10446,10065, 9683, 9173,
       8534, 8022,    0,  -16, 7116, 3049, 7913, 7144, 6631, 5989,
       5346, 4831, 4315, 3927, 3538, 3277, 3145, 3011, 3005, 3127,
       3251, 3502, 3881, 4261, 4769, 5278, 5915, 6553, 7064, 7831,
       8599, 9368, 9881,10523,11166,11681,12197,12585,12974,13235,
      13367,13501,13507,13385,13261,13010,12631,12251,11743,11234,
      10597, 9959, 9448, 8681, 7913,    0,  -16, 7116, 5716, 8273
      };
      short int subbuffer085[100] = {
       8015, 7630, 7246, 6863, 6609, 5706, 5960, 6085, 6072, 6197,
       6451, 6834, 7602, 7985, 8239,   64, 8273, 8527, 8910, 9294,
       9679, 9937,10826,10568,10437,10424,10293,10035, 9650, 8882,
       8497, 8239,    0,  -16, 7116, 6097, 8273, 8015, 7630, 7246,
       6863, 6609, 6219, 6087, 6082, 6206, 6458, 6838, 7474, 8239,
         64, 8273, 8527, 8910, 9294, 9679, 9937,10315,10439,10434,
      10302,10042, 9654, 9010, 8239,   64, 6218,10314,    0,  -16,
       7116, 6985, 8898, 8769, 8896, 9025, 9026, 8772, 8517, 8133,
       7748, 7490, 7359, 7357, 7482, 7736, 8119, 8375, 8760, 9018,
         64, 8133, 7876, 7618, 7487, 7485, 7610, 7864, 8119,   64
      };
      short int subbuffer086[100] = {
       7472, 9036,    0,  -16, 7116, 6986, 8268, 7351,   64, 8268,
       9143,   64, 8265, 9015,   64, 7613, 8765,   64, 7095, 7863,
         64, 8631, 9399,    0,  -16, 7116, 6859, 7500, 7479,   64,
       7628, 7607,   64, 7116, 8652, 9035, 9162, 9288, 9286, 9156,
       9027, 8642,   64, 8652, 8907, 9034, 9160, 9158, 9028, 8899,
       8642,   64, 7618, 8642, 9025, 9152, 9278, 9275, 9145, 9016,
       8631, 7095,   64, 8642, 8897, 9024, 9150, 9147, 9017, 8888,
       8631,    0,  -16, 7116, 6858, 9033, 9158, 9164, 9033, 8779,
       8396, 8140, 7755, 7497, 7367, 7236, 7231, 7356, 7482, 7736,
       8119, 8375, 8760, 9018, 9148,   64, 8140, 7883, 7625, 7495
      };
      short int subbuffer087[100] = {
       7364, 7359, 7484, 7610, 7864, 8119,    0,  -16, 7116, 6859,
       7500, 7479,   64, 7628, 7607,   64, 7116, 8396, 8779, 9033,
       9159, 9284, 9279, 9148, 9018, 8760, 8375, 7095,   64, 8396,
       8651, 8905, 9031, 9156, 9151, 9020, 8890, 8632, 8375,    0,
        -16, 7116, 6858, 7500, 7479,   64, 7628, 7607,   64, 8390,
       8382,   64, 7116, 9164, 9158, 9036,   64, 7618, 8386,   64,
       7095, 9143, 9149, 9015,    0,  -16, 7116, 6857, 7500, 7479,
         64, 7628, 7607,   64, 8390, 8382,   64, 7116, 9164, 9158,
       9036,   64, 7618, 8386,   64, 7095, 7991,    0,  -16, 7116,
       6860, 9033, 9158, 9164, 9033, 8779, 8396, 8140, 7755, 7497
      };
      short int subbuffer088[100] = {
       7367, 7236, 7231, 7356, 7482, 7736, 8119, 8375, 8760, 9018,
         64, 8140, 7883, 7625, 7495, 7364, 7359, 7484, 7610, 7864,
       8119,   64, 9023, 9015,   64, 9151, 9143,   64, 8639, 9535,
          0,  -16, 7116, 6732, 7372, 7351,   64, 7500, 7479,   64,
       9036, 9015,   64, 9164, 9143,   64, 6988, 7884,   64, 8652,
       9548,   64, 7490, 9026,   64, 6967, 7863,   64, 8631, 9527,
          0,  -16, 7116, 7622, 8268, 8247,   64, 8396, 8375,   64,
       7884, 8780,   64, 7863, 8759,    0,  -16, 7116, 7368, 8652,
       8635, 8504, 8247, 7991, 7736, 7610, 7612, 7741, 7868, 7739,
         64, 8524, 8507, 8376, 8247,   64, 8140, 9036,    0,  -16
      };
      short int subbuffer089[100] = {
       7116, 6730, 7372, 7351,   64, 7500, 7479,   64, 9164, 7487,
         64, 8131, 9143,   64, 8003, 9015,   64, 6988, 7884,   64,
       8652, 9420,   64, 6967, 7863,   64, 8631, 9399,    0,  -16,
       7116, 7113, 7756, 7735,   64, 7884, 7863,   64, 7372, 8268,
         64, 7351, 9271, 9277, 9143,    0,  -16, 7116, 6733, 7372,
       7351,   64, 7500, 8250,   64, 7372, 8247,   64, 9164, 8247,
         64, 9164, 9143,   64, 9292, 9271,   64, 6988, 7500,   64,
       9164, 9676,   64, 6967, 7735,   64, 8759, 9655,    0,  -16,
       7116, 6860, 7500, 7479,   64, 7628, 9145,   64, 7626, 9143,
         64, 9164, 9143,   64, 7116, 7628,   64, 8780, 9548,   64
      };
      short int subbuffer090[100] = {
       7095, 7863,    0,  -16, 7116, 6859, 8140, 7755, 7497, 7367,
       7235, 7232, 7356, 7482, 7736, 8119, 8375, 8760, 9018, 9148,
       9280, 9283, 9159, 9033, 8779, 8396, 8140,   64, 8140, 7883,
       7625, 7495, 7363, 7360, 7484, 7610, 7864, 8119,   64, 8375,
       8632, 8890, 9020, 9152, 9155, 9031, 8905, 8651, 8396,    0,
        -16, 7116, 6859, 7500, 7479,   64, 7628, 7607,   64, 7116,
       8652, 9035, 9162, 9288, 9285, 9155, 9026, 8641, 7617,   64,
       8652, 8907, 9034, 9160, 9157, 9027, 8898, 8641,   64, 7095,
       7991,    0,  -16, 7116, 6859, 8140, 7755, 7497, 7367, 7235,
       7232, 7356, 7482, 7736, 8119, 8375, 8760, 9018, 9148, 9280
      };
      short int subbuffer091[100] = {
       9283, 9159, 9033, 8779, 8396, 8140,   64, 8140, 7883, 7625,
       7495, 7363, 7360, 7484, 7610, 7864, 8119,   64, 8375, 8632,
       8890, 9020, 9152, 9155, 9031, 8905, 8651, 8396,   64, 7737,
       7738, 7868, 8125, 8253, 8508, 8634, 8755, 8882, 9138, 9268,
       9269,   64, 8634, 8758, 8884, 9011, 9139, 9268,    0,  -16,
       7116, 6859, 7500, 7479,   64, 7628, 7607,   64, 7116, 8652,
       9035, 9162, 9288, 9286, 9156, 9027, 8642, 7618,   64, 8652,
       8907, 9034, 9160, 9158, 9028, 8899, 8642,   64, 7095, 7991,
         64, 8258, 8513, 8640, 9017, 9144, 9272, 9401,   64, 8513,
       8639, 8888, 9015, 9271, 9401, 9402,    0,  -16, 7116, 6986
      };
      short int subbuffer092[100] = {
       9033, 9164, 9158, 9033, 8779, 8396, 8012, 7627, 7369, 7367,
       7493, 7620, 7875, 8641, 8896, 9150,   64, 7367, 7621, 7876,
       8642, 8897, 9024, 9150, 9146, 8888, 8503, 8119, 7736, 7482,
       7357, 7351, 7482,    0,  -16, 7116, 7114, 8268, 8247,   64,
       8396, 8375,   64, 7500, 7366, 7372, 9292, 9286, 9164,   64,
       7863, 8759,    0,  -16, 7116, 6732, 7372, 7357, 7482, 7736,
       8119, 8375, 8760, 9018, 9149, 9164,   64, 7500, 7485, 7610,
       7864, 8119,   64, 6988, 7884,   64, 8780, 9548,    0,  -16,
       7116, 6986, 7372, 8247,   64, 7500, 8250,   64, 9164, 8247,
         64, 7116, 7884,   64, 8652, 9420,    0,  -16, 7116, 6732
      };
      short int subbuffer093[100] = {
       7244, 7735,   64, 7372, 7740,   64, 8268, 7735,   64, 8268,
       8759,   64, 8396, 8764,   64, 9292, 8759,   64, 6860, 7756,
         64, 8908, 9676,    0,  -16, 7116, 6986, 7372, 9015,   64,
       7500, 9143,   64, 9164, 7351,   64, 7116, 7884,   64, 8652,
       9420,   64, 7095, 7863,   64, 8631, 9399,    0,  -16, 7116,
       6987, 7372, 8257, 8247,   64, 7500, 8385, 8375,   64, 9292,
       8385,   64, 7116, 7884,   64, 8780, 9548,   64, 7863, 8759,
          0,  -16, 7116, 6986, 9036, 7351,   64, 9164, 7479,   64,
       7500, 7366, 7372, 9164,   64, 7351, 9143, 9149, 9015,    0,
        -16, 7116, 6986, 8268, 7351,   64, 8268, 9143,   64, 8265
      };
      short int subbuffer094[100] = {
       9015,   64, 7613, 8765,   64, 7095, 7863,   64, 8631, 9399,
          0,  -16, 7116, 6859, 7500, 7479,   64, 7628, 7607,   64,
       7116, 8652, 9035, 9162, 9288, 9286, 9156, 9027, 8642,   64,
       8652, 8907, 9034, 9160, 9158, 9028, 8899, 8642,   64, 7618,
       8642, 9025, 9152, 9278, 9275, 9145, 9016, 8631, 7095,   64,
       8642, 8897, 9024, 9150, 9147, 9017, 8888, 8631,    0,  -16,
       7116, 7113, 7756, 7735,   64, 7884, 7863,   64, 7372, 9292,
       9286, 9164,   64, 7351, 8247,    0,  -16, 7116, 6986, 8268,
       7223,   64, 8268, 9271,   64, 8265, 9143,   64, 7352, 9144,
         64, 7223, 9271,    0,  -16, 7116, 6858, 7500, 7479,   64
      };
      short int subbuffer095[100] = {
       7628, 7607,   64, 8390, 8382,   64, 7116, 9164, 9158, 9036,
         64, 7618, 8386,   64, 7095, 9143, 9149, 9015,    0,  -16,
       7116, 6986, 9036, 7351,   64, 9164, 7479,   64, 7500, 7366,
       7372, 9164,   64, 7351, 9143, 9149, 9015,    0,  -16, 7116,
       6732, 7372, 7351,   64, 7500, 7479,   64, 9036, 9015,   64,
       9164, 9143,   64, 6988, 7884,   64, 8652, 9548,   64, 7490,
       9026,   64, 6967, 7863,   64, 8631, 9527,    0,  -16, 7116,
       6859, 8140, 7755, 7497, 7367, 7235, 7232, 7356, 7482, 7736,
       8119, 8375, 8760, 9018, 9148, 9280, 9283, 9159, 9033, 8779,
       8396, 8140,   64, 8140, 7883, 7625, 7495, 7363, 7360, 7484
      };
      short int subbuffer096[100] = {
       7610, 7864, 8119,   64, 8375, 8632, 8890, 9020, 9152, 9155,
       9031, 8905, 8651, 8396,   64, 7877, 7870,   64, 8645, 8638,
         64, 7874, 8642,   64, 7873, 8641,    0,  -16, 7116, 7622,
       8268, 8247,   64, 8396, 8375,   64, 7884, 8780,   64, 7863,
       8759,    0,  -16, 7116, 6730, 7372, 7351,   64, 7500, 7479,
         64, 9164, 7487,   64, 8131, 9143,   64, 8003, 9015,   64,
       6988, 7884,   64, 8652, 9420,   64, 6967, 7863,   64, 8631,
       9399,    0,  -16, 7116, 6986, 8268, 7351,   64, 8268, 9143,
         64, 8265, 9015,   64, 7095, 7863,   64, 8631, 9399,    0,
        -16, 7116, 6733, 7372, 7351,   64, 7500, 8250,   64, 7372
      };
      short int subbuffer097[100] = {
       8247,   64, 9164, 8247,   64, 9164, 9143,   64, 9292, 9271,
         64, 6988, 7500,   64, 9164, 9676,   64, 6967, 7735,   64,
       8759, 9655,    0,  -16, 7116, 6860, 7500, 7479,   64, 7628,
       9145,   64, 7626, 9143,   64, 9164, 9143,   64, 7116, 7628,
         64, 8780, 9548,   64, 7095, 7863,    0,  -16, 7116, 6859,
       7373, 7240,   64, 9293, 9160,   64, 7876, 7743,   64, 8772,
       8639,   64, 7355, 7222,   64, 9275, 9142,   64, 7371, 9163,
         64, 7370, 9162,   64, 7874, 8642,   64, 7873, 8641,   64,
       7353, 9145,   64, 7352, 9144,    0,  -16, 7116, 6859, 8140,
       7755, 7497, 7367, 7235, 7232, 7356, 7482, 7736, 8119, 8375
      };
      short int subbuffer098[100] = {
       8760, 9018, 9148, 9280, 9283, 9159, 9033, 8779, 8396, 8140,
         64, 8140, 7883, 7625, 7495, 7363, 7360, 7484, 7610, 7864,
       8119,   64, 8375, 8632, 8890, 9020, 9152, 9155, 9031, 8905,
       8651, 8396,    0,  -16, 7116, 6732, 7372, 7351,   64, 7500,
       7479,   64, 9036, 9015,   64, 9164, 9143,   64, 6988, 9548,
         64, 6967, 7863,   64, 8631, 9527,    0,  -16, 7116, 6859,
       7500, 7479,   64, 7628, 7607,   64, 7116, 8652, 9035, 9162,
       9288, 9285, 9155, 9026, 8641, 7617,   64, 8652, 8907, 9034,
       9160, 9157, 9027, 8898, 8641,   64, 7095, 7991,    0,  -16,
       7116, 6987, 7372, 8258, 7223,   64, 7244, 8130,   64, 7244
      };
      short int subbuffer099[100] = {
       9164, 9286, 9036,   64, 7352, 9016,   64, 7223, 9143, 9277,
       9015,    0,  -16, 7116, 7114, 8268, 8247,   64, 8396, 8375,
         64, 7500, 7366, 7372, 9292, 9286, 9164,   64, 7863, 8759,
          0,  -16, 7116, 7114, 7367, 7369, 7499, 7628, 7884, 8011,
       8137, 8261, 8247,   64, 7369, 7627, 7883, 8137,   64, 9287,
       9289, 9163, 9036, 8780, 8651, 8521, 8389, 8375,   64, 9289,
       9035, 8779, 8521,   64, 7863, 8759,    0,  -16, 7116, 6987,
       8268, 8247,   64, 8396, 8375,   64, 8007, 7622, 7493, 7363,
       7360, 7486, 7613, 7996, 8636, 9021, 9150, 9280, 9283, 9157,
       9030, 8647, 8007,   64, 8007, 7750, 7621, 7491, 7488, 7614
      };
