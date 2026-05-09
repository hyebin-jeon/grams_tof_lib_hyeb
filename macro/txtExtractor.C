
void txtExtractor()
{
	
	const char* finPath = "run_2026-01-22_22-22-37.717Z.stg1.root";
	
	TString dir   = std::filesystem::current_path().string() + "/output"; // Stg2 path 

	TString name1 = finPath;

	size_t idx = name1.Index(".stg1.root");
	TString name = (TString) name1(0, idx);
	
	const char* kPathStg2 = Form( "%s/%s.stg2.root", dir.Data(), name.Data() );

	cout << "name2: " << finPath << endl;
	cout << "index: " << idx << endl;
	cout << "sub  : " <<  name1(0, idx) << endl;
	cout << "name : " <<  name << endl;
	cout << kPathStg2 << endl;

}
