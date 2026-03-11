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
#include "BDSOutputROOTEventHistogramsSparse.hh"

#include "TAxis.h"
#include "THnSparse.h"
#include "BDSDebug.hh"
#include "BDSException.hh"

ClassImp(BDSOutputROOTEventHistogramsSparse)


BDSOutputROOTEventHistogramsSparse::BDSOutputROOTEventHistogramsSparse(const BDSOutputROOTEventHistogramsSparse& rhs):
  TObject(rhs)
{
  Fill(&rhs);
}

BDSOutputROOTEventHistogramsSparse::BDSOutputROOTEventHistogramsSparse(std::vector<THnSparseD*>& histograms1DIn,
                                                                       std::vector<THnSparseD*>& histograms2DIn,
                                                                       std::vector<THnSparseD*>& histograms3DIn):
  histograms1D(histograms1DIn),
  histograms2D(histograms2DIn),
  histograms3D(histograms3DIn)
{;}

BDSOutputROOTEventHistogramsSparse::~BDSOutputROOTEventHistogramsSparse()
{
  // Apparently I need this as memory management is different for THnSparse
  for (auto h : histograms1D) {delete h;}
  for (auto h : histograms2D) {delete h;}
  for (auto h : histograms3D) {delete h;}
}

void BDSOutputROOTEventHistogramsSparse::FillSimple(const BDSOutputROOTEventHistogramsSparse* rhs)
{
  if (!rhs)
    {return;}

  histograms1D = rhs->histograms1D;
  histograms2D = rhs->histograms2D;
  histograms3D = rhs->histograms3D;
}

void BDSOutputROOTEventHistogramsSparse::Fill(const BDSOutputROOTEventHistogramsSparse* rhs)
{
  if (!rhs) { return; }

  // for each histogram, clone (ie copy) it into this object
  for (auto h : rhs->histograms1D)
    {histograms1D.push_back(static_cast<THnSparseD*>(h->Clone()));}
  for (auto h : rhs->histograms2D)
    {histograms2D.push_back(static_cast<THnSparseD*>(h->Clone()));}
  for (auto h : rhs->histograms3D)
    {histograms3D.push_back(static_cast<THnSparseD*>(h->Clone()));}
}

int BDSOutputROOTEventHistogramsSparse::Create1DHistogramSTD(std::string name, std::string title,
                                                             int nbins, double xmin, double xmax)
{
  Int_t bins[1] = {nbins};
  Double_t mins[1] = {xmin};
  Double_t maxs[1] = {xmax};
  histograms1D.push_back(new THnSparseD(name.c_str(), title.c_str(), 1, bins, mins, maxs));
  return (int)histograms1D.size() - 1;
}

#ifndef __ROOTBUILD__

G4int BDSOutputROOTEventHistogramsSparse::Create1DHistogram(G4String name, G4String title,
                                                            G4int nbins, G4double xmin, G4double xmax)
{
  Int_t bins[1] = {nbins};
  Double_t mins[1] = {xmin};
  Double_t maxs[1] = {xmax};
  histograms1D.push_back(new THnSparseD(name.c_str(), title.c_str(), 1, bins, mins, maxs));
  return (G4int)histograms1D.size() - 1;
}

G4int BDSOutputROOTEventHistogramsSparse::Create1DHistogram(G4String name,
                                                            G4String title,
                                                            std::vector<double>& edges)
{
  std::vector<TAxis> axes;
  axes.reserve(1); // For a 1D histogram
  axes.emplace_back((Int_t)edges.size()-1, edges.data());
  histograms1D.push_back(new THnSparseD(name.c_str(), title.c_str(), axes));
  return (G4int)histograms1D.size() - 1;
}

G4int BDSOutputROOTEventHistogramsSparse::Create2DHistogram(G4String name, G4String title,
                                                            G4int nxbins, G4double xmin, G4double xmax,
                                                            G4int nybins, G4double ymin, G4double ymax)
{
  Int_t bins[2] = {nxbins, nybins};
  Double_t mins[2] = {xmin, ymin};
  Double_t maxs[2] = {xmax, ymax};
  histograms2D.push_back(new THnSparseD(name.c_str(), title.c_str(), 2, bins, mins, maxs));
  return (G4int)histograms2D.size() - 1;
}

G4int BDSOutputROOTEventHistogramsSparse::Create2DHistogram(G4String name, G4String title,
                                                            std::vector<double>& xedges,
                                                            std::vector<double>& yedges)
{
  std::vector<TAxis> axes;
  axes.reserve(2); // For a 2D histogram
  axes.emplace_back((Int_t)xedges.size()-1, xedges.data());
  axes.emplace_back((Int_t)yedges.size()-1, yedges.data());
  histograms2D.push_back(new THnSparseD(name.c_str(), title.c_str(), axes));
  return (G4int)histograms2D.size() - 1;
}

G4int BDSOutputROOTEventHistogramsSparse::Create3DHistogram(G4String name, G4String title,
                                                            G4int nxbins, G4double xmin, G4double xmax,
                                                            G4int nybins, G4double ymin, G4double ymax,
                                                            G4int nzbins, G4double zmin, G4double zmax)
{
  Int_t bins[3] = {nxbins, nybins, nzbins};
  Double_t mins[3] = {xmin, ymin, zmin};
  Double_t maxs[3] = {xmax, ymax, zmax};
  histograms3D.push_back(new THnSparseD(name.c_str(), title.c_str(), 3, bins, mins, maxs));
  return (G4int)histograms3D.size() - 1;
}

G4int BDSOutputROOTEventHistogramsSparse::Create3DHistogram(G4String name, G4String title,
                                                            std::vector<double>& xedges,
                                                            std::vector<double>& yedges,
                                                            std::vector<double>& zedges)
{
  std::vector<TAxis> axes;
  axes.reserve(3); // For a 3D histogram
  axes.emplace_back((Int_t)xedges.size()-1, xedges.data());
  axes.emplace_back((Int_t)yedges.size()-1, yedges.data());
  axes.emplace_back((Int_t)zedges.size()-1, zedges.data());
  histograms3D.push_back(new THnSparseD(name.c_str(), title.c_str(), axes));
  return (G4int)histograms3D.size() - 1;
}

void BDSOutputROOTEventHistogramsSparse::Fill1DHistogram(G4int histoId,
                                                         G4double value,
                                                         G4double weight)
{
  histograms1D[histoId]->Fill(value, weight);
}

void BDSOutputROOTEventHistogramsSparse::Fill2DHistogram(G4int    histoId,
                                                         G4double xValue,
                                                         G4double yValue,
                                                         G4double weight)
{
  histograms2D[histoId]->Fill(xValue,yValue,weight);
}

void BDSOutputROOTEventHistogramsSparse::Fill3DHistogram(G4int    histoId,
                                                         G4double xValue,
                                                         G4double yValue,
                                                         G4double zValue,
                                                         G4double weight)
{
  histograms3D[histoId]->Fill(xValue,yValue,zValue,weight);
}

void BDSOutputROOTEventHistogramsSparse::Set3DHistogramBinContent(G4int histoId,
                                                                  G4int globalBinID,
                                                                  G4double value)
{
  histograms3D[histoId]->SetBinContent(globalBinID, value);
}

void BDSOutputROOTEventHistogramsSparse::AccumulateHistogram3D(G4int histoId,
                                                               THnSparseD* otherHistogram)
{
  histograms3D[histoId]->Add(otherHistogram);
}

#endif

void BDSOutputROOTEventHistogramsSparse::Flush()
{
  for (auto h : histograms1D)
    {h->Reset();}
  for (auto h : histograms2D)
    {h->Reset();}
  for (auto h : histograms3D)
    {h->Reset();}
}
