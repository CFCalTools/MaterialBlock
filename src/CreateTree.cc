#include "CreateTree.hh"



CreateTree* CreateTree::fInstance = NULL;



CreateTree::CreateTree(TString name, bool init_data, bool position)
{
  if( fInstance )
  {
    return;
  }
  
  this -> INIT_DATA    	= init_data;
  this -> POSITION      = position;
  
  this -> fInstance = this;
  this -> fname     = name;
  this -> ftree     = new TTree(name,name);
  
  this->GetTree()->Branch("Event",&this->Event,"Event/I");
  
  this->GetTree()->Branch("Total_delta_absorber",        &this->Total_delta_absorber,        "Total_delta_absorber/F");
  this->GetTree()->Branch("Total_energy_absorber",       &this->Total_energy_absorber,       "Total_energy_absorber/F");
  this->GetTree()->Branch("Total_ion_energy_absorber",   &this->Total_ion_energy_absorber,   "Total_ion_energy_absorber/F");
  this->GetTree()->Branch("Total_nonion_energy_absorber",&this->Total_nonion_energy_absorber,"Total_nonion_energy_absorber/F");
  
  this->GetTree()->Branch("Radial_stepLength",               &this->Radial_stepLength,               "Radial_stepLength/F");
  this->GetTree()->Branch("Radial_ion_energy_absorber",      &this->Radial_ion_energy_absorber,      "Radial_ion_energy_absorber[1000]/F");
  this->GetTree()->Branch("Longitudinal_stepLength",         &this->Longitudinal_stepLength,         "Longitudinal_stepLength/F");
  this->GetTree()->Branch("Longitudinal_ion_energy_absorber",&this->Longitudinal_ion_energy_absorber,"Longitudinal_ion_energy_absorber[1000]/F");
  
  this->GetTree()->Branch("Total_delta_world",        &this->Total_delta_world,        "Total_delta_world/F");
  this->GetTree()->Branch("Total_energy_world",       &this->Total_energy_world,       "Total_energy_world/F");
  this->GetTree()->Branch("Total_ion_energy_world",   &this->Total_ion_energy_world,   "Total_ion_energy_world/F");
  this->GetTree()->Branch("Total_nonion_energy_world",&this->Total_nonion_energy_world,"Total_nonion_energy_world/F");
  
  this->GetTree()->Branch("Total_em_energy",&this->Total_em_energy,"Total_em_energy/F");
  
  if( this -> INIT_DATA )
  {
    this->GetTree()->Branch("InitialEnergy",&this->InitialEnergy,"InitialEnergy/F");
    
    this->GetTree()->Branch("InitialPositionX",&this->InitialPositionX,"InitialPositionX/F");
    this->GetTree()->Branch("InitialPositionY",&this->InitialPositionY,"InitialPositionY/F");
    this->GetTree()->Branch("InitialPositionZ",&this->InitialPositionZ,"InitialPositionZ/F");
    
    this->GetTree()->Branch("InitalMomentumDirectionX",&this->InitalMomentumDirectionX,"InitalMomentumDirectionX/F");
    this->GetTree()->Branch("InitalMomentumDirectionY",&this->InitalMomentumDirectionY,"InitalMomentumDirectionY/F");
    this->GetTree()->Branch("InitalMomentumDirectionZ",&this->InitalMomentumDirectionZ,"InitalMomentumDirectionZ/F");
    
    this->GetTree()->Branch("PrimaryParticleN",&this->PrimaryParticleN,"PrimaryParticleN/I");
    this->GetTree()->Branch("PrimaryParticleX",&this->PrimaryParticleX,"PrimaryParticleX[1000]/F");
    this->GetTree()->Branch("PrimaryParticleY",&this->PrimaryParticleY,"PrimaryParticleY[1000]/F");
    this->GetTree()->Branch("PrimaryParticleZ",&this->PrimaryParticleZ,"PrimaryParticleZ[1000]/F");
    this->GetTree()->Branch("PrimaryParticleE",&this->PrimaryParticleE,"PrimaryParticleE[1000]/F");
  }
  
  if( this -> POSITION )
  { 
    this->GetTree()->Branch("Local_ion_energy_absorber",&this->Local_ion_energy_absorber);
    this->GetTree()->Branch("depositionX",&this->depositionX);
    this->GetTree()->Branch("depositionY",&this->depositionY);
    this->GetTree()->Branch("depositionZ",&this->depositionZ);  
  }
  
  this->Clear();
}



CreateTree::~CreateTree()
{}



bool CreateTree::Write()
{
  TString filename = this->GetName();
  filename+=".root";
  TFile* file = new TFile(filename,"RECREATE");
  this->GetTree()->Write();
  file->Write();
  file->Close();
  return true;
}



void CreateTree::Clear()
{
  Event	= 0;
  
  Total_delta_absorber         = 0;
  Total_energy_absorber        = 0;
  Total_ion_energy_absorber    = 0;
  Total_nonion_energy_absorber = 0;
  
  Radial_stepLength = 0.;
  Longitudinal_stepLength = 0.;
  for(int i = 0; i < 1000; ++i)
  {
    Radial_ion_energy_absorber[i] = 0.;
    Longitudinal_ion_energy_absorber[i] = 0.;
  }
  
  Total_delta_world         = 0;
  Total_energy_world        = 0;
  Total_ion_energy_world    = 0;
  Total_nonion_energy_world = 0;
  
  Total_em_energy = 0;
  
  if( this->INIT_DATA )
  {
    InitialEnergy = 0;
    
    InitialPositionX = 0;
    InitialPositionY = 0;
    InitialPositionZ = 0;
    
    InitalMomentumDirectionX = 0;
    InitalMomentumDirectionY = 0;
    InitalMomentumDirectionZ = 0;
    
    PrimaryParticleN = 0;
    for(int i = 0; i < 1000; ++i)
    {
      PrimaryParticleX[i] = 0.;
      PrimaryParticleY[i] = 0.;
      PrimaryParticleZ[i] = 0.;
      PrimaryParticleE[i] = 0.;
    }
  }
  
  if( this->POSITION )
  {
    Local_ion_energy_absorber.clear();
    depositionX.clear();		
    depositionY.clear();		
    depositionZ.clear();
  }
}
