#include "CascadeGenerator.hh"

CascadeGenerator::CascadeGenerator(InputManager* aInMgr) {

  InMgr = aInMgr;

  InMgr->GetVariable("run_start",N_run);
//  N_run-=1;
  n_gamma = 5;//no of gammas
  InMgr->GetVariable("n_bin",n_bin);
  n_bin+=1;
  InMgr->GetVariable("run_end",run_end);
  run_end-=1;
  count = 0;
  InMgr->GetVariable("E_x",E_x);//excited state MeV

  end = false;
  custom = false;

  dE = E_x/double(n_bin-1);

  E_array.resize(n_bin);
  gamma_array.resize(n_gamma);
  point_array.resize(n_bin-1+n_gamma);

  string CascType;
  InMgr->GetVariable("CascType",CascType);

  if (CascType == "Custom") custom = GenerateCascadeCustom();
  if (CascType == "Regular") GenerateCascade();
  
  it = gammatot_array.begin();

}

//-------------------------------------------------------------------
//generates individual cascade for run
void CascadeGenerator::SetCascade() {

  if (custom == true) {
    for (int i=0; i<gammatot_array.size(); i++) {
      cascade.push_back(gammatot_array.at(i));
      G4cout << gammatot_array.at(i) << "\t";
    }
    G4cout << G4endl;
    end = true;
    return;
  }

  if ((N_run)*n_gamma>=gammatot_array.size() || N_run>run_end) {
   end = true;
//   G4cout << "END!!" << G4endl;
//   return;
  }

  cascade.clear();
  G4cout << N_run << "\t";

  for (int i=N_run*n_gamma; i<N_run*n_gamma+n_gamma; i++) {
    if (gammatot_array.at(i)>0) {
      cascade.push_back(gammatot_array.at(i));
      G4cout << gammatot_array.at(i) << "\t";
    }
  }

  G4cout << G4endl;

}

//-------------------------------------------------------------------

std::vector<double> CascadeGenerator::GetCascade() {
  return cascade;
}

bool CascadeGenerator::GenerateCascadeCustom() {

  double Ein;

  InMgr->GetVariable("E0",Ein);
  gammatot_array.push_back(Ein);//MeV

  InMgr->GetVariable("E1",Ein);
  gammatot_array.push_back(Ein);//MeV

  InMgr->GetVariable("E2",Ein);
  gammatot_array.push_back(Ein);//MeV

  InMgr->GetVariable("E3",Ein);
  gammatot_array.push_back(Ein);//MeV

  InMgr->GetVariable("E4",Ein);
  gammatot_array.push_back(Ein);//MeV

  return true;

}

//-------------------------------------------------------------------
//generates all possible cascades for excited state, E in MeV
void CascadeGenerator::GenerateCascade() {

  for (int i=0; i<n_bin; i++) {
    E_array.at(i) = dE*i;
  }

  for (int i=0; i<n_gamma; i++) {
    gamma_array.at(i) = 0;
  }

  for (int i=n_gamma; i<n_bin-1+n_gamma; i++) {
    point_array.at(i) = 1;
  }

  for (int i=0; i<n_bin-1+n_gamma; i++) {

    for (int j=i+1; j<n_bin-1+n_gamma; j++) {

      for (int k=j+1; k<n_bin-1+n_gamma; k++) {

        for (int l=k+1; l<n_bin-1+n_gamma; l++) {

          for (int m=l+1; m<n_bin-1+n_gamma; m++) {

            for (int a=0; a<n_bin-1+n_gamma; a++) {
              point_array.at(a) = 1;
            }

            point_array.at(i) = 0;
            point_array.at(j) = 0;
            point_array.at(k) = 0;
            point_array.at(l) = 0;
            point_array.at(m) = 0;

            int gamma_count = 0;
            int decay_count = 0;
            double E_sum = 0;
            for (int a=0; a<n_bin-1+n_gamma; a++) {

              if (point_array.at(a) == 0) {
                gamma_array.at(decay_count) = E_array.at(gamma_count);
                E_sum += gamma_array.at(decay_count);
                decay_count += 1;
              }
              if (point_array.at(a) == 1) gamma_count+=1;

            }

            count+=1;
/*
            G4cout << count << "\t";
            for (int a=0; a<n_bin-1+n_gamma; a++) {
              G4cout << point_array.at(a) << "\t";
            }
*/
            if (E_sum/E_x-1<0.001 && E_sum/E_x-1>-0.001) {
//              G4cout << G4endl;
//              G4cout << E_sum << "\t";
              for (int a=0; a<n_gamma; a++) {
//                G4cout << gamma_array.at(a) << "\t";
                gammatot_array.push_back(gamma_array.at(a));
              }
            }
//            G4cout << G4endl;
          }

        }

      }

    }

  }
//  G4cout << G4endl;
}
