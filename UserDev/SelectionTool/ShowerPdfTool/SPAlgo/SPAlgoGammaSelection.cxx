#ifndef SELECTIONTOOL_SPALGOGAMMASELECTION_CXX
#define SELECTIONTOOL_SPALGOGAMMASELECTION_CXX

#include "SPAlgoGammaSelection.h"

namespace sptool {

  SPAlgoGammaSelection::SPAlgoGammaSelection() : SPAlgoBase()
  {
    _name     = "SPAlgoGammaSelection";
    _nshowers = 1;
  }

  void SPAlgoGammaSelection::Init(double xmin, double xmax, double lmin, double lmax)
  {
    _factory = new ShowerPdfFactory();
    _vars.reserve(2);
    _vars.push_back(RooRealVar("_RadLenx","Distance [cm]",xmin,xmax));
    _vars.push_back(RooRealVar("_RadLenl","Radiation length [cm]",lmin,lmax));
    _pdf = _factory->RadLenPdf(_vars);
    _data = new RooDataSet("_data","RooFit distance data set",RooArgSet(_vars[0]));
  }
  
  float SPAlgoGammaSelection::Select(const SPAData &data)
  {
    
  }

  void SPAlgoGammaSelection::Fill(const SPAData &data)
  {
    for ( auto &a : data._showers ){
      double x = a.Start().Dist(data._vtxs[0]);
      std::cout << "Distance: " << x << std::endl;
      _vars[0].setVal(x);
      _data->fill();
    }
  }

  void SPAlgoGammaSelection::Fit(TFile* fout)
  {}

}

#endif
