//
// Validation script for PAT
//
// (inspirde by code from Slava and J.R.)
//
// e.g. > cp ~/work/cms/CMSSW/L2/scripts/validatePat*
// e.g. > root -l 'validatePatTuple.C+( "standard" );'
//


#include "validatePat.h"


Double_t plotVarsCommon()
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
  returnValue = plotVar( "patElectrons_selectedPatElectrons__PAT.obj.electronIDs_.second" );
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
  returnValue = plotVar( "patJets_selectedPatJets__PAT.obj.pairDiscriVector_.second" );
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
  returnValue = plotVar( "recoPFCandidates_selectedPatJets_pfCandidates_PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoPFCandidates_selectedPatJets_pfCandidates_PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoPFCandidates_selectedPatJets_pfCandidates_PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoPFCandidates_selectedPatJets_pfCandidates_PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  return returnSum;
}


Double_t plotVarsMC()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVar( "recoGenJets_selectedPatJets_genJets_PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_selectedPatJets_genJets_PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_selectedPatJets_genJets_PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "recoGenJets_selectedPatJets_genJets_PAT.obj.phi()" );
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


Double_t plotVarsPF2PAT()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
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
  returnValue = plotVar( "patJets_selectedPatJetsAK4PF__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  for ( Int_t iDiscr = 0; iDiscr < 64; ++iDiscr ) {
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
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  for ( Int_t iDiscr = 0; iDiscr < 64; ++iDiscr ) {
    TString varExp( "patJets_selectedPatJetsCA8PFCHSPrunedSubjets__PAT.obj.pairDiscriVector_[" );
    varExp += iDiscr;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  return returnSum;
}


Double_t plotVarsAddJets()
{
  Double_t returnSum( 0. );
  Double_t returnValue;
  returnValue = plotVarsStandard();
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETPF__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETPF__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETPF__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETPF__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4PFCHSCopy__PAT.obj.jecFactor(1)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4PFCHSCopy__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4PFCHSCopy__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4PFCHSCopy__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4PFCHSCopy__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.pairDiscriVector_[0].second", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsCA8PFCHSPruned__PAT.obj.jecFactor(1)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.phi()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.partonFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.hadronFlavour()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.pairDiscriVector_@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.pairDiscriVector_.second" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  for ( Int_t iDiscr = 0; iDiscr < 7; ++iDiscr ) {
    TString varExp( "patJets_selectedPatJetsAK4Calo__PAT.obj.pairDiscriVector_[" );
    varExp += iDiscr;
    varExp += "].second";
    returnValue = plotVar( varExp, 1 );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.jecFactor(0)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patJets_selectedPatJetsAK4Calo__PAT.obj.jecFactor(1)", 1 );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4Calo__PAT.obj@.size()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4Calo__PAT.obj.pt()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4Calo__PAT.obj.eta()" );
  if (returnValue  < 0. ) return -returnSum;
  returnSum += returnValue;
  returnValue = plotVar( "patMETs_patMETsAK4Calo__PAT.obj.phi()" );
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
  std::vector< TString > collTags;
  collTags.push_back( TString( "patElectrons_shiftedPatElectronsEnDown__PAT" ) );
  collTags.push_back( TString( "patElectrons_shiftedPatElectronsEnUp__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype1p2Corr__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype1p2CorrEnDown__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype1p2CorrEnUp__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype1p2CorrOriginalReserved__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype1p2CorrResDown__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype1p2CorrResUp__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype2Corr__PAT" ) );
  collTags.push_back( TString( "patJets_selectedPatJetsForMETtype2CorrOriginalReserved__PAT" ) );
  collTags.push_back( TString( "patJets_shiftedPatJetsEnDown__PAT" ) );
  collTags.push_back( TString( "patJets_shiftedPatJetsEnDownForRawMEt__PAT" ) );
  collTags.push_back( TString( "patJets_shiftedPatJetsEnUp__PAT" ) );
  collTags.push_back( TString( "patJets_shiftedPatJetsEnUpForRawMEt__PAT" ) );
  collTags.push_back( TString( "patJets_smearedPatJets__PAT" ) );
  collTags.push_back( TString( "patJets_smearedPatJetsResDown__PAT" ) );
  collTags.push_back( TString( "patJets_smearedPatJetsResUp__PAT" ) );
  collTags.push_back( TString( "patMuons_shiftedPatMuonsEnDown__PAT" ) );
  collTags.push_back( TString( "patMuons_shiftedPatMuonsEnUp__PAT" ) );
  collTags.push_back( TString( "patTaus_shiftedPatTausEnDown__PAT" ) );
  collTags.push_back( TString( "patTaus_shiftedPatTausEnUp__PAT" ) );
  collTags.push_back( TString( "patTaus_shiftedPatTausEnUp__PAT" ) );
  collTags.push_back( TString( "recoGenJets_selectedPatJetsForMETtype1p2Corr_genJets_PAT" ) );
  collTags.push_back( TString( "recoGenJets_selectedPatJetsForMETtype1p2CorrOriginalReserved_genJets_PAT" ) );
  collTags.push_back( TString( "recoGenJets_selectedPatJetsForMETtype2Corr_genJets_PAT" ) );
  collTags.push_back( TString( "recoGenJets_selectedPatJetsForMETtype2CorrOriginalReserved_genJets_PAT" ) );
  collTags.push_back( TString( "recoPFCandidates_selectedPatJetsForMETtype1p2Corr_pfCandidates_PAT" ) );
  collTags.push_back( TString( "recoPFCandidates_selectedPatJetsForMETtype1p2CorrOriginalReserved_pfCandidates_PAT" ) );
  collTags.push_back( TString( "recoPFCandidates_selectedPatJetsForMETtype2Corr_pfCandidates_PAT" ) );
  collTags.push_back( TString( "recoPFCandidates_selectedPatJetsForMETtype2CorrOriginalReserved_pfCandidates_PAT" ) );
  for ( size_t iTag = 0; iTag < collTags.size(); ++iTag ) {
    returnValue = plotVar( TString( collTags[ iTag ] + ".obj@.size()" ) );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
    returnValue = plotVar( TString( collTags[ iTag ] + ".obj.pt()" ) );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
    returnValue = plotVar( TString( collTags[ iTag ] + ".obj.eta()" ) );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
    returnValue = plotVar( TString( collTags[ iTag ] + ".obj.phi()" ) );
    if (returnValue  < 0. ) return -returnSum;
    returnSum += returnValue;
  }
  return returnSum;
}


Double_t validatePatTuple( const TString testID, const TString origFile = TString( "" ), const TString file = TString( "" ), bool diffNew = false, bool showAll = false, bool verbose = false )
{
  // Parameters
  testID_  = testID;
  diffNew_ = diffNew;
  showAll_ = showAll;
  verbose_ = verbose;

  TString origFileName(origFile  );
  // Default file names
  if ( origFileName.Length() == 0 ) {
    origFileName = TString( "patTuple_" + testID_ + ".orig.root" );
  }
  TString fileName( file );
  if ( fileName.Length() == 0 ) {
    fileName = TString( "patTuple_" + testID_ + ".root" );
  }

  // Get files and trees
  origFile_ = TFile::Open( origFileName );
  if ( !origFile_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> original file '" << origFileName.Data() << "' could not be opened." << std::endl;
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> original file '" << origFileName.Data() << "' opened." << std::endl;
  }
  origEvents_ = ( TTree* )( origFile_->Get( "Events" ) );
  if ( !origEvents_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> original tree 'Events' could not be read." << std::endl;
    origFile_->Close();
    return 1;
  }
  file_ = TFile::Open( fileName );
  if ( !file_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> new file '" << fileName.Data() << "' could not be opened." << std::endl;
    origFile_->Close();
    return 1;
  }
  if ( verbose_ ) {
    std::cout << "validatePatTuple INFO:" << std::endl;
    std::cout << "--> file '" << fileName.Data() << "' opened." << std::endl;
  }
  events_  = ( TTree* )( TFile::Open( fileName )->Get( "Events" ) );
  if ( !events_ ) {
    std::cout << "validatePatTuple ERROR:" << std::endl;
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
  if ( testID_ == "standard" ) {
    returnValue = plotVarsStandard();
  }
//   else if ( testID_ == "data" ) {
//     returnValue =plotVarsData();
//   }
  else if ( testID_ == "PF2PAT" ) {
    returnValue =plotVarsPF2PAT();
  }
//   else if ( testID_ == "PATandPF2PAT" ) { // repeats "standard" and "PF2PAT"
//     returnValue =plotVarsPATandPF2PAT();
//   }
  else if ( testID_ == "addDecayInFlight" ) {
    returnValue = plotVarsAddDecayInFlight();
  }
  else if ( testID_ == "addBTagging" ) {
    returnValue = plotVarAddBTaggings();
  }
  else if ( testID_ == "addJets" ) {
    returnValue = plotVarsAddJets();
  }
  else if ( testID_ == "addTracks" ) {
    returnValue = plotVarsAddTracks();
  }
  else if ( testID_ == "addTriggerInfo" ) {
    returnValue = plotVarsAddTriggerInfo();
  }
  else if ( testID_ == "addVertexInfo" ) {
    returnValue = plotVarsAddVertexInfo();
  }
  else if ( testID_ == "fastsim" ) {
    returnValue = plotVarsFastsim();
  }
//   else if ( testID_ == "topSelection" ) {
//     returnValue = plotVarsTopSelection();
//   }
//   else if ( testID_ == "userData" ) {
//     returnValue = plotVarsUserData();
//   }
  else if ( testID_ == "metUncertainties" ) {
    returnValue = plotVarsMetUncertainties();
  }
  else {
    std::cout << "validatePatTuple ERROR:" << std::endl;
    std::cout << "--> testID '" << testID_ << "' is not available." << std::endl;
    returnValue = -1.;
  }

  origFile_->Close();
  file_->Close();

  print();

  return returnValue;

}
