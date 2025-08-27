#pragma once

#ifndef _TOF_ATTRIBUTES_
#define _TOF_ATTRIBUTES_

#include "TObject.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TText.h"
#include "TPaveStats.h"
#include "TCollection.h"
#include "TLegendEntry.h"
//#include "TIter.h"

#include "TOF_Constants.h"

class TOF_Attributes : public TObject
{
	public:
    inline static TOF_Attributes * theAttr{nullptr}; 
  	static TOF_Attributes *getInstance() {
  		if( theAttr == nullptr ) {
  			theAttr = new TOF_Attributes;
  		}
  
  		return theAttr;
  	}
  
  	~TOF_Attributes() = default;

	private:
    static constexpr int    fPrettyGreen = 30 ;
    static constexpr double fPrettyAlpha_hist  = 0.3;
    static constexpr double fPrettyAlpha_graph = 0.6;
		static constexpr int    fFillSolid = 1000;
		static constexpr int    fFillHollow = 0;

	public:
    /// Graph
    void AttribGraph( TGraph* g );
    void AttribGraph( TGraph* g, int color, int marker, int line=1, double alpha=0.6, int lineW=1 );

		/// Histogram
    void AttribHist( TH1* hist );
    void AttribHist( TH1* hist, int color, double alpha, int fill=fFillSolid );
    void AttribTH2 ( TH2* hist, int color, int fill=fFillSolid );
    void AttribProf( TProfile* prof, int color );
    
    void MoveStatBox   ( TPaveStats* ps, double x0, double y0, double x1, double y1 );
    int  MoveStatBoxNDC( TPaveStats* ps, double x0, double y0, double x1, double y1 );

    /// TLegend
    void SetAttribLegendEntries(TLegend *leg, std::string colorOpt);
    void SetAttribLegend(TLegend *leg, double marginSize, double textSize, int fontType); 


    void DrawText   ( double size, int align, int color, double x0, double y0, const char* tt);
    void DrawTextNDC( double size, int align, int color, double x0, double y0, const char* tt);
    void DrawText   ( double size, int align, int color, int font, double x0, double y0, const char* tt);
    void DrawTextNDC( double size, int align, int color, int font, double x0, double y0, const char* tt);

    /// Axis with time label
    void setTimeXaxis( TGraph* g, bool kUTC=false );
    void setTimeXaxis( TGraph* g );
    void setTimeXaxis( TMultiGraph* g, double tmin=0, double tmax=0 );

    /// Axis
    //void AttribGraphAxis( TGraph* g );
    void AttribYaxis( TGraph* g, double labelSize, double titleOffset, double titleSize, double ymin, double ymax );
    void AttribYaxis( TMultiGraph* g, double labelSize, double titleOffset, double titleSize, double ymin=0, double ymax=0 );
	
  ClassDef(TOF_Attributes, 1)
};

#endif
