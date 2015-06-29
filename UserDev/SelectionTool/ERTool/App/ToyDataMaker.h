/**
 * \file ToyDataMaker.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class ToyDataMaker
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/
#ifndef TOYDATAMAKER_H
#define TOYDATAMAKER_H

#include <iostream>
#include "ERTool/Base/Shower.h"
#include "ERTool/Base/Track.h"
#include "ERTool/Base/UtilFunc.h"
namespace ertool {
  /**
     \class ToyDataMaker
     User defined class ToyDataMaker ... these comments are used to generate
     doxygen documentation!
  */
  class ToyDataMaker{
    
  public:
    
    /// Default constructor
    ToyDataMaker(){}
    
    /// Default destructor
    ~ToyDataMaker(){}

    /// Track maker
    Track MakeTrack(double x,  double y,  double z,
		    double px, double py, double pz,
		    int pdg_code)
    {
      Track t;
      t.push_back(::geoalgo::Vector(x,y,z));
      auto mass = ParticleMass(pdg_code);
      t._energy = sqrt(pow(mass,2) + pow(px,2) + pow(py,2) + pow(pz,2) );
      return t;
    }
    
    /// Shower adder
    Shower MakeShower(double x, double y, double z,
		      double px, double py, double pz,
		      int pdg_code)
    {
      Shower s( geoalgo::Vector(x,y,z),
		geoalgo::Vector(px,py,pz),
		30, 15);
      s._energy = 100;
      s._dedx = 2;
      return s;
    }

    /// Vtx sigma
    void SetVertexSigma(const double s)
    { _vtx_sigma = s; }

    /// Trajectory parameter
    void SetTrajectoryParams(const double step_size   = 5, // cm
			     const double step_sigma  = 1, // cm
			     const double angle_sigma = 5, // degree
			     const double dedx = 2 // MeV/cm 
			     )
    {
      _track_step_size  = step_size;
      _track_step_sigma = step_sigma;
      _track_step_angle_sigma = angle_sigma;
      _track_step_dedx = dedx;
    }

    /// Shower parameter
    void SetShowerParams(const double length, // cm
			 const double dedx=2  // MeV/cm
			 )
    {
      _shower_length = length;
      _shower_dedx   = dedx;
    }

    /// Clear added elements
    void Clear()
    { _shower_v.clear(); _track_v.clear(); }

  private:

    std::vector< ertool::Shower > _shower_v;
    std::vector< ertool::Track  > _track_v;

    /// vtx point smearing
    double _vtx_sigma;

    //
    // Track params
    //
    double _track_step_size;        ///< step size of a trajectory in cm 
    double _track_step_sigma;       ///< that for sigma 
    double _track_step_angle_sigma; ///< angle obviously in degree
    double _track_step_dedx;        ///< guess your unit

    // Shower params
    double _shower_length;
    double _shower_dedx;

  };
}
#endif
/** @} */ // end of doxygen group 

