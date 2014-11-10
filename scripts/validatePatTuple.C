//
// Validation script for PAT
//
// (inspirde by code from Slava and J.R.)
//
// e.g. > root -l '~/work/cms/CMSSW/L2/scripts/validatePatTuple.C+ ( "standard", "patTuple_standard.orig.root", "patTuple_standard.root" );'
//


#if !defined(__CINT__) || defined(__MAKECINT__)

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
TFile* newFile_  = 0;
TTree* origEvents_ = 0;
TTree* newEvents_  = 0;
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
    std::cout << "validatePatTuple ERROR:" << std::endl;
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
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> canvas '" << nameCanvas.Data() << "' created." << std::endl;
  }

  // Reference histograms
  TString nameHistoOrig( "orig_" + name );
  TH1F* origHisto = createHisto( var, origEvents_, nameHistoOrig, area );
  if ( !origHisto ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHistoOrig.Data() << "' has not been created." << std::endl;
    delete canvas;
    return -1.;
  }
  origHisto->SetLineColor( kRed );
  origHisto->SetFillColor( kYellow );
  origEvents_->Draw( var + ">>" + nameHistoOrig, "", "", nMax_ );
  Bool_t origHistoFilled( origHisto->GetEntries() > 0 );

  // New histograms
  TString nameHistoNew( "new_" + name );
  TH1F* newHisto;
  if ( origHistoFilled ) {
    newHisto = new TH1F( nameHistoNew, origHisto->GetTitle(), origHisto->GetNbinsX(), origHisto->GetXaxis()->GetXmin(), origHisto->GetXaxis()->GetXmax() );
  }
  else {
    newHisto = createHisto( var, newEvents_, nameHistoNew, area );
  }
  if ( !newHisto ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameHistoNew.Data() << "' has not been created." << std::endl;
    delete canvas;
    return -1.;
  }
  newHisto->SetLineColor( kBlue );
  newEvents_->Draw( var + ">>" + nameHistoNew, "", "", nMax_ );
  Bool_t newHistoFilled( newHisto->GetEntries() > 0 );

  // Plot
  TString titleDiffHisto;
  Int_t binsDiffHisto;
  Float_t minDiffHisto;
  Float_t maxDiffHisto;
  if ( !origHistoFilled ) {
    if ( !newHistoFilled ) newHisto->SetMaximum( 1. );
    newHisto->SetMinimum( -0.05 * newHisto->GetMaximum() );
    newHisto->Draw();
    origHisto->Draw( "Same" );
    newHisto->Draw( "Same" );
    titleDiffHisto = newHisto->GetTitle();
    binsDiffHisto  = newHisto->GetNbinsX();
    minDiffHisto   = newHisto->GetXaxis()->GetXmin();
    maxDiffHisto   = newHisto->GetXaxis()->GetXmax();
  }
  else {
    origHisto->SetMinimum( -0.05 * origHisto->GetMaximum() );
    origHisto->Draw();
    newHisto->Draw( "Same" );
    titleDiffHisto = origHisto->GetTitle();
    binsDiffHisto  = origHisto->GetNbinsX();
    minDiffHisto   = origHisto->GetXaxis()->GetXmin();
    maxDiffHisto   = origHisto->GetXaxis()->GetXmax();
  }
  TString nameDiffHist( "diff_" + name );
  TH1F* diffHisto( new TH1F( nameDiffHist, titleDiffHisto, binsDiffHisto, minDiffHisto, maxDiffHisto ) );
  if ( !diffHisto ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> histogram '" << nameDiffHist.Data() << "' could not be plotted." << std::endl;
    delete canvas;
    return -1.;
  }
  if ( origHistoFilled ) diffHisto->Add( origHisto );
  diffHisto->Add( newHisto, -1. );
  diffHisto->SetMarkerColor( kBlack );
  diffHisto->SetMarkerStyle( 7 );
  diffHisto->SetLineColor( kBlack );
  diffHisto->Draw( "P Same" );
  TLegend * leg = new TLegend( 0.65, 0.8, 0.89, 0.89 );
  leg->AddEntry( origHisto, "orig" );
  leg->AddEntry( newHisto, "new", "L" );
  leg->AddEntry( diffHisto, "diff (orig-new)", "P" );
  leg->Draw();

  // Check
  Double_t diff( 0. );
  for ( Int_t iBin = 0; iBin <= diffHisto->GetNbinsX() + 1; ++iBin ) {
    diff += std::fabs( diffHisto->GetBinContent( iBin ) );
    if ( std::fabs( diffHisto->GetBinContent( iBin ) ) != 0. ) {
      if ( iBin == 0 || iBin == diffHisto->GetBinContent( iBin ) || verbose_ ) {
        TString mode( verbose_ ? "INFO" : "WARNING" );
        std::cout << "validatePatTuple " << mode.Data() << ":" << std::endl;
        std::cout << "--> variable '" << var.Data() << "' has " << 100. * ( diffHisto->GetBinContent( iBin ) / origHisto->GetBinContent( iBin ) ) << "% difference in bin " << iBin;
        if ( iBin == 0 )                               std::cout << " (underflow)";
        else if ( iBin == diffHisto->GetNbinsX() + 1 ) std::cout << " (overflow)";
        std::cout << std::endl;
      }
    }
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
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.trackIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.ecalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.hcalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  for ( Int_t iID = 0; iID < 5; ++iID ) {
    TString varExp( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_[" );
    varExp += iID;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
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
  for ( Int_t iDiscr = 0; iDiscr < 9; ++iDiscr ) {
    TString varExp( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_[" );
    varExp += iDiscr;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.jecFactor(1)", 1 );
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
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.trackIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.ecalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMuons_selectedPatMuons__PAT.obj.hcalIso()", 1 );
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
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.trackIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.ecalIso()", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patPhotons_selectedPatPhotons__PAT.obj.hcalIso()", 1 );
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
  for ( Int_t iDiscr = 0; iDiscr < 57; ++iDiscr ) {
    TString varExp( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_[" );
    varExp += iDiscr;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.jecFactor(1)", 1 );
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

  // Get files and trees
  origFile_ = TFile::Open( origFile );
  if ( !origFile_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> original file '" << origFile.Data() << "' could not be opened." << std::endl;
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> original file '" << origFile.Data() << "' opened." << std::endl;
  }
  origEvents_ = ( TTree* )( origFile_->Get( "Events" ) );
  if ( !origEvents_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> original tree 'Events' could not be read." << std::endl;
    origFile_->Close();
    return 1;
  }
  newFile_ = TFile::Open( newFile );
  if ( !newFile_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> new file '" << origFile.Data() << "' could not be opened." << std::endl;
    origFile_->Close();
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> new file '" << newFile.Data() << "' opened." << std::endl;
  }
  newEvents_  = ( TTree* )( TFile::Open( newFile )->Get( "Events" ) );
  if ( !newEvents_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> new tree 'Events' could not be read." << std::endl;
    origFile_->Close();
    newFile_->Close();
    return 1;
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

  origFile_->Close();
  newFile_->Close();

  print();

  return returnValue;

}
