/*
    Converting TXT to ROOT file
    ---------------------------

    #Authors: A. Arzenton, L. Morselli
    #Contact: alberto.arzenton@pd.infn.it
*/

void txttoroot(std::string filename)
{
  std::ifstream file (filename.c_str(), std::ios::in);
  std::string buffer;

  TH3F* histo = new TH3F("histo","",148,0,148,160,0,160,160,0,160);

  int i, j, k;
  double act = 0.;

  while (file >> buffer) {

    i = std::stoi(buffer);
    file >> buffer;
    j = std::stoi(buffer);
    file >> buffer;
    k = std::stoi(buffer);
    file >> buffer;
    act = std::stod(buffer);
    histo->SetBinContent(i,j,k,act);

    }

  TFile* et = new TFile("PET_1.root", "RECREATE");
  histo->Write();
  et->Close();
}
