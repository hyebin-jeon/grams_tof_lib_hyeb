#include "tempDep.C"

void tdc_bin()
{

	std::vector<kFilePath> vPath;
	getPathList( &vPath );

	const char* bins = "tdc_calibration.bins";

	for( auto path: vPath )
	{
		auto dir = path.fpath;
		TString fname = Form("%s/%s", dir.Data(), bins);
    FILE *binsFile = fopen( fname.Data(), "r");
    if(binsFile == NULL) {
      fprintf(stderr, "Could not open '%s' for reading: %s\n", bins, strerror(errno));
      exit(1);
    }
    int nBins;
    float xMin, xMax;
    if(fscanf(binsFile, "%d\t%f\t%f\n", &nBins, &xMin, &xMax) != 3) { 
      fprintf(stderr, "Error parsing %s\n", bins);
      exit(1);
    }

		cout << Form( "%s: %d\t%f\t%f", fname.Data(), nBins, xMin, xMax ) << endl;
	}
}
