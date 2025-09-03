class CalParams
{
public:
  CalParams();
  virtual ~CalParams();
  double GetP0( unsigned int channelID, unsigned short tacID );
  double GetP1( unsigned int channelID, unsigned short tacID );
  double GetP2( unsigned int channelID, unsigned short tacID );
  double GetP3( unsigned int channelID, unsigned short tacID );
  double GetP4( unsigned int channelID, unsigned short tacID );
  double GetP5( unsigned int channelID, unsigned short tacID );
  double GetP6( unsigned int channelID, unsigned short tacID );
  double GetP7( unsigned int channelID, unsigned short tacID );
  double GetP8( unsigned int channelID, unsigned short tacID );
  double GetP9( unsigned int channelID, unsigned short tacID );
  
  double GetT0( unsigned int channelID, unsigned short tacID, bool isT = kTRUE );
  double GetA0( unsigned int channelID, unsigned short tacID, bool isT = kTRUE );
  double GetA1( unsigned int channelID, unsigned short tacID, bool isT = kTRUE );
  double GetA2( unsigned int channelID, unsigned short tacID, bool isT = kTRUE );

  void ReadTdcCalib( const char *fname );
  void ReadQdcCalib( const char *fname );
  
private:
  double T0[16][64][4][2];
  double A0[16][64][4][2];
  double A1[16][64][4][2];
  double A2[16][64][4][2];
  
  double P0[16][64][4];
  double P1[16][64][4];
  double P2[16][64][4];
  double P3[16][64][4];
  double P4[16][64][4];
  double P5[16][64][4];
  double P6[16][64][4];
  double P7[16][64][4];
  double P8[16][64][4];
  double P9[16][64][4];
};

CalParams::CalParams()
{
  for ( Int_t a_id = 0; a_id < 16; a_id++ ) {
    for ( Int_t c_id = 0; c_id < 64; c_id++ ) {
      for ( Int_t t_id = 0; t_id < 4; t_id++ ) {
	for ( Int_t b_id = 0; b_id < 2; b_id++ ) {
	  T0[a_id][c_id][t_id][b_id] = 0.0;
	  A0[a_id][c_id][t_id][b_id] = 0.0;
	  A1[a_id][c_id][t_id][b_id] = 0.0;
	  A2[a_id][c_id][t_id][b_id] = 0.0;
	}
	P0[a_id][c_id][t_id] = 0.0;
	P1[a_id][c_id][t_id] = 0.0;
	P2[a_id][c_id][t_id] = 0.0;
	P3[a_id][c_id][t_id] = 0.0;
	P4[a_id][c_id][t_id] = 0.0;
	P5[a_id][c_id][t_id] = 0.0;
	P6[a_id][c_id][t_id] = 0.0;
	P7[a_id][c_id][t_id] = 0.0;
	P8[a_id][c_id][t_id] = 0.0;
      }
    }
  }
}

CalParams::~CalParams()
{
}

double CalParams::GetP0( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P0[a_id][c_id][tacID];
}

double CalParams::GetP1( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P1[a_id][c_id][tacID];
}

double CalParams::GetP2( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P2[a_id][c_id][tacID];
}

double CalParams::GetP3( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P3[a_id][c_id][tacID];
}

double CalParams::GetP4( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P4[a_id][c_id][tacID];
}

double CalParams::GetP5( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P5[a_id][c_id][tacID];
}

double CalParams::GetP6( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P6[a_id][c_id][tacID];
}

double CalParams::GetP7( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P7[a_id][c_id][tacID];
}

double CalParams::GetP8( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P8[a_id][c_id][tacID];
}

double CalParams::GetP9( unsigned int channelID, unsigned short tacID )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return P9[a_id][c_id][tacID];
}

double CalParams::GetT0( unsigned int channelID, unsigned short tacID, bool isT )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return isT ? T0[a_id][c_id][tacID][0] : T0[a_id][c_id][tacID][1];
}

double CalParams::GetA0( unsigned int channelID, unsigned short tacID, bool isT )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return isT ? A0[a_id][c_id][tacID][0] : A0[a_id][c_id][tacID][1];
}

double CalParams::GetA1( unsigned int channelID, unsigned short tacID, bool isT )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return isT ? A1[a_id][c_id][tacID][0] : A1[a_id][c_id][tacID][1];
}

double CalParams::GetA2( unsigned int channelID, unsigned short tacID, bool isT )
{
  Int_t a_id = channelID/64;
  Int_t c_id = channelID%64;
  return isT ? A2[a_id][c_id][tacID][0] : A2[a_id][c_id][tacID][1];
}

void CalParams::ReadTdcCalib( const char *fname )
{
  std::ifstream finT( fname );
  unsigned short portID, slaveID, chipID, channelID, tacID;
  char branch;
  double t0, a0, a1, a2;
  TString head;
  char buf[256];
  unsigned short ndata = 0;
  do {
    finT >> head;
    if ( head.Contains( '#' ) ) {
      finT.getline( buf, 256 );
      continue;
    }
    if ( finT.eof() ) break;
    finT >> slaveID >> chipID >> channelID >> tacID >> branch >> t0 >> a0 >> a1 >> a2;
    //    std::cout << Form( "%u %u", chipID, channelID ) << std::endl;
    if ( branch == 'T' ) {
      T0[chipID][channelID][tacID][0] = t0;
      A0[chipID][channelID][tacID][0] = a0;
      A1[chipID][channelID][tacID][0] = a1;
      A2[chipID][channelID][tacID][0] = a2;
    }
    else {
      T0[chipID][channelID][tacID][1] = t0;
      A0[chipID][channelID][tacID][1] = a0;
      A1[chipID][channelID][tacID][1] = a1;
      A2[chipID][channelID][tacID][1] = a2;
    }
    ndata++;
  } while( 1 );
  std::cout << Form( "TDC Calibration Data %d lines", ndata ) << std::endl;
}

void CalParams::ReadQdcCalib( const char *fname )
{
  std::ifstream finQ( fname );
  unsigned short portID, slaveID, chipID, channelID, tacID;
  double p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;
  TString head;
  char buf[256];
  do {
    finQ >> head;
    if ( head.Contains( '#' ) ) {
      finQ.getline( buf, 256 );
      continue;
    }
    if ( finQ.eof() ) break;
    finQ >> slaveID >> chipID >> channelID >> tacID >> p0 >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7 >> p8 >> p9;
    //    std::cout << Form( "%u %u", chipID, channelID ) << std::endl;
    P0[chipID][channelID][tacID] = p0;
    P1[chipID][channelID][tacID] = p1;
    P2[chipID][channelID][tacID] = p2;
    P3[chipID][channelID][tacID] = p3;
    P4[chipID][channelID][tacID] = p4;
    P5[chipID][channelID][tacID] = p5;
    P6[chipID][channelID][tacID] = p6;
    P7[chipID][channelID][tacID] = p7;
    P8[chipID][channelID][tacID] = p8;
    P9[chipID][channelID][tacID] = p9;
  } while( 1 );
}

void TextDraw( Double_t x, Double_t y, TString t,
	       Double_t s, Int_t col = kBlack, Bool_t NDC = kFALSE );

Int_t  index( unsigned int cid );
double getTime  ( CalParams *calp, unsigned int cid, unsigned short tid, long long fid, unsigned short tcoarse, unsigned short tfine );
double getEnergy( CalParams *calp, unsigned int cid, unsigned short tid, long long fid, unsigned short ecoarse, unsigned short efine, double time );

//void plotraw3( const char *fname = "TOF2_PI4_QDC_raw.root", const char *dname = "../preBeam2" )
void plotraw3( const char *fname = "test_qdc_raw.root", const char *dname = "./" )
{
  gStyle->SetOptStat( 0 );
  gStyle->SetTitleFontSize( 0.07 );
  gStyle->SetTitleFont( 22, "" );
  gStyle->SetTitleW( 0.7 );
  gStyle->SetTitleH( 0.08 );
  gStyle->SetTitleY( 1.00 );
  gStyle->SetTextFont( 22 );
  gStyle->SetLabelFont( 22, "XY" );
  gStyle->SetTitleFont( 22, "XY" );
  gStyle->SetPadBottomMargin( 0.15 );
  gStyle->SetLabelSize( 0.05, "XY" );
  gStyle->SetTitleSize( 0.06, "XY" );
  //  gStyle->SetTitleOffset( 0.5 );
  gStyle->SetPadGridX( 1 );

  const char *tcalib = Form( "%s/tdc_calibration.tsv", dname );
  const char *qcalib = Form( "%s/qdc_calibration.tsv", dname );
  
  CalParams *calp = new CalParams();
  calp->ReadTdcCalib( tcalib );
  calp->ReadQdcCalib( qcalib );
  
  TFile f( Form( "%s/%s", dname, fname ) );
  if ( !f.IsOpen() ) return;

  TTree *hData = (TTree *)f.Get( "data" );
  if ( !hData ) return;

  // ROOT Tree fields
  float		brStep1;
  float		brStep2;
  long long 	brStepBegin;
  long long 	brStepEnd;
  
  long long	brFrameID;
  unsigned int	brChannelID;
  unsigned short	brTacID;
  unsigned short	brTCoarse;
  unsigned short	brECoarse;
  unsigned short	brTFine;
  unsigned short	brEFine;
  
  hData->SetBranchAddress("step1", &brStep1 );
  hData->SetBranchAddress("step2", &brStep2);
  hData->SetBranchAddress("frameID", &brFrameID);
  hData->SetBranchAddress("channelID", &brChannelID);
  hData->SetBranchAddress("tacID", &brTacID);
  hData->SetBranchAddress("tcoarse", &brTCoarse);
  hData->SetBranchAddress("ecoarse", &brECoarse);
  hData->SetBranchAddress("tfine", &brTFine);
  hData->SetBranchAddress("efine", &brEFine);

  Double_t etime[4];
  Double_t echrg[4];
  Double_t efine[4];
  
  Int_t n = hData->GetEntries();

  Int_t nhit = 0;

  double frequency = 200E6;
  double Tps = 1E12/frequency;

  TH1D *hist = new TH1D( "hist", "Time resolution [ps]", 1024, -1024.0*16, 1024.0*16 );
  TH1I *histn = new TH1I( "histn", "histn", 5, 0, 5 );

  TH2D *tof[2];
  TH1D *qdiv[2];
  TH1D *qdivf[2];
  for ( Int_t i = 0; i < 2; i++ ) {
    tof[i] = new TH2D( Form( "tof%d", i ), "tof", 128, 0.0, 16.0, 128, 0.0, 16.0 );
    qdiv[i] = new TH1D( Form( "qdiv%d", i ), "qdiv", 128, -1.0, 1.0 );
    qdivf[i] = new TH1D( Form( "qdivf%d", i ), "qdivf", 128, -1.0, 1.0 );
  }

  
  
  TH1D *qhist[4][4];
  TH1D *ehist[4][4];
  for ( Int_t i = 0; i < 4; i++ ) {
    for ( Int_t j = 0; j < 4; j++ ) {
      qhist[i][j] = new TH1D( Form( "qhist%d%d", i, j ), "qhist", 128, 0.0, 16.0 );
      ehist[i][j] = new TH1D( Form( "ehist%d%d", i, j ), "ehist", 256, 64.0, 256.0+64.0 );
    }
  }

  float offset[4] = { 130.0, 165.0, 105.0, 105.0 };
  bool hitch[4] = { kFALSE, kFALSE, kFALSE, kFALSE };
  long long lastTime = -1;
  long long currTime = -1;
  for ( Int_t i = 0; i < n; i++ ) {
    hData->GetEvent( i );
    currTime = brFrameID*1024 + brTCoarse;
    if ( lastTime == -1 ) lastTime = currTime;
    if ( TMath::Abs( currTime - lastTime ) < 2 ) {
      nhit++;
      etime[index(brChannelID)] = getTime  ( calp, brChannelID, brTacID, brFrameID, brTCoarse, brTFine );
      echrg[index(brChannelID)] = getEnergy( calp, brChannelID, brTacID, brFrameID, brECoarse, brEFine, etime[index(brChannelID)] );
      efine[index(brChannelID)] = brEFine;
      hitch[index(brChannelID)] = kTRUE;
    }
    else {
      if ( nhit == 4 ) {
	      Double_t tdiff = ((etime[1]-etime[3])+(etime[0]-etime[2]))*Tps/2.0;
	      //	std::cout << Form( "nhit = %d: Tdiff = %10.2lf", nhit, tdiff ) << std::endl;
	      hist->Fill( tdiff );
	      tof[0]->Fill( echrg[0], echrg[1] );
	      tof[1]->Fill( echrg[2], echrg[3] );
	      qdiv[0]->Fill( (echrg[0]-echrg[1]) / (echrg[0]+echrg[1]) );
	      qdiv[1]->Fill( (echrg[2]-echrg[3]) / (echrg[2]+echrg[3]) );
	      qdivf[0]->Fill( (efine[0]-efine[1]) / (efine[0]+efine[1]) );
	      qdivf[1]->Fill( (efine[2]-efine[3]) / (efine[2]+efine[3]) );
      }
      /*
      if ( nhit == 2 ) {
	for ( Int_t i = 0; i < 4; i++ ) {
	  if ( hitch[i] ) std::cout << Form( "%d ", i );
	}
	std::cout << std::endl;
      }
      */
      
      histn->Fill( nhit );
      for ( Int_t i = 0; i < 4; i++ ) {
	if ( echrg[i] > 0.0 ) {
	  qhist[i][nhit-1]->Fill( echrg[i] );
	  ehist[i][nhit-1]->Fill( efine[i] );
	}
      }
      etime[0] = etime[1] = etime[2] = etime[3] = 0;
      echrg[0] = echrg[1] = echrg[2] = echrg[3] = 0;
      efine[0] = efine[1] = efine[2] = efine[3] = 0;
      hitch[0] = hitch[1] = hitch[2] = hitch[3] = kFALSE;
      etime[index(brChannelID)] = getTime  ( calp, brChannelID, brTacID, brFrameID, brTCoarse, brTFine );
      echrg[index(brChannelID)] = getEnergy( calp, brChannelID, brTacID, brFrameID, brECoarse, brEFine, etime[index(brChannelID)] );
      efine[index(brChannelID)] = brEFine;
      hitch[index(brChannelID)] = kTRUE;
      lastTime = currTime;
      nhit = 1;
    }
  }
  
  //  hData->Print();
  TCanvas *c1 = new TCanvas( "c1", "c1" );
  //  hData->Draw( "tcoarse" );
  hist->GetXaxis()->SetTitle( "[ps]" );
  hist->GetXaxis()->SetTitle( "[ps]" );
  hist->Draw();
  hist->Fit( "gaus" );
  Double_t peak = hist->GetFunction( "gaus" )->GetParameter( 1 );
  Double_t sigm = hist->GetFunction( "gaus" )->GetParameter( 2 );
  hist->Fit( "gaus", "", "", peak-1.0*sigm, peak+1.0*sigm );
  peak = hist->GetFunction( "gaus" )->GetParameter( 1 );
  sigm = hist->GetFunction( "gaus" )->GetParameter( 2 );
  hist->Fit( "gaus", "", "", peak-1.0*sigm, peak+1.0*sigm );
  peak = hist->GetFunction( "gaus" )->GetParameter( 1 );
  sigm = hist->GetFunction( "gaus" )->GetParameter( 2 );
  hist->Fit( "gaus", "", "", peak-2.0*sigm, peak+2.0*sigm );
  TextDraw( 0.50, 0.80, Form( "#sigma = %6.2lf ps", sigm ), 0.08, kRed, kTRUE );
  TString filename = fname;
  filename.ReplaceAll( "root", "png" );
  c1->SaveAs( filename );
  c1->SaveAs( "plotraw3.png" );
  c1->Clear();
  histn->Draw();
  c1->SaveAs( "plotraw2n.png" );
  for ( Int_t j = 0; j < 4; j++ ) {
    c1->Clear();
    c1->Divide( 2, 2 );
    for ( Int_t i = 0; i < 4; i++ ) {
      c1->cd( i+1 );
      qhist[j][i]->Draw();
    }
    c1->SaveAs( Form( "plotraw2q%d.png", j ) );
  }
  for ( Int_t j = 0; j < 4; j++ ) {
    c1->Clear();
    c1->Divide( 2, 2 );
    for ( Int_t i = 0; i < 4; i++ ) {
      c1->cd( i+1 );
      ehist[j][i]->Draw();
    }
    c1->SaveAs( Form( "plotraw2e%d.png", j ) );
  }
  c1->Clear();
  tof[0]->Draw();
  c1->SaveAs( "tof0.png" );
  c1->Clear();
  tof[1]->Draw();
  c1->SaveAs( "tof1.png" );
  c1->Clear();
  qdiv[0]->Draw();
  c1->SaveAs( "qdiv0.png" );
  c1->Clear();
  qdiv[1]->Draw();
  c1->SaveAs( "qdiv1.png" );
  c1->Clear();
  qdivf[0]->Draw();
  c1->SaveAs( "qdivf0.png" );
  c1->Clear();
  qdivf[1]->Draw();
  c1->SaveAs( "qdivf1.png" );
}

Int_t index( unsigned int cid )
{
  if      ( cid == 512 ) return 0;
  else if ( cid == 513 ) return 1;
  else if ( cid == 576 ) return 2;
  else if ( cid == 577 ) return 3;
  else if ( cid == 640 ) return 0;
  else if ( cid == 641 ) return 1;
  else if ( cid == 704 ) return 2;
  else if ( cid == 705 ) return 3;
  else if ( cid == 128 ) return 0;
  else if ( cid == 129 ) return 1;
  else if ( cid == 192 ) return 2;
  else if ( cid == 193 ) return 3;
  else if ( cid == 256 ) return 0;
  else if ( cid == 257 ) return 1;
  else if ( cid == 192+128 ) return 2;
  else if ( cid == 193+128 ) return 3;
  else return 0;
}

double getTime( CalParams *calp, unsigned int cid, unsigned short tid, long long fid, unsigned short tcoarse, unsigned short tfine )
{
  double t0 = calp->GetT0( cid, tid );
  double a0 = calp->GetA0( cid, tid );
  double a1 = calp->GetA1( cid, tid );
  double a2 = calp->GetA2( cid, tid );
  
  float q_T = ( -a1 + sqrtf((a1*a1) - (4.0f * (a0 - tfine) * a2))) / (2.0f * a2);

  return double((fid*1024+tcoarse)) - q_T - t0;
}

double getEnergy( CalParams *calp, unsigned int cid, unsigned short tid, long long fid, unsigned short ecoarse, unsigned short efine, double time )
{
  double p0 = calp->GetP0( cid, tid );
  double p1 = calp->GetP1( cid, tid );
  double p2 = calp->GetP2( cid, tid );
  double p3 = calp->GetP3( cid, tid );
  double p4 = calp->GetP4( cid, tid );
  double p5 = calp->GetP5( cid, tid );
  double p6 = calp->GetP6( cid, tid );
  double p7 = calp->GetP7( cid, tid );
  double p8 = calp->GetP8( cid, tid );
  double p9 = calp->GetP9( cid, tid );

  double frequency = 200E6;
  double Tps = 1E12/frequency;

  double timeEnd = double(fid*1024+ecoarse);
  double energy  = double(efine);

  float ti = (timeEnd - time);
  float t_eq = ti;
  float delta = 0;
  int iter = 0;

  do {
    float f = (p0 - efine) +
      p1 * t_eq + 
      p2 * t_eq * t_eq + 
      p3 * t_eq * t_eq * t_eq + 
      p4 * t_eq * t_eq * t_eq * t_eq +
      p5 * t_eq * t_eq * t_eq * t_eq * t_eq + 
      p6 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq + 
      p7 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq + 
      p8 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq +
      p9 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq;
    
    float f_ = p1 +
      p2 * t_eq * 2 + 
      p3 * t_eq * t_eq * 3 + 
      p4 * t_eq * t_eq * t_eq * 4 +
      p5 * t_eq * t_eq * t_eq * t_eq * 5 + 
      p6 * t_eq * t_eq * t_eq * t_eq * t_eq * 6 + 
      p7 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * 7 + 
      p8 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * 8 +
      p9 * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * t_eq * 9;
    
    delta = - f / f_;
    
    // Avoid very large steps
    if(delta < -10.0) delta = -10.0;
    if(delta > +10.0) delta = +10.0;
    
    t_eq = t_eq + delta;
    iter += 1;
  } while ((fabs(delta) > 0.05) && (iter < 100));
  
  // Express energy as t_eq - actual integration time
  // WARNING Adding 1.0 clock to shift spectrum into positive range
  // .. needs better understanding.
  energy = t_eq - ti;
  
  return energy;
}

void TextDraw( Double_t x, Double_t y, TString t,
	       Double_t s, Int_t col, Bool_t NDC )
{
  TLatex *text = new TLatex( x, y, t );
  text->SetTextSize( s );
  text->SetTextColor( col );
  if ( NDC ) text->SetNDC();
  text->Draw();
}
