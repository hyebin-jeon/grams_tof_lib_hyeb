
#pragma once

#include <TROOT.h>
#include <TColor.h>
#include <TH1D.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>

using namespace std;

class HLX_AttribManager {

  private:
    explicit HLX_AttribManager(int pal=0) {
      
      //Kenichi
      if(pal==0) {
        fTColorVec.push_back(new TColor(fCurrentColor + 0, 237.0 / 255, 28.0 / 255, 36.0 / 255)); //blue
        fTColorVec.push_back(new TColor(fCurrentColor + 1, 3.0 / 255, 78.0 / 255, 162.0 / 255)); //red
        fTColorVec.push_back(new TColor(fCurrentColor + 2, 171.0 / 255, 225.0 / 255, 250.0 / 255)); //cyan
        fTColorVec.push_back(new TColor(fCurrentColor + 3, 57.0 / 255, 181.0 / 255, 74.0 / 255)); //green
        fTColorVec.push_back(new TColor(fCurrentColor + 4, 178.0 / 255, 210.0 / 255, 53.0 / 255)); //yellow green
        fTColorVec.push_back(new TColor(fCurrentColor + 5, 218.0 / 255, 111.0 / 255, 171.0 / 255)); //violet
        fTColorVec.push_back(new TColor(fCurrentColor + 6, 147.0 / 255, 149.0 / 255, 152.0 / 255)); //glay
      } else {
        //Scott
        int colPic = 8;
        fTColorVec.push_back(new TColor(fCurrentColor++, gROOT->GetColor(colPic)->GetRed(), gROOT->GetColor(colPic)->GetGreen(),  gROOT->GetColor(colPic)->GetBlue()));
        colPic = 9;
        fTColorVec.push_back(new TColor(fCurrentColor++, gROOT->GetColor(colPic)->GetRed(), gROOT->GetColor(colPic)->GetGreen(),  gROOT->GetColor(colPic)->GetBlue()));
        colPic = 38;
        fTColorVec.push_back(new TColor(fCurrentColor++, gROOT->GetColor(colPic)->GetRed(), gROOT->GetColor(colPic)->GetGreen(),  gROOT->GetColor(colPic)->GetBlue()));
        colPic = 46;
        fTColorVec.push_back(new TColor(fCurrentColor++, gROOT->GetColor(colPic)->GetRed(), gROOT->GetColor(colPic)->GetGreen(),  gROOT->GetColor(colPic)->GetBlue()));
        colPic = 36;
        fTColorVec.push_back(new TColor(fCurrentColor++, gROOT->GetColor(colPic)->GetRed(), gROOT->GetColor(colPic)->GetGreen(),  gROOT->GetColor(colPic)->GetBlue()));

        fFillHistos = false;
      }
    }

  public:
    ~HLX_AttribManager() = default;
    inline static double kAlphaDefault{0.85};

  public:
    static HLX_AttribManager* getInstance(int pal=0) {
      static HLX_AttribManager instance(pal);
      return &instance;
    }

  private:
    inline static size_t fSelectedColorIdx{0};

  private:
    vector<TColor*> fTColorVec;
    bool            fFillHistos{true};

  public:
    /// Get the next color in the list
    int getNextColor(){
      if(fSelectedColorIdx >= fTColorVec.size()) fSelectedColorIdx = 0;
      return fTColorVec[fSelectedColorIdx++]->GetNumber();
    }
    int resetColorIdx(){
      fSelectedColorIdx = 0;
      return fTColorVec[0]->GetNumber();
    }

  public:
    int fCurrentColor {TColor::GetFreeColorIndex()};

  public:
    void setAttribHist1D(TH1D *hist, int color, double alpha=kAlphaDefault)
    {
      if(!hist) return;
      hist->SetMarkerStyle(20);
      hist->SetMarkerSize (0.6);
      hist->SetMarkerColor(color);
      hist->SetLineColorAlpha(color, alpha);
      if(fFillHistos) hist->SetFillStyle  (1000);
      if(fFillHistos) hist->SetFillColorAlpha(color, alpha);
    }      

    void setAttribGraph(TGraph *g, int color, int markerStyle=8, int lineStyle=1, double alpha=kAlphaDefault)
    {
      if(!g) return;
      g->SetLineWidth(2);
      g->SetMarkerColorAlpha(color, alpha);
      g->SetLineColorAlpha  (color, alpha);
      g->SetMarkerStyle     (markerStyle);
      g->SetLineStyle       (lineStyle);
    }

    void setAttribLegend(TLegend *leg, double marginSize, double textSize, int fontType) {
      if(!leg) return;
      leg->SetFillStyle (0);
      leg->SetBorderSize(0);
      leg->SetTextSize  (textSize);
      leg->SetTextFont  (fontType);
      leg->SetMargin    (marginSize);
    }

		/// Set colors of TLegend entry colors
		/// colorOpt = "marker", "line", or "fill"
    void setAttribLegendEntries(TLegend *leg, std::string colorOpt){
      if(!leg) return;
			TList* l = leg->GetListOfPrimitives();
			TIter next(l);
			while (auto entry = (TLegendEntry *) next())
			{
				Color_t color=kBlack;

  		  if( entry->GetObject()->InheritsFrom(TH1::Class()) ) {
  		    auto obj = (TH1*) entry->GetObject();
  		    if( colorOpt=="marker" ) color = obj->GetMarkerColor();
  		    if( colorOpt=="line"   ) color = obj->GetLineColor();
  		    if( colorOpt=="fill"   ) color = obj->GetFillColor();
  		  }
  		  if( entry->GetObject()->InheritsFrom(TH2::Class()) ) {
  		    auto obj = (TH1*) entry->GetObject();
  		    if( colorOpt=="marker" ) color = obj->GetMarkerColor();
  		    if( colorOpt=="line"   ) color = obj->GetLineColor();
  		    if( colorOpt=="fill"   ) color = obj->GetFillColor();
  		  }
  		  if( entry->GetObject()->InheritsFrom(TGraph::Class()) ) {
  		    auto obj = (TGraph*) entry->GetObject();
  		    if( colorOpt=="marker" ) color = obj->GetMarkerColor();
  		    if( colorOpt=="line"   ) color = obj->GetLineColor();
  		    if( colorOpt=="fill"   ) color = obj->GetFillColor();
  		  }
  		  if( entry->GetObject()->InheritsFrom(TGraphErrors::Class()) ) {
  		    auto obj = (TGraphErrors*) entry->GetObject();
  		    if( colorOpt=="marker" ) color = obj->GetMarkerColor();
  		    if( colorOpt=="line"   ) color = obj->GetLineColor();
  		    if( colorOpt=="fill"   ) color = obj->GetFillColor();
  		  }
  		  entry->SetTextColor( color );
  		}
    }

    void drawText(double x, double y, TString t, double textSize, int color, int textAlign=11, bool fNDC=true) {
      TText *text = new TText(x, y, t);
      text->SetTextSize(textSize);
      text->SetTextColor(color);
      text->SetTextAlign(textAlign);
      if(fNDC) text->SetNDC();
      text->Draw();
    }
};

