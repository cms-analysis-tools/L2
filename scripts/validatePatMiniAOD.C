//
// Validation script for PAT
//
// (inspirde by code from Slava and J.R.)
//
// e.g. > root -l '~/work/cms/CMSSW/L2/scripts/validatePatMiniAOD.C+ ( "standard", "pattMiniAOD_standard.orig.root", "pattMiniAOD_standard.root" );'
//


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <vector>
#include <iostream>
#include <cmath>

#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"

#endif


// Global variables

bool showAll_ = false;
bool verbose_ = false;
TFile* origFile_ = 0;
TFile* file_     = 0;
TTree* origEvents_ = 0;
TTree* events_     = 0;
Int_t nMax_ = 0;


void print()
{
  // Save plots
  const TSeqCollection* canvases = gROOT->GetListOfCanvases();
  for ( Int_t iCanvas = 0 ; iCanvas != canvases->GetSize(); ++iCanvas ){
    TString namePlot( canvases->At( iCanvas )->GetName() );
    namePlot.Append( ".png" );
    canvases->At( iCanvas )->Print( namePlot );
  }
}


TH1F* createHisto( const TString& var, TTree* events, const TString& nameHisto, Int_t area = 0 )
{
  TH1F* histo = 0;
  TString nameHistoTmp( nameHisto + "_tmp" );
  events->Draw( var + ">>" + nameHistoTmp, "", "", nMax_ );
  TH1F* histoTmp( ( TH1F* )gROOT->Get( nameHistoTmp ) );
  if ( !histoTmp ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHistoTmp.Data() << "' could not be plotted." << std::endl;
    return histo;
  }
  Float_t xMin( histoTmp->GetXaxis()->GetXmin() );
  Float_t xMax( histoTmp->GetXaxis()->GetXmax() );
  if ( area > 0 ) {
    xMin = TMath::Max( 0., histoTmp->GetXaxis()->GetXmin() );
    if ( xMin >= xMax ) xMax = xMin + 1.;
  }
  else if (area < 0  ) {
    xMax =TMath::Min( histoTmp->GetXaxis()->GetXmax(), 0. );
    if ( xMin >= xMax ) xMin = xMax - 1.;
  }
  else {
    if ( xMin >= xMax ) {
      xMin = xMax - 0.5;
      xMax = xMin + 1.;
    }
  }
  Int_t nBins( histoTmp->GetNbinsX() );
  if ( nBins == 0 ) ++nBins;
  histo = new TH1F( nameHisto, histoTmp->GetTitle(), nBins, xMin, xMax ); // Re-using nunmber of bins, regardless, if same range is used.
  delete histoTmp;
  return histo;
}


Double_t plotVar( const TString& var, Int_t area = 0 )
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
    std::cout << "validatePatMiniAOD INFO:" << std::endl;
    std::cout << "--> canvas '" << nameCanvas.Data() << "' created." << std::endl;
  }

  // Reference histogram
  TString nameHistoOrig( "orig_" + name );
  TH1F* origHisto = createHisto( var, origEvents_, nameHistoOrig, area );
  if ( !origHisto ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHistoOrig.Data() << "' has not been created." << std::endl;
    delete canvas;
    return -1.;
  }
  origHisto->SetLineColor( kRed );
  origHisto->SetFillColor( kYellow );
  origEvents_->Draw( var + ">>" + nameHistoOrig, "", "", nMax_ );
  Bool_t origHistoFilled( origHisto->GetEntries() > 0 );

  // Histogram
  TString nameHisto( name );
  TH1F* histo;
  if ( origHistoFilled ) {
    histo = new TH1F( nameHisto, origHisto->GetTitle(), origHisto->GetNbinsX(), origHisto->GetXaxis()->GetXmin(), origHisto->GetXaxis()->GetXmax() );
  }
  else {
    histo = createHisto( var, events_, nameHisto, area );
  }
  if ( !histo ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHisto.Data() << "' has not been created." << std::endl;
    delete canvas;
    return -1.;
  }
  histo->SetLineColor( kBlue );
  events_->Draw( var + ">>" + nameHisto, "", "", nMax_ );
  Bool_t histoFilled( histo->GetEntries() > 0 );

  // Plot
  TString titleDiffHisto;
  Int_t binsDiffHisto;
  Float_t minDiffHisto;
  Float_t maxDiffHisto;
  if ( !origHistoFilled ) {
    if ( !histoFilled ) histo->SetMaximum( 1. );
    histo->SetMinimum( -0.05 * histo->GetMaximum() );
    histo->Draw();
    origHisto->Draw( "Same" );
    histo->Draw( "Same" );
    titleDiffHisto = histo->GetTitle();
    binsDiffHisto  = histo->GetNbinsX();
    minDiffHisto   = histo->GetXaxis()->GetXmin();
    maxDiffHisto   = histo->GetXaxis()->GetXmax();
  }
  else {
    origHisto->SetMinimum( -0.05 * origHisto->GetMaximum() );
    origHisto->Draw();
    histo->Draw( "Same" );
    titleDiffHisto = origHisto->GetTitle();
    binsDiffHisto  = origHisto->GetNbinsX();
    minDiffHisto   = origHisto->GetXaxis()->GetXmin();
    maxDiffHisto   = origHisto->GetXaxis()->GetXmax();
  }
  TString nameDiffHist( "diff_" + name );
  TH1F* diffHisto( new TH1F( nameDiffHist, titleDiffHisto, binsDiffHisto, minDiffHisto, maxDiffHisto ) );
  if ( !diffHisto ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameDiffHist.Data() << "' could not be plotted." << std::endl;
    delete canvas;
    return -1.;
  }
  if ( origHistoFilled ) diffHisto->Add( origHisto );
  if ( histoFilled )  diffHisto->Add( histo, -1. );
  diffHisto->SetMarkerColor( kBlack );
  diffHisto->SetMarkerStyle( 7 );
  diffHisto->SetLineColor( kBlack );
  diffHisto->SetFillColor( kRed );
  diffHisto->SetFillStyle( 3004 );
  diffHisto->Draw( "HP Same" );
  TLegend * leg = new TLegend( 0.65, 0.8, 0.89, 0.89 );
  TString legOrigTxt( "orig (" );
  legOrigTxt += origHisto->GetEntries();
  legOrigTxt += " entries)";
  leg->AddEntry( origHisto, legOrigTxt, "F" );
  TString legTxt( "new (" );
  legTxt += histo->GetEntries();
  legTxt += " entries)";
  leg->AddEntry( histo, legTxt, "L" );
  leg->AddEntry( diffHisto, "diff", "FP" );
  leg->Draw();

  // Check
  Double_t diff( 0. );
  for ( Int_t iBin = 0; iBin <= diffHisto->GetNbinsX() + 1; ++iBin ) {
    diff += std::fabs( diffHisto->GetBinContent( iBin ) );
    if ( std::fabs( diffHisto->GetBinContent( iBin ) ) != 0. ) {
      if ( iBin == 0 || iBin == diffHisto->GetBinContent( iBin ) || verbose_ ) {
        TString mode( verbose_ ? "INFO" : "WARNING" );
        std::cout << "validatePatMiniAOD " << mode.Data() << ":" << std::endl;
        std::cout << "--> variable '" << var.Data() << "' has " << 100. * ( diffHisto->GetBinContent( iBin ) / origHisto->GetBinContent( iBin ) ) << "% difference in bin " << iBin;
        if ( iBin == 0 )                               std::cout << " (underflow)";
        else if ( iBin == diffHisto->GetNbinsX() + 1 ) std::cout << " (overflow)";
        std::cout << std::endl;
      }
    }
  }
  if ( diff != 0. || origHisto->GetEntries() != histo->GetEntries() ) {
    std::cout << "validatePatMiniAOD WARNING:" << std::endl;
    std::cout << "--> variable '" << var.Data() << "' has differences." << std::endl;
  }
  else if ( !showAll_ ) {
    delete canvas;
  }

  return diff;
}


Double_t plotVarsCommon()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.ecalDrivenMomentum().pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.ecalDrivenMomentum().eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.ecalDrivenMomentum().phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.trackIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.ecalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.hcalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.electronIDs_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_slimmedElectrons__PAT.obj.electronIDs_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  for ( Int_t iID = 0; iID < 15; ++iID ) {
    TString varExp( "patElectrons_slimmedElectrons__PAT.obj.electronIDs_[" );
    varExp += iID;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  for ( Int_t iDiscr = 0; iDiscr < 9; ++iDiscr ) {
    TString varExp( "patJets_slimmedJets__PAT.obj.pairDiscriVector_[" );
    varExp += iDiscr;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  returnValue = plotVar( "patJets_slimmedJets__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_slimmedJetsAK8__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_slimmedMETs__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_slimmedMETs__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_slimmedMETs__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_slimmedMETs__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj.trackIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj.ecalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_slimmedMuons__PAT.obj.hcalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_lostTracks__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_lostTracks__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_lostTracks__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_lostTracks__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_packedPFCandidates__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_packedPFCandidates__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_packedPFCandidates__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedCandidates_packedPFCandidates__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj.trackIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj.ecalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_slimmedPhotons__PAT.obj.hcalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_slimmedTaus__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_slimmedTaus__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_slimmedTaus__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTaus_slimmedTaus__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.triggerObjectTypes_" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.filterLabels_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.pathIndices_" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.pathLastFilterAccepted_" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patTriggerObjectStandAlones_selectedPatTrigger__PAT.obj.pathL3FilterAccepted_" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoCATopJetTagInfos_caTopTagInfos__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoCATopJetTagInfos_caTopTagInfos__PAT.obj.properties_.nSubJets" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoCATopJetTagInfos_caTopTagInfos__PAT.obj.properties_.minMass" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoCATopJetTagInfos_caTopTagInfos__PAT.obj.properties_.topMass" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoCATopJetTagInfos_caTopTagInfos__PAT.obj.properties_.wMass" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsMC()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVar( "patPackedGenParticles_packedGenParticles__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedGenParticles_packedGenParticles__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedGenParticles_packedGenParticles__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedGenParticles_packedGenParticles__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedGenParticles_packedGenParticles__PAT.obj.pdgId()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPackedGenParticles_packedGenParticles__PAT.obj.status()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_slimmedGenJets__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_slimmedGenJets__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_slimmedGenJets__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_slimmedGenJets__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenParticles_prunedGenParticles__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenParticles_prunedGenParticles__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenParticles_prunedGenParticles__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenParticles_prunedGenParticles__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenParticles_prunedGenParticles__PAT.obj.pdgId()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenParticles_prunedGenParticles__PAT.obj.status()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsStandard()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsCommon();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVarsMC();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsData()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsCommon();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t validatePatMiniAOD( const TString testID, const TString origFile = TString( "" ), const TString file = TString( "" ), bool showAll = false, bool verbose = false )
{
  // Parameters
  showAll_ = showAll;
  verbose_ = verbose;

  TString origFileName(origFile  );
  // Default file names
  if ( origFileName.Length() == 0 ) {
    origFileName = TString( "patMiniAOD_" + testID + ".orig.root" );
  }
  TString fileName( file );
  if ( fileName.Length() == 0 ) {
    fileName = TString( "patMiniAOD_" + testID + ".root" );
  }

  // Get files and trees
  origFile_ = TFile::Open( origFileName );
  if ( !origFile_ ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> original file '" << origFileName.Data() << "' could not be opened." << std::endl;
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatMiniAOD INFO:" << std::endl;
    std::cout << "--> original file '" << origFileName.Data() << "' opened." << std::endl;
  }
  origEvents_ = ( TTree* )( origFile_->Get( "Events" ) );
  if ( !origEvents_ ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> original tree 'Events' could not be read." << std::endl;
    origFile_->Close();
    return 1;
  }
  file_ = TFile::Open( fileName );
  if ( !file_ ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> new file '" << fileName.Data() << "' could not be opened." << std::endl;
    origFile_->Close();
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatMiniAOD INFO:" << std::endl;
    std::cout << "--> file '" << fileName.Data() << "' opened." << std::endl;
  }
  events_  = ( TTree* )( TFile::Open( fileName )->Get( "Events" ) );
  if ( !events_ ) {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> tree 'Events' could not be read." << std::endl;
    origFile_->Close();
    file_->Close();
    return 1;
  }

  // Normalisation
  Int_t nOrig( origEvents_->GetEntries() );
  Int_t n( events_->GetEntries() );
  nMax_ = TMath::Min( nOrig, n );

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
  else {
    std::cout << "validatePatMiniAOD ERROR:" << std::endl;
    std::cout << "--> testID '" << testID << "' is not available." << std::endl;
    returnValue = -1.;
  }

  origFile_->Close();
  file_->Close();

  print();

  return returnValue;

}