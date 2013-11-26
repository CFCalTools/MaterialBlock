#include "SteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "EventAction.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UnitsTable.hh"
#include "CreateTree.hh"
#include "MyMaterials.hh"

#include <iostream>
#include <fstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"



SteppingAction::SteppingAction()
{}



SteppingAction::~SteppingAction()
{}



void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  
  
  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePrePV  = thePrePoint ->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePrePVName  = ""; if( thePrePV )  thePrePVName  = thePrePV  -> GetName();
  G4String thePostPVName = ""; if( thePostPV ) thePostPVName = thePostPV -> GetName();
  
  
  // primary particle
  if( theTrack->GetTrackID() == 1 )
  {
    G4int nStep = theTrack -> GetCurrentStepNumber() - 1;
    if( nStep < 1000 )
    {
      G4ThreeVector pos = thePostPoint -> GetPosition();
      
      CreateTree::Instance()->PrimaryParticleN = nStep;
      CreateTree::Instance()->PrimaryParticleX[nStep] = pos[0]/mm;
      CreateTree::Instance()->PrimaryParticleY[nStep] = pos[1]/mm;
      CreateTree::Instance()->PrimaryParticleZ[nStep] = pos[2]/mm;
      CreateTree::Instance()->PrimaryParticleE[nStep] = thePostPoint->GetTotalEnergy()/GeV;
    }
  }
  
  
  // optical photon
  if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
  {
    theTrack->SetTrackStatus(fKillTrackAndSecondaries);
  }
  
  
  // non optical photon
  else
  {
    G4TouchableHandle thePreTouch = thePrePoint -> GetTouchableHandle();
    G4VPhysicalVolume* thePreVolume = thePreTouch -> GetVolume();
    G4LogicalVolume* thePrePVLog = thePreVolume -> GetLogicalVolume();
    G4String thePrePVLogName = thePrePVLog -> GetName();
    
    
    float delta = thePrePoint->GetTotalEnergy()/GeV - thePostPoint->GetTotalEnergy()/GeV;
    float energy = theStep->GetTotalEnergyDeposit()/GeV;
    float ion_energy = theStep->GetTotalEnergyDeposit()/GeV - theStep->GetNonIonizingEnergyDeposit()/GeV;
    float nonion_energy = theStep->GetNonIonizingEnergyDeposit()/GeV;
    
    if( delta > 0 )
    {	
      CreateTree::Instance()->Total_delta_world         += delta;
      CreateTree::Instance()->Total_energy_world        += energy;
      CreateTree::Instance()->Total_ion_energy_world    += ion_energy;
      CreateTree::Instance()->Total_nonion_energy_world += nonion_energy;
      
      if( thePrePVName == "Absorber" || thePrePVLogName == "Absorber_log" )
      {
        CreateTree::Instance()->Total_delta_absorber         += delta;
        CreateTree::Instance()->Total_energy_absorber        += energy;
        CreateTree::Instance()->Total_ion_energy_absorber    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_absorber += nonion_energy;
        
        G4ThreeVector pos = thePostPoint -> GetPosition();
        G4ThreeVector initPos(CreateTree::Instance()->InitialPositionX,CreateTree::Instance()->InitialPositionY,CreateTree::Instance()->InitialPositionZ);
        
        for(int i = 0; i < 1000; ++i)
        {
          G4double radius = 0. + CreateTree::Instance()->Radial_stepLength * i;
          if( sqrt( pow(pos[0]/mm-initPos[0],2) + pow(pos[0]/mm-initPos[0],2) ) < radius )
            CreateTree::Instance()->Radial_ion_energy_absorber[i] += ion_energy;
        }
        
        for(int i = 0; i < 1000; ++i)
        {
          G4double depth = 0. + CreateTree::Instance()->Longitudinal_stepLength * i;
          if( (pos[2]/mm - initPos[2]) < depth )
            CreateTree::Instance()->Longitudinal_ion_energy_absorber[i] += ion_energy;
        }
        
        if( CreateTree::Instance()->Position() )
        {
          G4ThreeVector pos = thePostPoint -> GetPosition();
          CreateTree::Instance() -> Local_ion_energy_absorber.push_back(ion_energy);
          CreateTree::Instance() -> depositionX.push_back(pos[0]);		
          CreateTree::Instance() -> depositionY.push_back(pos[1]);
          CreateTree::Instance() -> depositionZ.push_back(pos[2]);
        }
      }  
    }
  } // non optical photon
}
