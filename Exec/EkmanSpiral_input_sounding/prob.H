#ifndef _PROB_H_
#define _PROB_H_

#include <AMReX_REAL.H>

struct ProbParm {
  amrex::Real rho_0 = 1.0;
  amrex::Real Theta_0 = 300.0;
  amrex::Real V_0 = 1.0;
}; // namespace ProbParm

extern ProbParm parms;

#endif
