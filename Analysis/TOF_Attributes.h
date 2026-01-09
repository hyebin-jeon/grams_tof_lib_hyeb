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
		static constexpr int    fFillSolid = 1001;
		static constexpr int    fFillHollow = 0;

	public:
    /// Graph
    void attribGraph( TGraph* g );
    void attribGraph( TGraph* g, int color, int marker, int line=1, double alpha=0.6, int lineW=1 );
    void attribGraphErrors( TGraphErrors* g );
    void attribGraphErrors( TGraphErrors* g, int color, int marker, int line=1, double alpha=0.6, int lineW=1 );

		/// Histogram
    void attribHist( TH1* hist );
    void attribHist( TH1* hist, int color, double alpha, int fill=fFillSolid );
    void attribTH2 ( TH2* hist, int color, int fill=fFillSolid );
    void attribProf( TProfile* prof, int color );
    
    void moveStatBox   ( TPaveStats* ps, double x0, double y0, double x1, double y1 );
    int  moveStatBoxNDC( TPaveStats* ps, double x0, double y0, double x1, double y1 );

    /// TLegend
    void setAttribLegendEntries(TLegend *leg, std::string colorOpt);
    void setAttribLegend(TLegend *leg, double marginSize, double textSize, int fontType); 


    void drawText   ( double size, int align, int color, double x0, double y0, const char* tt);
    void drawTextNDC( double size, int align, int color, double x0, double y0, const char* tt);
    void drawText   ( double size, int align, int color, int font, double x0, double y0, const char* tt);
    void drawTextNDC( double size, int align, int color, int font, double x0, double y0, const char* tt);

    /// Axis with time label
    void setTimeXaxis( TGraph* g, bool kUTC=false );
    void setTimeXaxis( TGraph* g );
    void setTimeXaxis( TMultiGraph* g, double tmin=0, double tmax=0 );

    /// Axis
    //void AttribGraphAxis( TGraph* g );
    void attribYaxis( TGraph* g, double labelSize, double titleOffset, double titleSize, double ymin, double ymax );
    void attribYaxis( TMultiGraph* g, double labelSize, double titleOffset, double titleSize, double ymin=0, double ymax=0 );
	
  ClassDef(TOF_Attributes, 1)
};

#endif
