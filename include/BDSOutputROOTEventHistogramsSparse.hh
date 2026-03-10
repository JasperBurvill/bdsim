/*
Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway,
University of London 2001 - 2024.

This file is part of BDSIM.

BDSIM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation version 3 of the License.

BDSIM is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BDSIM.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef BDSOUTPUTROOTEVENTHISTOGRAMSSPARSE_H
#define BDSOUTPUTROOTEVENTHISTOGRAMSSPARSE_H

#ifndef __ROOTBUILD__
#include "globals.hh"
#endif

#include "TObject.h"
#include "THnSparse.h"

class THnSparse;

/**
 * @brief Holder for a set of sparse histograms to be stored.
 *
 * @author Jasper Burvill
 */

class BDSOutputROOTEventHistogramsSparse: public TObject
{
public:
  BDSOutputROOTEventHistogramsSparse() = default;
  BDSOutputROOTEventHistogramsSparse(const BDSOutputROOTEventHistogramsSparse &h);
  BDSOutputROOTEventHistogramsSparse operator=(const BDSOutputROOTEventHistogramsSparse&) = delete;
  BDSOutputROOTEventHistogramsSparse(std::vector<THnSparseD*>& histogramIn);
  virtual ~BDSOutputROOTEventHistogramsSparse();

  /// Interface function to create a 1D histogram using only standard types.
  int Create1DHistogramSTD(std::string name, std::string title, int nbins, double xmin, double xmax);
#ifndef __ROOTBUILD__
  G4int Create1DHistogram(G4String name, G4String title, G4int nbins, G4double xmin, G4double xmax);
  G4int Create1DHistogram(G4String name, G4String title, std::vector<double>& edges);
  void Fill1DHistogram(G4int histoId, G4double value, G4double weight = 1.0);
#endif
  /// Flush the contents.
  virtual void Flush();

  /// Copy (using the TH->Clone) method from another instance.
  void Fill(const BDSOutputROOTEventHistogramsSparse* rhs);

  /// Copy (without using the TH->Clone) method from another instance. (Quicker).
  void FillSimple(const BDSOutputROOTEventHistogramsSparse* rhs);

  /// @{ Accessors.
  std::vector<THnSparseD*>& Get1DHistograms() {return histograms1D;}
  THnSparseD* Get1DHistogram(int iHisto) const {return histograms1D[iHisto];}
  /// @}

private:
  std::vector<THnSparseD*> histograms1D;

  ClassDef(BDSOutputROOTEventHistogramsSparse,4)
};
#endif