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

bool diffNew_ = false;
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
    if ( verbose_ ) {
      std::cout << "validatePatTuple INFO:" << std::endl;
      std::cout << "--> histogram '" << nameHistoTmp.Data() << "' could not be plotted." << std::endl;
    }
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


Double_t plotVar( const TString& varOrig, Int_t area = 0, Bool_t diffNew = false )
{
  // Create individual name
  TString name( varOrig );
  name.ReplaceAll( ".", "_" );
  name.ReplaceAll( "(", "" );
  name.ReplaceAll( ")", "" );
  name.ReplaceAll( "@", "AT" );
  name.ReplaceAll( "[", "_" );
  name.ReplaceAll( "]", "_" );

  // Canvas
  TString nameCanvas( "c_" + name );
  TCanvas* canvas( new TCanvas( nameCanvas, varOrig ) );
  canvas->SetGrid();
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> canvas '" << nameCanvas.Data() << "' created." << std::endl;
  }

  // Reference histogram
  TString nameHistoOrig( "orig_" + name );
  TH1F* origHisto = createHisto( varOrig, origEvents_, nameHistoOrig, area );
  Bool_t origHistoFilled( false );
  if ( !origHisto ) {
    std::cout << "validatePatTuple WARNING:" << std::endl;
    std::cout << "--> histogram '" << nameHistoOrig.Data() << "' has not been created properly." << std::endl;
  }
  else {
    origHisto->SetLineColor( kRed );
    origHisto->SetFillColor( kYellow );
    origEvents_->Draw( varOrig + ">>" + nameHistoOrig, "", "", nMax_ );
    origHistoFilled = ( origHisto->GetEntries() > 0 );
  }

  // Histograms
  TString nameHisto( name );
  TString var( varOrig );
  // Uncomment and fill, if e.g. variable name has changed
//   if ( diffNew ) {
//   }
  TH1F* histo;
  if ( origHistoFilled ) {
    histo = new TH1F( nameHisto, origHisto->GetTitle(), origHisto->GetNbinsX(), origHisto->GetXaxis()->GetXmin(), origHisto->GetXaxis()->GetXmax() );
  }
  else {
    histo = createHisto( varNew, events_, nameHisto, area );
  }
  Bool_t histoFilled( false );
  if ( !histo ) {
    std::cout << "validatePatTuple WARNING:" << std::endl;
    std::cout << "--> histogram '" << nameHisto.Data() << "' has not been created properly." << std::endl;
  }
  else {
    histo->SetLineColor( kBlue );
    events_->Draw( var + ">>" + nameHisto, "", "", nMax_ );
    histoFilled = ( histo->GetEntries() > 0 );
  }

  // Plot
  TString titleDiffHisto;
  Int_t binsDiffHisto;
  Float_t minDiffHisto;
  Float_t maxDiffHisto;
  if ( !origHistoFilled ) {
    if ( !histoFilled ) {
      if ( !histo ) {
        histo = new TH1F( nameHisto, var, 20, 0., 1. );
        histo->SetLineColor( kBlue );
      }
      histo->SetMaximum( 1. );
    }
    histo->SetMinimum( -TMath::Sqrt( histo->GetMaximum() ) );
    histo->Draw();
    if ( !origHisto ) {
      origHisto = new TH1F( nameHistoOrig, histo->GetTitle(), histo->GetNbinsX(), histo->GetXaxis()->GetXmin(), histo->GetXaxis()->GetXmax() );
      origHisto->SetLineColor( kRed );
      origHisto->SetFillColor( kYellow );
    }
    origHisto->Draw( "Same" );
    histo->Draw( "Same" );
    titleDiffHisto = histo->GetTitle();
    binsDiffHisto  = histo->GetNbinsX();
    minDiffHisto   = histo->GetXaxis()->GetXmin();
    maxDiffHisto   = histo->GetXaxis()->GetXmax();
  }
  else {
    origHisto->SetMinimum( -TMath::Sqrt( origHisto->GetMaximum() ) );
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
    std::cout << "validatePatTuple ERROR:" << std::endl;
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
        std::cout << "validatePatTuple " << mode.Data() << ":" << std::endl;
        if ( origHisto->GetBinContent( iBin ) == 0. ) std::cout << "--> variable '" << var.Data() << "' has " << 100. * ( diffHisto->GetBinContent( iBin ) / origHisto->GetBinContent( iBin ) ) << "% difference in bin " << iBin;
        else                                          std::cout << "--> variable '" << var.Data() << "' has 100% difference in bin " << iBin;
        if ( iBin == 0 )                               std::cout << " (underflow)";
        else if ( iBin == diffHisto->GetNbinsX() + 1 ) std::cout << " (overflow)";
        std::cout << std::endl;
      }
    }
  }
  if ( diff != 0. || origHisto->GetEntries() != histo->GetEntries() ) {
    std::cout << "validatePatTuple WARNING:" << std::endl;
    std::cout << "--> variable '" << var.Data() << "' has differences." << std::endl;
  }
  else if ( !showAll_ ) {
    delete canvas;
  }

  return diff;
}
