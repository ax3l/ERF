#ifndef _TERRAIN_METRIC_H_
#define _TERRAIN_METRIC_H_

#include <AMReX.H>
#include <AMReX_Geometry.H>
#include <AMReX_MultiFab.H>
#include <IndexDefines.H>

// Declare function for ERF.cpp
void init_terrain_grid( amrex::Geometry& geom, amrex::MultiFab& z_phys_nd);

//*****************************************************************************************
// Compute terrain metric terms at cell-center
//*****************************************************************************************
// Metric is at cell center
AMREX_GPU_DEVICE
inline amrex::Real
Compute_h_zeta_AtCellCenter(const int &i, const int &j, const int &k,
                            const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                            const amrex::Array4<const amrex::Real>& z_nd)
{
    amrex::Real dzInv = cellSizeInv[2];
    amrex::Real met_h_zeta = 0.25 * dzInv *
      ( z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1) + z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
       -z_nd(i+1,j,k  ) - z_nd(i+1,j+1,k  ) - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
    return met_h_zeta;
}

// Metric is at cell center
AMREX_GPU_DEVICE
inline void
ComputeMetricAtCellCenter(const int &i, const int &j, const int &k,
                          amrex::Real& met_h_xi,
                          amrex::Real& met_h_eta,
                          amrex::Real& met_h_zeta,
                          const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                          const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi   = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) - z_nd(i  ,j,k+1) - z_nd(i  ,j+1,k+1) );
    break;
  case TerrainMet::h_eta:
    met_h_eta  = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) - z_nd(i,j  ,k+1) - z_nd(i+1,j  ,k+1) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = 0.25 * dzInv *
      ( z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1) + z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
       -z_nd(i+1,j,k  ) - z_nd(i+1,j+1,k  ) - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi   = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) - z_nd(i  ,j,k+1) - z_nd(i  ,j+1,k+1) );
    met_h_eta  = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) - z_nd(i,j  ,k+1) - z_nd(i+1,j  ,k+1) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi   = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) - z_nd(i  ,j,k+1) - z_nd(i  ,j+1,k+1) );
    met_h_zeta = 0.25 * dzInv *
      ( z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1) + z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
       -z_nd(i+1,j,k  ) - z_nd(i+1,j+1,k  ) - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
     break;
  case TerrainMet::h_eta_zeta:
    met_h_eta  = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) - z_nd(i,j  ,k+1) - z_nd(i+1,j  ,k+1) );
    met_h_zeta = 0.25 * dzInv *
      ( z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1) + z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
       -z_nd(i+1,j,k  ) - z_nd(i+1,j+1,k  ) - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
    break;
  default:
    met_h_xi   = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) - z_nd(i  ,j,k+1) - z_nd(i  ,j+1,k+1) );
    met_h_eta  = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) - z_nd(i,j  ,k+1) - z_nd(i+1,j  ,k+1) );
    met_h_zeta = 0.25 * dzInv *
      ( z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k+1) + z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
       -z_nd(i+1,j,k  ) - z_nd(i+1,j+1,k  ) - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
  }
}

//*****************************************************************************************
// Compute terrain metric terms at face-centers
//*****************************************************************************************
// Metric coincides with U location
AMREX_GPU_DEVICE
inline void
ComputeMetricAtIface(const int &i, const int &j, const int &k,
                   amrex::Real& met_h_xi,
                   amrex::Real& met_h_eta,
                   amrex::Real& met_h_zeta,
                   const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                   const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi   = 0.125 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) - z_nd(i-1,j+1,k) - z_nd(i-1,j+1,k+1) );
    break;
  case TerrainMet::h_eta:
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k  ) + z_nd(i,j+1,k+1)
                               - z_nd(i,j  ,k  ) - z_nd(i,j  ,k+1) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                               - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi   = 0.125 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) - z_nd(i-1,j+1,k) - z_nd(i-1,j+1,k+1) );
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k  ) + z_nd(i,j+1,k+1)
                               - z_nd(i,j  ,k  ) - z_nd(i,j  ,k+1) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi   = 0.125 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) - z_nd(i-1,j+1,k) - z_nd(i-1,j+1,k+1) );
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                               - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
     break;
  case TerrainMet::h_eta_zeta:
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k  ) + z_nd(i,j+1,k+1)
                               - z_nd(i,j  ,k  ) - z_nd(i,j  ,k+1) );
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                               - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
    break;
  default:
    met_h_xi   = 0.125 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) - z_nd(i-1,j+1,k) - z_nd(i-1,j+1,k+1) );
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k  ) + z_nd(i,j+1,k+1)
                               - z_nd(i,j  ,k  ) - z_nd(i,j  ,k+1) );
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                               - z_nd(i,j,k  ) - z_nd(i,j+1,k  ) );
  }
}

// Metric coincides with V location
AMREX_GPU_DEVICE
inline void
ComputeMetricAtJface(const int &i, const int &j, const int &k,
                   amrex::Real& met_h_xi,
                   amrex::Real& met_h_eta,
                   amrex::Real& met_h_zeta,
                   const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                   const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
                                -z_nd(i  ,j,k) - z_nd(i  ,j,k+1) );
    break;
  case TerrainMet::h_eta:
    met_h_eta  = 0.125 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) - z_nd(i+1,j-1,k) - z_nd(i+1,j-1,k+1) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                               - z_nd(i,j,k  ) - z_nd(i+1,j,k  ) );
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
                                -z_nd(i  ,j,k) - z_nd(i  ,j,k+1) );
    met_h_eta  = 0.125 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) - z_nd(i+1,j-1,k) - z_nd(i+1,j-1,k+1) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
                                -z_nd(i  ,j,k) - z_nd(i  ,j,k+1) );
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                               - z_nd(i,j,k  ) - z_nd(i+1,j,k  ) );
     break;
  case TerrainMet::h_eta_zeta:
    met_h_eta  = 0.125 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) - z_nd(i+1,j-1,k) - z_nd(i+1,j-1,k+1) );
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                               - z_nd(i,j,k  ) - z_nd(i+1,j,k  ) );
    break;
  default:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
                                -z_nd(i  ,j,k) - z_nd(i  ,j,k+1) );
    met_h_eta  = 0.125 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) - z_nd(i+1,j-1,k) - z_nd(i+1,j-1,k+1) );
    met_h_zeta = 0.5 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                               - z_nd(i,j,k  ) - z_nd(i+1,j,k  ) );
  }
}

// Metric coincides with W location
AMREX_GPU_DEVICE
inline void
ComputeMetricAtKface(const int &i, const int &j, const int &k,
                   amrex::Real& met_h_xi,
                   amrex::Real& met_h_eta,
                   amrex::Real& met_h_zeta,
                   const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                   const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k)
                                -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) );
    break;
  case TerrainMet::h_eta:
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k)
                                -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = 0.125 * dzInv *
      ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) - z_nd(i,j+1,k-1) - z_nd(i+1,j+1,k-1) );
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k)
                                -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) );
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k)
                                -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k)
                                -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) );
    met_h_zeta = 0.125 * dzInv *
      ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) - z_nd(i,j+1,k-1) - z_nd(i+1,j+1,k-1) );
     break;
  case TerrainMet::h_eta_zeta:
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k)
                                -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) );
    met_h_zeta = 0.125 * dzInv *
      ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) - z_nd(i,j+1,k-1) - z_nd(i+1,j+1,k-1) );
    break;
  default:
    met_h_xi   = 0.5 * dxInv * ( z_nd(i+1,j,k) + z_nd(i+1,j+1,k)
                                -z_nd(i  ,j,k) - z_nd(i  ,j+1,k) );
    met_h_eta  = 0.5 * dyInv * ( z_nd(i,j+1,k) + z_nd(i+1,j+1,k)
                                -z_nd(i,j  ,k) - z_nd(i+1,j  ,k) );
    met_h_zeta = 0.125 * dzInv *
      ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
       -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) - z_nd(i,j+1,k-1) - z_nd(i+1,j+1,k-1) );
  }
}

//*****************************************************************************************
// Compute terrain metric terms at edge-centers
//*****************************************************************************************
// Metric is at edge and center Z (red pentagon)
AMREX_GPU_DEVICE
inline void
ComputeMetricAtEdgeCenterK(const int &i, const int &j, const int &k,
                           amrex::Real& met_h_xi,
                           amrex::Real& met_h_eta,
                           amrex::Real& met_h_zeta,
                           const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                           const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi  = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) );
    break;
  case TerrainMet::h_eta:
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = dzInv * (z_nd(i,j,k+1) - z_nd(i,j,k));
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi  = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) );
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi  = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) );
    met_h_zeta = dzInv * (z_nd(i,j,k+1) - z_nd(i,j,k));
     break;
  case TerrainMet::h_eta_zeta:
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) );
    met_h_zeta = dzInv * (z_nd(i,j,k+1) - z_nd(i,j,k));
    break;
  default:
    met_h_xi  = 0.25 * dxInv *
      ( z_nd(i+1,j,k) + z_nd(i+1,j,k+1)
       -z_nd(i-1,j,k) - z_nd(i-1,j,k+1) );
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i,j+1,k) + z_nd(i,j+1,k+1)
       -z_nd(i,j-1,k) - z_nd(i,j-1,k+1) );
    met_h_zeta = dzInv * (z_nd(i,j,k+1) - z_nd(i,j,k));
  }
}

// Metric is at edge and center Y (magenta cross)
AMREX_GPU_DEVICE
inline void
ComputeMetricAtEdgeCenterJ(const int &i, const int &j, const int &k,
                           amrex::Real& met_h_xi,
                           amrex::Real& met_h_eta,
                           amrex::Real& met_h_zeta,
                           const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                           const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi = 0.25 * dxInv *
      ( z_nd(i+1,j+1,k) + z_nd(i+1,j  ,k)
       -z_nd(i-1,j+1,k) - z_nd(i-1,j  ,k) );
    break;
  case TerrainMet::h_eta:
    met_h_eta = dyInv * ( z_nd(i,j+1,k) - z_nd(i,j,k) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i,j+1,k-1) );
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi = 0.25 * dxInv *
      ( z_nd(i+1,j+1,k) + z_nd(i+1,j  ,k)
       -z_nd(i-1,j+1,k) - z_nd(i-1,j  ,k) );
    met_h_eta = dyInv * ( z_nd(i,j+1,k) - z_nd(i,j,k) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi = 0.25 * dxInv *
      ( z_nd(i+1,j+1,k) + z_nd(i+1,j  ,k)
       -z_nd(i-1,j+1,k) - z_nd(i-1,j  ,k) );
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i,j+1,k-1) );
     break;
  case TerrainMet::h_eta_zeta:
    met_h_eta = dyInv * ( z_nd(i,j+1,k) - z_nd(i,j,k) );
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i,j+1,k-1) );
    break;
  default:
    met_h_xi = 0.25 * dxInv *
      ( z_nd(i+1,j+1,k) + z_nd(i+1,j  ,k)
       -z_nd(i-1,j+1,k) - z_nd(i-1,j  ,k) );
    met_h_eta = dyInv * ( z_nd(i,j+1,k) - z_nd(i,j,k) );
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i,j+1,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i,j+1,k-1) );
  }
}

// Metric is at edge and center I (purple hexagon)
AMREX_GPU_DEVICE
inline void
ComputeMetricAtEdgeCenterI(const int &i, const int &j, const int &k,
                           amrex::Real& met_h_xi,
                           amrex::Real& met_h_eta,
                           amrex::Real& met_h_zeta,
                           const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& cellSizeInv,
                           const amrex::Array4<const amrex::Real>& z_nd, const int flag)
{
  met_h_xi   = 0.0;
  met_h_eta  = 0.0;
  met_h_zeta = 0.0;
  amrex::Real dxInv = cellSizeInv[0];
  amrex::Real dyInv = cellSizeInv[1];
  amrex::Real dzInv = cellSizeInv[2];

  switch(flag) {
  case TerrainMet::h_xi:
    met_h_xi  = dxInv * ( z_nd(i+1,j,k) - z_nd(i,j,k) );
    break;
  case TerrainMet::h_eta:
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i+1,j+1,k) + z_nd(i,j+1,k)
       -z_nd(i+1,j-1,k) - z_nd(i,j-1,k) );
    break;
  case TerrainMet::h_zeta:
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) );
    break;
  case TerrainMet::h_xi_eta:
    met_h_xi  = dxInv * ( z_nd(i+1,j,k) - z_nd(i,j,k) );
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i+1,j+1,k) + z_nd(i,j+1,k)
       -z_nd(i+1,j-1,k) - z_nd(i,j-1,k) );
    break;
  case TerrainMet::h_xi_zeta:
    met_h_xi  = dxInv * ( z_nd(i+1,j,k) - z_nd(i,j,k) );
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) );
    break;
  case TerrainMet::h_eta_zeta:
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i+1,j+1,k) + z_nd(i,j+1,k)
       -z_nd(i+1,j-1,k) - z_nd(i,j-1,k) );
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) );
    break;
  default:
    met_h_xi  = dxInv * ( z_nd(i+1,j,k) - z_nd(i,j,k) );
    met_h_eta = 0.25 * dyInv *
      ( z_nd(i+1,j+1,k) + z_nd(i,j+1,k)
       -z_nd(i+1,j-1,k) - z_nd(i,j-1,k) );
    met_h_zeta = 0.25 * dzInv * ( z_nd(i,j,k+1) + z_nd(i+1,j,k+1)
                                 -z_nd(i,j,k-1) - z_nd(i+1,j,k-1) );
  }
}

//*****************************************************************************************
// Map between W <--> Omega
//*****************************************************************************************
AMREX_GPU_DEVICE
inline
amrex::Real OmegaFromW(int i, int j, int k, amrex::Real w,
                       const amrex::Array4<const amrex::Real> u,
                       const amrex::Array4<const amrex::Real> v,
                       const amrex::Array4<const amrex::Real> z_nd,
                       const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& dxInv)
{
  // This is dh/dxi at z-face (i,j,k-1/2)
  amrex::Real met_zlo_xi   = 0.5 * dxInv[0] *
    ( z_nd(i+1,j+1,k  ) + z_nd(i+1,j  ,k  )    // hi i, lo k
     -z_nd(i  ,j+1,k  ) - z_nd(i  ,j  ,k  ) ); // lo i, lo k

  // This is dh/deta at z-face (i,j,k-1/2)
  amrex::Real met_zlo_eta  = 0.5 * dxInv[1] *
    ( z_nd(i+1,j+1,k  ) + z_nd(i  ,j+1,k  )    // hi j, lo k
     -z_nd(i+1,j  ,k  ) - z_nd(i  ,j  ,k  ) ); // lo j, lo k

  amrex::Real omega;
  if (k == 0) {
      omega = 0.;
  } else {
      amrex::Real vec_zlo_xi   = 0.25 * ( u(i,j,k-1) + u(i+1,j,k-1) + u(i,j,k) + u(i+1,j,k) );
      amrex::Real vec_zlo_eta  = 0.25 * ( v(i,j,k-1) + v(i,j+1,k-1) + v(i,j,k) + v(i,j+1,k) );
      omega =  -met_zlo_xi * vec_zlo_xi - met_zlo_eta * vec_zlo_eta + w;
  }
  return omega;
}

AMREX_GPU_DEVICE AMREX_FORCE_INLINE
amrex::Real WFromOmega(int i, int j, int k, amrex::Real omega,
                       const amrex::Array4<const amrex::Real>& u,
                       const amrex::Array4<const amrex::Real>& v,
                       const amrex::Array4<const amrex::Real>& z_nd,
                       const amrex::GpuArray<amrex::Real, AMREX_SPACEDIM>& dxInv)
{
  // This is dh/dxi at z-face (i,j,k-1/2)
  amrex::Real met_zlo_xi   = 0.5 * dxInv[0] *
    ( z_nd(i+1,j+1,k  ) + z_nd(i+1,j  ,k  )    // hi i, lo k
     -z_nd(i  ,j+1,k  ) - z_nd(i  ,j  ,k  ) ); // lo i, lo k

  // This is dh/deta at z-face (i,j,k-1/2)
  amrex::Real met_zlo_eta  = 0.5 * dxInv[1] *
    ( z_nd(i+1,j+1,k  ) + z_nd(i  ,j+1,k  )    // hi j, lo k
     -z_nd(i+1,j  ,k  ) - z_nd(i  ,j  ,k  ) ); // lo j, lo k

  // Use extrapolation instead of interpolation if at the bottom boundary
  amrex::Real vec_zlo_xi   = (k == 0) ? 1.5 * (0.5*(u(i,j,k)+u(i+1,j,k))) - 0.5*(0.5*(u(i,j,k+1)+u(i+1,j,k+1))) :
    0.25 * ( u(i,j,k-1) + u(i+1,j,k-1) + u(i,j,k) + u(i+1,j,k) );
  amrex::Real vec_zlo_eta  = (k == 0) ? 1.5 * (0.5*(v(i,j,k)+v(i,j+1,k))) - 0.5*(0.5*(v(i,j,k+1)+v(i,j+1,k+1))) :
    0.25 * ( v(i,j,k-1) + v(i,j+1,k-1) + v(i,j,k) + v(i,j+1,k) );

  amrex::Real w = met_zlo_xi * vec_zlo_xi + met_zlo_eta * vec_zlo_eta + omega;
  return w;
}

//*****************************************************************************************
// Compute detJ & z_phys at cell-center
//*****************************************************************************************
inline
void make_metrics(amrex::Geometry& geom,
                  amrex::MultiFab& z_phys_nd,
                  amrex::MultiFab& z_phys_cc,
                  amrex::MultiFab& detJ_cc)
{
    auto dx = geom.CellSize();
    amrex::Real dzInv = 1.0/dx[2];

    // Domain valid box (z_nd is nodal)
    const amrex::Box& domain = geom.Domain();
    int domlo_z = domain.smallEnd(2);

    // Number of ghost cells
    int ngrow = z_phys_cc.nGrow();

#ifdef _OPENMP
#pragma omp parallel if (amrex::Gpu::notInLaunchRegion())
#endif
    for ( amrex::MFIter mfi(z_phys_cc, amrex::TilingIfNotGPU()); mfi.isValid(); ++mfi )
    {
        const amrex::Box& gbx = mfi.growntilebox(ngrow);
        amrex::Array4<amrex::Real const> z_nd = z_phys_nd.const_array(mfi);
        amrex::Array4<amrex::Real      > z_cc = z_phys_cc.array(mfi);
        amrex::Array4<amrex::Real      > detJ = detJ_cc.array(mfi);
        amrex::ParallelFor(gbx, [=] AMREX_GPU_DEVICE(int i, int j, int k) noexcept {
            if (k >= domlo_z) {
               z_cc(i, j, k) = .125 * (
                       z_nd(i,j,k  ) + z_nd(i+1,j,k  ) + z_nd(i,j+1,k  ) + z_nd(i+1,j+1,k  )
                      +z_nd(i,j,k+1) + z_nd(i+1,j,k+1) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1) );
               detJ(i, j, k) = .25 * dzInv * (
                       z_nd(i,j,k+1) + z_nd(i+1,j,k+1) + z_nd(i,j+1,k+1) + z_nd(i+1,j+1,k+1)
                      -z_nd(i,j,k  ) - z_nd(i+1,j,k  ) - z_nd(i,j+1,k  ) - z_nd(i+1,j+1,k  ) );
            }
       });
    }
    detJ_cc.FillBoundary(geom.periodicity());
}
#endif
