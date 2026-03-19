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

#include <iostream>
#include <ostream>
#include <TROOT.h>

#include "TAxis.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "THnSparse.h"
#include "BDSBH4DBase.hh"
#include "BDSBH4D.hh"

#ifndef USE_BOOST
#include "BDSBH4DTypeDefs.hh"
#include "BDSDebug.hh"
#include "BDSException.hh"
#endif

ClassImp(BDSOutputROOTEventHistogramsSparse)


BDSOutputROOTEventHistogramsSparse::BDSOutputROOTEventHistogramsSparse(const BDSOutputROOTEventHistogramsSparse& rhs):
  TObject(rhs)
{
  Fill(&rhs);
}

BDSOutputROOTEventHistogramsSparse::BDSOutputROOTEventHistogramsSparse(std::vector<THnSparseD*>& histograms1DIn,
                                                                       std::vector<THnSparseD*>& histograms2DIn,
                                                                       std::vector<THnSparseD*>& histograms3DIn,
                                                                       std::vector<BDSBH4DBase*>& histograms4DIn):
  histograms1D(histograms1DIn),
  histograms2D(histograms2DIn),
  histograms3D(histograms3DIn),
  histograms4D(histograms4DIn)
{;}

BDSOutputROOTEventHistogramsSparse::~BDSOutputROOTEventHistogramsSparse()
{
  // Apparently I need this as memory management is different for THnSparse
  for (auto h : histograms1D) {delete h;}
  for (auto h : histograms2D) {delete h;}
  for (auto h : histograms3D) {delete h;}
  for (auto h : histograms4D) {delete h;}
}

void BDSOutputROOTEventHistogramsSparse::FillSimple(const BDSOutputROOTEventHistogramsSparse* rhs)
{
  if (!rhs)
    {return;}

  histograms1D = rhs->histograms1D;
  histograms2D = rhs->histograms2D;
  histograms3D = rhs->histograms3D;
  histograms4D = rhs->histograms4D;
}

void BDSOutputROOTEventHistogramsSparse::Fill(const BDSOutputROOTEventHistogramsSparse* rhs)
{
  if (!rhs)
    {return;}

  // for each histogram, clone (ie copy) it into this object
  for (auto h : rhs->histograms1D)
    {histograms1D.push_back(static_cast<THnSparseD*>(h->Clone()));}
  for (auto h : rhs->histograms2D)
    {histograms2D.push_back(static_cast<THnSparseD*>(h->Clone()));}
  for (auto h : rhs->histograms3D)
    {histograms3D.push_back(static_cast<THnSparseD*>(h->Clone()));}
#ifdef USE_BOOST
  for (auto h : rhs->histograms4D)
    {histograms4D.push_back(static_cast<BDSBH4DBase*>(h->Clone("")));}
#endif
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

#ifdef USE_BOOST
G4int BDSOutputROOTEventHistogramsSparse::Create4DHistogram(const G4String& name,
                                                      const G4String& title,
                                                      const G4String& eScale,
                                                      const std::vector<double>& eBinsEdges,
                                                      unsigned int nxbins, G4double xmin, G4double xmax,
                                                      unsigned int nybins, G4double ymin, G4double ymax,
                                                      unsigned int nzbins, G4double zmin, G4double zmax,
                                                      unsigned int nebins, G4double emin, G4double emax)
{
  std::string nameC   = (std::string)name;
  std::string titleC  = (std::string)title;
  std::string eScaleC = (std::string)eScale;

  if(eScale == "linear")
    {
      histograms4D.push_back(new BDSBH4D<boost_histogram_linear>(nameC, titleC, eScaleC,
                                                                 nxbins, xmin, xmax,
                                                                 nybins, ymin, ymax,
                                                                 nzbins, zmin, zmax,
                                                                 nebins, emin, emax));
    }
  else if(eScale == "log")
    {
      histograms4D.push_back(new BDSBH4D<boost_histogram_log>(nameC, titleC, eScaleC,
                                                              nxbins, xmin, xmax,
                                                              nybins, ymin, ymax,
                                                              nzbins, zmin, zmax,
                                                              nebins, emin, emax));
    }
  else if(eScale == "user")
    {
      histograms4D.push_back(new BDSBH4D<boost_histogram_variable>(nameC, titleC, eScaleC, eBinsEdges,
                                                                   nxbins, xmin, xmax,
                                                                   nybins, ymin, ymax,
                                                                   nzbins, zmin, zmax));
    }

  return (G4int)histograms4D.size() - 1;
}
#else
G4int BDSOutputROOTEventHistogramsSparse::Create4DHistogram(const G4String&, const G4String&, const G4String&,
                                                      const std::vector<double>&,
                                                      unsigned int, G4double, G4double,
                                                      unsigned int, G4double, G4double,
                                                      unsigned int, G4double, G4double,
                                                      unsigned int, G4double, G4double)
{
  throw BDSException(__METHOD_NAME__, "BDSIM compiled without BOOST support -> no 4D histograms.");
}
#endif

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

#ifdef USE_BOOST
void BDSOutputROOTEventHistogramsSparse::Fill4DHistogram(G4int    histoId,
                                                   G4double xValue,
                                                   G4double yValue,
                                                   G4double zValue,
                                                   G4double eValue)
{
  histograms4D[histoId]->Fill_BDSBH4D(xValue, yValue, zValue, eValue);
}
#else
void BDSOutputROOTEventHistogramsSparse::Fill4DHistogram(G4int,
                                                   G4double,
                                                   G4double,
                                                   G4double,
                                                   G4double)
{
  throw BDSException(__METHOD_NAME__, "BDSIM compiled without BOOST support -> no 4D histograms.");
}
#endif

void BDSOutputROOTEventHistogramsSparse::Set3DHistogramBinContent(G4int histoId,
                                                                  G4int globalBinID,
                                                                  G4double value)
{
  histograms3D[histoId]->SetBinContent(globalBinID, value);
}

#ifdef USE_BOOST
void BDSOutputROOTEventHistogramsSparse::Set4DHistogramBinContent(G4int histoId,
                                                            G4int x,
                                                            G4int y,
                                                            G4int z,
                                                            G4int e,
                                                            G4double value)
{
  histograms4D[histoId]->Set_BDSBH4D(x, y, z, e, value);
}
#else
void BDSOutputROOTEventHistogramsSparse::Set4DHistogramBinContent(G4int, G4int, G4int, G4int, G4int, G4double)
{
  throw BDSException(__METHOD_NAME__, "BDSIM compiled without BOOST support -> no 4D histograms.");
}
#endif

void BDSOutputROOTEventHistogramsSparse::AccumulateHistogram3D(G4int histoId,
                                                               THnSparseD* otherHistogram)
{
  histograms3D[histoId]->Add(otherHistogram);
}

void BDSOutputROOTEventHistogramsSparse::AccumulateHistogram4D(G4int histoId,
                                                         BDSBH4DBase* otherHistogram)
{
  *histograms4D[histoId] += *otherHistogram;
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
#ifdef USE_BOOST
  for (auto h : histograms4D)
    {h->Reset_BDSBH4D();}
#endif
}

#ifdef JB_TEST
int main()
{
  // Set up objects
  TTree* TestOutputTree = new TTree("Tests", "Test Output");
  BDSOutputROOTEventHistogramsSparse* histos = new BDSOutputROOTEventHistogramsSparse();
  TFile* TestOutputFile = new TFile("test_output.root","RECREATE", "Sparse histo test output file");
  if (TestOutputFile->IsZombie())
    {throw BDSException(__METHOD_NAME__, "Unable to open output file: test_output.root");}
  TestOutputFile->cd();
  TestOutputTree->Branch("Tests.", "BDSOutputROOTEventHistogramsSparse",    histos,     32000, 2);

  // Test 1D fill with fixed bin size:
  histos->Create1DHistogram("1D_histogram","1D Histogram",10, 0, 100);

  // Test 1D fill with varying bin size:
  // std::vector<double> histo_edges{0,20,30,80,100,130};
  // histos->Create1DHistogram("1D_histogram","1D Histogram",histo_edges);
  for (int i=0; i < 10; i++)
  {
    histos->Fill1DHistogram(0, 10*i, (double)i/10);
  }

  // Test 2D fill with fixed bin size:
  histos->Create2DHistogram("2D_histogram", "2D Histogram", 10, 0, 100, 10, 0, 10);

  // Test 2D fill with varying bin size:
  // std::vector<double> x_edges{0,20,30,80,100};
  // std::vector<double> y_edges{0,2,2.5,4.3,6.9,10};
  // histos->Create2DHistogram("2D_histogram", "2D Histogram", x_edges, y_edges);
  // for (int i=0; i < 10; i++)
  // {
  //   for (int j=0; j < 15; j++)
  //   {
  //     histos->Fill2DHistogram(0, 10*i, j, (double)i*j/150);
  //   }
  // }

  // Test 3D fill with fixed bin size:
  histos->Create3DHistogram("3D_histogram", "3D Histogram", 10, 0, 100, 10, 0, 10, 10, 3, 5);

  // Test 3D fill with varying bin size:
  // std::vector<double> x_edges_3d{0,20,30,80,100};
  // std::vector<double> y_edges_3d{0,2,2.5,4.3,6.9,10};
  // std::vector<double> z_edges_3d{3,3.1,3.9,4.3,4.6,5};
  // histos->Create3DHistogram("3D_histogram", "3D Histogram", x_edges_3d, y_edges_3d, z_edges_3d);
  for (int i=0; i < 10; i++)
  {
    for (int j=0; j < 15; j++)
    {
      for (int k=9; k < 25; k++)
      {
        histos->Fill3DHistogram(0, 10*i, j, std::sqrt(k), (double)i*j*k/750);
      }
    }
  }

  // Test setting 3D bin content:
  // Double_t coords[3] = {35, 3, 4};
  // Long64_t globalBin = histos->Get3DHistogram(0)->GetBin(coords);
  // histos->Set3DHistogramBinContent(0, globalBin, 1000); // make it 1000 to be obvious
  // std::cout<<"Bin content set to: "<<histos->Get3DHistogram(0)->GetBinContent(globalBin)<<std::endl;

  // Test accumulating 3D histogram:
  // Int_t bins[3] = {10, 10, 10};
  // Double_t mins[3] = {0, 0, 3};
  // Double_t maxs[3] = {100, 10, 5};
  // THnSparseD* histo_to_add = new THnSparseD("histo_to_add","Histogram to Add", 3, bins, mins, maxs);
  // for (int i=0; i < 10; i++)
  // {
  //   for (int j=0; j < 15; j++)
  //   {
  //     for (int k=9; k < 25; k++)
  //     {
  //       histo_to_add->Fill(i*i, j, std::sqrt(k), (double)i*j*k/750);
  //     }
  //   }
  // }
  // histos->AccumulateHistogram3D(0, histo_to_add);
  // delete histo_to_add;

  // Test instatiate from full object
  // TTree* TestOutputTree2 = new TTree("Tests2", "Test Output 2");
  // BDSOutputROOTEventHistogramsSparse* histos2 = new BDSOutputROOTEventHistogramsSparse(*histos);
  // TestOutputTree2->Branch("Tests2.", "BDSOutputROOTEventHistogramsSparse",    histos2,     32000, 2);
  // TestOutputTree2->Fill();
  // TestOutputTree2->Write();

  // Test fill from full object
  // TTree* TestOutputTree2 = new TTree("Tests2", "Test Output 2");
  // BDSOutputROOTEventHistogramsSparse* histos2 = new BDSOutputROOTEventHistogramsSparse();
  // TestOutputTree2->Branch("Tests2.", "BDSOutputROOTEventHistogramsSparse",    histos2,     32000, 2);
  // histos2->Fill(histos);
  // TestOutputTree2->Fill();
  // TestOutputTree2->Write();

  // Test flush (histograms should be blank)
  // histos->Flush();

  TestOutputTree->Fill();
  TestOutputTree->Write();
  TestOutputFile->Close();

  // Clean up
  delete histos;
  delete TestOutputTree;
  delete TestOutputFile;

  // Access and view histograms
  TFile* TestAccessFile = new TFile("test_output.root","READ");
  if (TestOutputFile->IsZombie())
    {throw BDSException(__METHOD_NAME__, "Unable to open output file: test_output.root");}
  TTree* TestAccessTree = (TTree*)TestAccessFile->Get("Tests");

  BDSOutputROOTEventHistogramsSparse* access_histos = nullptr;
  TestAccessTree->SetBranchAddress("Tests.", &access_histos);
  TestAccessTree->GetEntry(0);

  THnSparse* h1D = access_histos->Get1DHistogram(0);
  TH1D* h1D_proj = h1D->Projection(0);

  THnSparse* h2D = access_histos->Get2DHistogram(0);
  TH2D* h2D_proj = h2D->Projection(1, 0);

  THnSparse* h3D = access_histos->Get3DHistogram(0);
  TH3D* h3D_proj = h3D->Projection(0, 1, 2);

  TCanvas* c1D = new TCanvas("c1D", "1D Histogram");
  h1D_proj->Draw();
  c1D->SaveAs("TestOutput1D.png");

  TCanvas* c2D = new TCanvas("c2D", "2D Histogram");
  h2D_proj->Draw();
  c2D->SaveAs("TestOutput2D.png");

  TCanvas* c3D = new TCanvas("c3D", "3D Histogram");
  h3D_proj->Draw();
  c3D->SaveAs("TestOutput3D.png");

  TestAccessFile->Close();

  // Clean up
  delete access_histos;
  delete TestAccessFile;
  delete c1D;
  delete c2D;
  delete c3D;
}
#endif
