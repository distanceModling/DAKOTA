############################################################################
#
# CMake configuration file for RHEL6 platform
#
############################################################################


############################################################################
# Make 2.8.6 has problems with RHEL6/Boost -- the following is a workaround
#
#set( Boost_NO_BOOST_CMAKE ON 
#     CACHE BOOL "Obtain desired behavior on RHEL6" FORCE)
#
# Boost_NO_BOOST_CMAKE
#     Do not do a find_package call in config mode before searching
#     for a regular boost install. This will avoid finding boost-cmake
#     installs. Defaults to OFF.                 [Since CMake 2.8.6]
############################################################################

############################################################################
# BLAS, LAPACK libraries assumed to be installed in /usr/lib64 on RHEL6
#
#set( BLAS_LIBS 
#      "/usr/lib64"
#      CACHE FILEPATH "Use non-standard BLAS library path" FORCE )
#set( LAPACK_LIBS 
#      "/usr/lib64"
#      CACHE FILEPATH "Use non-standard BLAS library path" FORCE )
############################################################################

# BMA: Not sure this is the right place to make these settings...
set( DAKOTA_HAVE_MPI OFF
     CACHE BOOL "Cygwin is serial build" FORCE)

# Use shared libraries and don't use static
set(BUILD_STATIC_LIBS TRUE  CACHE BOOL "Build static libs?")
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Build shared libs?")