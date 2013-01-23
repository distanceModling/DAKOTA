/*  _______________________________________________________________________

    DAKOTA: Design Analysis Kit for Optimization and Terascale Applications
    Copyright (c) 2010, Sandia National Laboratories.
    This software is distributed under the GNU Lesser General Public License.
    For more information, see the README file in the top Dakota directory.
    _______________________________________________________________________ */

//- Class:	 NonDLHSSingleInterval
//- Description: Class for LHS-based single interval propagation in DAKOTA/UQ
//- Owner:	 Laura Swiler
//- Checked by:
//- Version:

#ifndef NOND_LHS_SINGLE_INTERVAL_H
#define NOND_LHS_SINGLE_INTERVAL_H

#include "data_types.hpp"
#include "NonDLHSInterval.hpp"
#include "DakotaModel.hpp"
#include "DakotaIterator.hpp"

namespace Dakota {


/// Class for pure interval propagation using LHS

/** The NonDSingleInterval class implements the propagation of epistemic
    uncertainty using ... */

class NonDLHSSingleInterval: public NonDLHSInterval
{
public:

  //
  //- Heading: Constructors and destructor
  //

  NonDLHSSingleInterval(Model& model); ///< constructor
  ~NonDLHSSingleInterval();            ///< destructor

protected:

  //
  //- Heading: virtual function redefinitions
  //

  void initialize();
  void post_process_samples();

private:

  //
  // - Heading: Data
  //

  /// counter for finalStatistics
  size_t statCntr;
};

} // namespace Dakota

#endif