//
// Validation script for PAT
//
// (inspirde by code from Slava and J.R.)
//
// e.g. > root -l '~/work/cms/CMSSW/L2/scripts/validatePatTuple.C+ ( "standard","patTuple_standard.orig.root","patTuple_standard.root" );'
//


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <cmath>

#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"

#endif


// Global variables

bool showAll_ = false;
bool verbose_ = false;
TTree* origEvents_ = 0;
TTree* newEvents_  = 0;
Int_t nMax_ = 0;


Double_t plotVar( const TString& var )
{
  // Create individual name
  TString name( var );
  name.ReplaceAll( ".", "_" );
  name.ReplaceAll( "(", "" );
  name.ReplaceAll( ")", "" );
  name.ReplaceAll( "@", "AT" );
  name.ReplaceAll( "[", "_" );
  name.ReplaceAll( "]", "_" );

  // Canvas
  TString nameCanvas( "c_" + name );
  TCanvas* canvas( new TCanvas( nameCanvas, var ) );
  canvas->SetGrid();
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> canvas '" << nameCanvas.Data() << "' created." << std::endl;
  }

  // Reference histograms
  TString nameHistoOrig( "orig_" + name );
  origEvents_->Draw( var + ">>" + nameHistoOrig, "", "", nMax_ );
  TH1F* origHisto( ( TH1F* )gROOT->Get( nameHistoOrig ) );
  if ( origHisto ) {
    origHisto->SetLineColor( kRed );
    origHisto->SetFillColor( kYellow );
  }
  else {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHistoOrig.Data() << "' could not be plotted." << std::endl;
    delete canvas;
    return -1;
  }

  // New histograms
  TString nameHistoNew( "new_" + name );
  TH1F* newHisto( new TH1F( nameHistoNew, origHisto->GetTitle(), origHisto->GetNbinsX(), origHisto->GetXaxis()->GetXmin(), origHisto->GetXaxis()->GetXmax() ) );
  newEvents_->Draw( var + ">>" + nameHistoNew, "", "", nMax_ );
  if ( newHisto ) {
    newHisto->SetLineColor( kBlue );
  }
  else {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHistoNew.Data() << "' could not be plotted." << std::endl;
    delete canvas;
    return -1;
  }

  // Plot
  origHisto->SetMinimum( -0.05 * origHisto->GetMaximum() );
  origHisto->Draw();
  newHisto->Draw( "Same" );
  TString nameDiffHist( "diff_" + name );
  TH1F* diffHisto( new TH1F(nameDiffHist, origHisto->GetTitle(), origHisto->GetNbinsX(), origHisto->GetXaxis()->GetXmin(), origHisto->GetXaxis()->GetXmax() ) );
  diffHisto->Add( origHisto );
  diffHisto->Add( newHisto, -1. );
  diffHisto->SetMarkerColor( kBlack );
  diffHisto->SetMarkerStyle( 7 );
  diffHisto->SetLineColor( kBlack );
  diffHisto->Draw( "P Same" );
  TLegend * leg = new TLegend( 0.5, 0.8, 0.99, 0.99 );
  leg->AddEntry( origHisto, "orig" );
  leg->AddEntry( newHisto, "new", "L" );
  leg->AddEntry( diffHisto, "diff (|orig-new|)", "P" );
  leg->Draw();

  // Check
  Double_t diff( 0. );
  for ( Int_t iBin = 0; iBin <= diffHisto->GetNbinsX(); ++iBin ) {
    diff += std::fabs( diffHisto->GetBinContent( iBin ) );
  }
  if ( diff != 0. ) {
    std::cout << "validatePatTuple WARNING:" << std::endl;
    std::cout << "--> variable '" << var.Data() << "' has differences." << std::endl;
  }
  else if ( !showAll_ ) {
    delete canvas;
  }

  return diff;
}


Double_t plotVarsStandard()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.ecalDrivenMomentum().pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.ecalDrivenMomentum().eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.ecalDrivenMomentum().phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.trackIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.ecalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.hcalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_[0].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_[1].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_[2].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_[3].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[0].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[1].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[2].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[3].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[4].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[5].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[6].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[7].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[8].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.jecFactor(0)" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.jecFactor(1)" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETs__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETs__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETs__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETs__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.trackIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.ecalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.hcalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.trackIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.ecalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.hcalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj.trackIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj.ecalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_selectedPatTaus__PAT.obj.hcalIso()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsData()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsPF2PAT()
{
  Double_t returnSum( 0. );
//   Double_t returnValue;
  return returnSum;
}


Double_t plotVarsPATandPF2PAT()
{
  Double_t returnSum( 0. );
//   Double_t returnValue;
  return returnSum;
}


Double_t plotVarsAddDecayInFlight()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarAddBTaggings()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[0].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[1].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[2].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[3].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[4].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[5].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[6].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[7].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[8].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[9].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[10].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[11].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[12].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[13].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[14].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[15].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[16].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[17].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[18].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[19].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[20].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[21].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[22].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[23].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[24].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[25].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[26].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[27].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[28].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[29].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[30].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[31].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[32].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[33].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[34].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[35].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[36].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[37].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[38].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[39].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[40].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[41].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[42].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[43].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[44].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[45].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[46].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[47].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[48].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[49].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[50].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[51].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[52].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[53].second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
//   returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[54].second" );
//   if (returnValue  < 0. ) return -returnSum;
//   returnSum += returnValue;
//   returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[55].second" );
//   if (returnValue  < 0. ) return -returnSum;
//   returnSum += returnValue;
//   returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[56].second" );
//   if (returnValue  < 0. ) return -returnSum;
//   returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.jecFactor(0)" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.jecFactor(1)" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsAddJets()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
 return returnSum;
}


Double_t plotVarsAddTracks()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return 0.;
}


Int_t plotVarsAddTriggerInfo()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsAddVertexInfo()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsFastsim()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsTopSelection()
{
  Double_t returnSum( 0. );
//   Double_t returnValue;
  return returnSum;
}



Double_t plotVarsUserData()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}



Double_t plotVarsMetUncertainties()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t validatePatTuple( const TString testID, const TString origFile, const TString newFile, bool showAll = false, bool verbose = false )
{
  // Parameters
  showAll_ = showAll;
  verbose_ = verbose;

  // Get trees
  origEvents_ = ( TTree* )( TFile::Open( origFile )->Get( "Events" ) );
  if ( !origEvents_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> original file '" << origFile.Data() << "' could not be opened." << std::endl;
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> original file '" << origFile.Data() << "' opened." << std::endl;
  }
  newEvents_  = ( TTree* )( TFile::Open( newFile )->Get( "Events" ) );
  if ( !newEvents_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> new file '" << newFile.Data() << "' could not be opened." << std::endl;
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> new file '" << newFile.Data() << "' opened." << std::endl;
  }

  // Normalisation
  Int_t nOrig( origEvents_->GetEntries() );
  Int_t nNew( newEvents_->GetEntries() );
  nMax_ = TMath::Min( nOrig, nNew );

  // RooT
  gStyle->SetOptStat( 0 );
  gROOT-> cd();

  // Act according to ID
  Double_t returnValue( 0. );
  if ( testID == "standard" ) {/**/
    returnValue = plotVarsStandard();
  }
//   else if ( testID == "data" ) {
//     returnValue =plotVarsData();
//   }
  else if ( testID == "PF2PAT" ) {
    returnValue =plotVarsPF2PAT();
  }
//   else if ( testID == "PATandPF2PAT" ) { // repeats "standard" and "PF2PAT"
//     returnValue =plotVarsPATandPF2PAT();
//   }
  else if ( testID == "addDecayInFlight" ) {
    returnValue =plotVarsAddDecayInFlight();
  }
  else if ( testID == "addBTagging" ) {
    returnValue =plotVarAddBTaggings();
  }
  else if ( testID == "addJets" ) {
    returnValue =plotVarsAddJets();
  }
  else if ( testID == "addTracks" ) {
    returnValue =plotVarsAddTracks();
  }
  else if ( testID == "addTriggerInfo" ) {
    returnValue =plotVarsAddTriggerInfo();
  }
  else if ( testID == "addVertexInfo" ) {
    returnValue =plotVarsAddVertexInfo();
  }
  else if ( testID == "fastsim" ) {
    returnValue =plotVarsFastsim();
  }
//   else if ( testID == "topSelection" ) {
//     returnValue =plotVarsTopSelection();
//   }
//   else if ( testID == "userData" ) {
//     returnValue =plotVarsUserData();
//   }
  else if ( testID == "metUncertainties" ) {
    returnValue =plotVarsMetUncertainties();
  }
  else {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> testID '" << testID << "' is not available." << std::endl;
    returnValue = -1.;
  }

  return returnValue;

}
