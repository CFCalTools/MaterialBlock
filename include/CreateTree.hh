#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"



class CreateTree
{
private:
  
  TTree*  ftree;
  TString fname;
  
public:
  
  bool INIT_DATA; 
  bool POSITION;
  
public:
  
  CreateTree(TString name, bool init_data, bool position);
  ~CreateTree();
  
  TTree*             GetTree() const { return ftree; };
  TString            GetName() const { return fname; };
  int                Fill() { return this->GetTree()->Fill(); };
  bool               Write();
  void               Clear();
  static CreateTree* Instance() { return fInstance; };
  static CreateTree* fInstance;
  
  bool Init_data() const { return this -> INIT_DATA; };
  bool Position()  const { return this -> POSITION;  };
  
  int Event;
  
  float InitialEnergy;
  
  float InitialPositionX;
  float InitialPositionY;
  float InitialPositionZ;
  
  float InitalMomentumDirectionX;
  float InitalMomentumDirectionY;
  float InitalMomentumDirectionZ;
  
  int   PrimaryParticleN;
  float PrimaryParticleX[1000];
  float PrimaryParticleY[1000];
  float PrimaryParticleZ[1000];
  float PrimaryParticleE[1000];
  
  std::vector<float> Local_ion_energy_absorber;
  std::vector<float> depositionX;
  std::vector<float> depositionY;
  std::vector<float> depositionZ;
  
  float Total_delta_absorber;
  float Total_energy_absorber;
  float Total_ion_energy_absorber;
  float Total_nonion_energy_absorber;

  float Radial_stepLength;
  float Longitudinal_stepLength;
  float Radial_ion_energy_absorber[1000];
  float Longitudinal_ion_energy_absorber[1000];
  
  float Total_delta_world;
  float Total_energy_world;
  float Total_ion_energy_world;
  float Total_nonion_energy_world;
  
  float Total_em_energy;
};
