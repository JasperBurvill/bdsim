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
  BDSOutputROOTEventHistogramsSparse(std::vector<THnSparseD*>& histograms1DIn,
             std::vector<THnSparseD*>& histograms2DIn,
             std::vector<THnSparseD*>& histograms3DIn);
  virtual ~BDSOutputROOTEventHistogramsSparse();

  /// Interface function to create a 1D histogram using only standard types.
  int Create1DHistogramSTD(std::string name, std::string title,
          int nbins, double xmin, double xmax);

#ifndef __ROOTBUILD__
  G4int Create1DHistogram(G4String name, G4String title,
                          G4int nbins, G4double xmin, G4double xmax);
  G4int Create1DHistogram(G4String name, G4String title,
                          std::vector<double>& edges);
  G4int Create2DHistogram(G4String name, G4String title,
                          G4int nxbins, G4double xmin, G4double xmax,
                          G4int nybins, G4double ymin, G4double ymax);
  G4int Create2DHistogram(G4String name, G4String title,
                          std::vector<double>& xedges,
                          std::vector<double>& yedges);
  G4int Create3DHistogram(G4String name, G4String title,
        G4int nxbins, G4double xmin, G4double xmax,
        G4int nybins, G4double ymin, G4double ymax,
        G4int nzbins, G4double zmin, G4double zmax);
  G4int Create3DHistogram(G4String name, G4String title,
        std::vector<double>& xedges,
        std::vector<double>& yedges,
        std::vector<double>& zedges);

  void Fill1DHistogram(G4int histoId, G4double value, G4double weight = 1.0);
  void Fill2DHistogram(G4int histoId, G4double xValue, G4double yValue, G4double weight = 1.0);
  void Fill3DHistogram(G4int histoId, G4double xValue, G4double yValue, G4double zValue, G4double weight = 1.0);

  /// Set the value of a bin by (ROOT!!) global bin index.
  void Set3DHistogramBinContent(G4int    histoId,
        G4int    globalBinID,
        G4double value);

  /// Add the values from one supplied 3D histogram to another. Uses TH3-Add().
  void AccumulateHistogram3D(G4int histoId,
           THnSparseD* otherHistogram);

#endif
  /// Flush the contents.
  virtual void Flush();

  /// Copy (using the TH->Clone) method from another instance.
  void Fill(const BDSOutputROOTEventHistogramsSparse* rhs);

  /// Copy (without using the TH->Clone) method from another instance. (Quicker).
  void FillSimple(const BDSOutputROOTEventHistogramsSparse* rhs);

  /// @{ Accessors.
  std::vector<THnSparseD*>& Get1DHistograms() {return histograms1D;}
  std::vector<THnSparseD*>& Get2DHistograms() {return histograms2D;}
  std::vector<THnSparseD*>& Get3DHistograms() {return histograms3D;}
  THnSparseD* Get1DHistogram(int iHisto) const {return histograms1D[iHisto];}
  THnSparseD* Get2DHistogram(int iHisto) const {return histograms2D[iHisto];}
  THnSparseD* Get3DHistogram(int iHisto) const {return histograms3D[iHisto];}
  /// @}

private:
  std::vector<THnSparseD*> histograms1D;
  std::vector<THnSparseD*> histograms2D;
  std::vector<THnSparseD*> histograms3D;

  ClassDef(BDSOutputROOTEventHistogramsSparse,4)
};
#endif