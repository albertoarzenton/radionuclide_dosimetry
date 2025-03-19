/*
    Reading ROOT TH3F file
    -------------------------

    #Authors: A. Arzenton, L. Morselli
    #Contact: alberto.arzenton@pd.infn.it

*/

void read_dose(std::string filename)
{
  // Open ROOT file

  //filename = "../build/" + filename + ".root";
  //outfilename = "../read/" + outfilename + ".txt";

  TFile * input_file = new TFile(filename.c_str());

  TH3F* hDose = (TH3F*)input_file->Get("hDose");

  int sh1 = hDose->GetNbinsX();
  int sh2 = hDose->GetNbinsY();
  int sh3 = hDose->GetNbinsZ();

  std::ofstream f("dose_map.txt");

  for (int i = 0; i < sh1; i++) {
    for (int j = 0; j < sh2; j++) {
      for (int k = 0; k < sh3; k++) {

      f << hDose->GetBinContent(i,j,k)*1.6022e-10 << " "; // MeV/g -> Gy

      }

    f << endl;

    }

  f << endl;

  }

}
