void plots()
{
  TFile* f = TFile::Open("out.root");
  TTree* t = (TTree*)( f->Get("tree") );
  
  float InitialPositionX;
  float InitialPositionY;
  float InitialPositionZ;
  float InitialEnergy;
  int PrimaryParticleN;
  float PrimaryParticleX[1000];
  float PrimaryParticleY[1000];
  float PrimaryParticleZ[1000];
  float PrimaryParticleE[1000];
  float Total_ion_energy_absorber;
  float Radial_stepLength;
  float Longitudinal_stepLength;
  float Radial_ion_energy_absorber[1000];
  float Longitudinal_ion_energy_absorber[1000];
  
  t -> SetBranchStatus("*",0);
  
  t -> SetBranchStatus("InitialPositionX",1);  t -> SetBranchAddress("InitialPositionX",&InitialPositionX);
  t -> SetBranchStatus("InitialPositionY",1);  t -> SetBranchAddress("InitialPositionY",&InitialPositionY);
  t -> SetBranchStatus("InitialPositionZ",1);  t -> SetBranchAddress("InitialPositionZ",&InitialPositionZ);
  t -> SetBranchStatus("InitialEnergy",   1);  t -> SetBranchAddress("InitialEnergy",   &InitialEnergy);
  
  t -> SetBranchStatus("PrimaryParticleN",1);  t -> SetBranchAddress("PrimaryParticleN",&PrimaryParticleN);
  t -> SetBranchStatus("PrimaryParticleX",1);  t -> SetBranchAddress("PrimaryParticleX", PrimaryParticleX);
  t -> SetBranchStatus("PrimaryParticleY",1);  t -> SetBranchAddress("PrimaryParticleY", PrimaryParticleY);
  t -> SetBranchStatus("PrimaryParticleZ",1);  t -> SetBranchAddress("PrimaryParticleZ", PrimaryParticleZ);
  t -> SetBranchStatus("PrimaryParticleE",1);  t -> SetBranchAddress("PrimaryParticleE", PrimaryParticleE);
  
  t -> SetBranchStatus("Total_ion_energy_absorber",1);  t -> SetBranchAddress("Total_ion_energy_absorber", &Total_ion_energy_absorber);
  t -> SetBranchStatus("Radial_ion_energy_absorber",1); t -> SetBranchAddress("Radial_ion_energy_absorber", Radial_ion_energy_absorber);
  t -> SetBranchStatus("Longitudinal_ion_energy_absorber",1); t -> SetBranchAddress("Longitudinal_ion_energy_absorber", Longitudinal_ion_energy_absorber);
  t -> SetBranchStatus("Radial_stepLength",      1); t -> SetBranchAddress("Radial_stepLength",      &Radial_stepLength);
  t -> SetBranchStatus("Longitudinal_stepLength",1); t -> SetBranchAddress("Longitudinal_stepLength",&Longitudinal_stepLength);
  
  
  TH1F* h = new TH1F("h","",200,0.,100.);
  TH1F* h2 = new TH1F("h2","",500,0.,1000.);
  TProfile* p = new TProfile("p","",1000,0.,5000.);
  
  for(int entry = 0; entry < t->GetEntries(); ++entry)
  {
    t -> GetEntry(entry);
    
    float r_M = 0.;
    for(int i = 0; i < 1000; ++i)
    {
      if( Radial_ion_energy_absorber[i]/Total_ion_energy_absorber < 0.90 )
      {
        r_M = i * Radial_stepLength;
      }
    }
    
    h -> Fill(r_M);
    
    for(int i = 0; i < 1000; ++i)
    {
      p -> Fill(i*Longitudinal_stepLength,Longitudinal_ion_energy_absorber[i]/Total_ion_energy_absorber);
    }
    
    
    float X_0 = 0.;
    for(int iStep = 0; iStep < PrimaryParticleN+1; ++iStep)
    {
      if( PrimaryParticleE[iStep]/InitialEnergy > 0.63 )
      {
        X_0 = PrimaryParticleZ[iStep]-InitialPositionZ;
      }
    }
    
    h2 -> Fill(X_0);
  }
  
  
  TCanvas* c1 = new TCanvas("c1","r_M");
  h -> GetXaxis() -> SetRangeUser(0.,100.);
  h -> GetXaxis() -> SetTitle("r_{M} [mm]");
  h -> Draw();
  TLatex* latex = new TLatex(0.5,0.5,Form("r_{M} = %1.1f mm",h->GetMean()));
  latex -> SetNDC();
  latex -> SetTextFont(42);
  latex -> SetTextColor(kBlack);
  latex -> SetTextSize(0.04);
  latex -> Draw("same");
  
  TCanvas* c2 = new TCanvas("c2","X_0");
  h2 -> GetXaxis() -> SetRangeUser(0.,500.);
  h2 -> GetXaxis() -> SetTitle("X_{0} [mm]");
  h2 -> Draw();
  TLatex* latex2 = new TLatex(0.5,0.5,Form("X_{0} = %1.1f mm",h2->GetMean()));
  latex2 -> SetNDC();
  latex2 -> SetTextFont(42);
  latex2 -> SetTextColor(kBlack);
  latex2 -> SetTextSize(0.04);
  latex2 -> Draw("same");
  
  TCanvas* c3 = new TCanvas("c3","long");
  p -> Draw("same");
}
