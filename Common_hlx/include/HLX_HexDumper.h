////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Filename : HLX_HexDumper.h
/// Created  : 9/20/2019  by wakely
/// inspired by https://gist.github.com/shreyasbharath/32a8092666303a916e24a81b18af146b
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cmath>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "HLX_Logger.h"

using namespace std;

class HLX_HexDumper {
  public:
    HLX_HexDumper()=default;
    ~HLX_HexDumper()=default;

  public:
    template <class T>
    static void dump(const vector<T> &in, int rowsToShow=16, int columnsPerRow=16, bool fancy=true)
    {
      if(in.size()<1) return;

      int dataWidth{0};
      if(std::is_same<T,uint8_t>::value) dataWidth = 8;
      if(std::is_same<T,uint16_t>::value) dataWidth = 16;
      if(std::is_same<T,uint32_t>::value) dataWidth = 32;
      if(dataWidth == 0) return;
      if(dataWidth == 32 && columnsPerRow >8) columnsPerRow = 8; //prevent crazy line lengths

      if(columnsPerRow == 0) return;
      int fullRowCount  = floor(in.size()/columnsPerRow);          //rows with chars in each space
      int totalRowCount = round(in.size()/columnsPerRow+0.5);      //all rows, including partial
      int extraCount    = in.size()%columnsPerRow;                 //number of chars in partial row
      int totalRowCountDigits = (int)floor(log10(totalRowCount))+1;//
      if(totalRowCountDigits<2) totalRowCountDigits = 2;  //for uniformity in most readouts.

      //print("Data Width        = {}\n", dataWidth);
      //print("fullRowCount      = {}\n", fullRowCount );
      //print("totalRowCount     = {}\n", totalRowCount);
      //print("extraCount        = {}\n", extraCount   );
      //print("totalRowCountDigs = {}\n", totalRowCountDigits);

      bool elided{false};
      //hlog::info(hlog::kBlue, "Data size: {} | Cols per row: {} | Total Rows: {}", in.size(), columnsPerRow, totalRowCount);
      if(dataWidth==8)  hlog::info(hlog::kBlue, "Data size: {} bytes", in.size());
      if(dataWidth==16) hlog::info(hlog::kBlue, "Data size: {} words ({} bytes)", in.size(), in.size()*sizeof(T));
      if(dataWidth==32) hlog::info(hlog::kBlue, "Data size: {} dwords ({} words/{} bytes)", in.size(), in.size()*2, in.size()*sizeof(T));
      for(int row=0;row<=fullRowCount;row++){
        string fullBuff;
        string asciiBuff;
        if(row==rowsToShow/2 && fullRowCount>rowsToShow) row = fullRowCount-rowsToShow/2-1; //skip ahead
        if(row==fullRowCount && extraCount==0) break;
        if(row < rowsToShow/2 || row >= fullRowCount-rowsToShow/2){
          fullBuff += format(fg(color::yellow), "{:{}d} ", row, totalRowCountDigits);
          for(int i=0;i<columnsPerRow;i++){
            int idx = row*columnsPerRow + i;
            if(idx<(int)in.size()){
              fullBuff += format("{:0{}X} ", in[idx], dataWidth / 4);
              //build the string of ASCII values by bytes
              for(int byte= dataWidth / 8 - 1; byte >= 0; byte--) {
                int thisByte = (in[idx] >> byte * 8) & 0xFF;
                if (thisByte >= 0x20 && thisByte <= 0x7e) {
                  asciiBuff += (char)thisByte;
                } else {
                  asciiBuff += format(fg(color::cyan), ".");
                }
              }//loop over byes
            }
          }
          if(fancy) {
            //if(row==fullRowCount) for(int n=0;n<columnsPerRow-extraCount;n++) fullBuff += ""; // pad it out
            // add some padding for partial rows
            if(row==fullRowCount){
              for(int n=0;n<columnsPerRow-extraCount;n++){
                for(int b=0;b<dataWidth/4+1;b++) fullBuff += " "; // +1 to cover the space after
                for(int b=0;b<dataWidth/8;b++) asciiBuff += " "; // pad it out
              }
            }
            //fullBuff += asciiBuff;
            fullBuff = fullBuff + format(fg(color::cyan), "|") + asciiBuff + format(fg(color::cyan), "|");
          }
          hlog::info("{}", fullBuff); //need to use this format because fullBuff contains formatting
        } else {
          hlog::info(format(fg(color::yellow),"         [...{:d} rows skipped...]", fullRowCount-rowsToShow));
          elided = true;
        }
      }
    }

#if 0
    static void dumpOld(const vector<uint32_t> &inDat, int rowsToShow=16, int columnsPerRow=16, bool fancy=true){
      vector<uint8_t> out;
      for(const auto &el: inDat){
        out.push_back( (el>>24u)&0xFF );
        out.push_back( (el>>16u)&0xFF );
        out.push_back( (el>>8u) &0xFF );
        out.push_back( (el>>0u) &0xFF );
      }
      dumpOld(out, rowsToShow, columnsPerRow, fancy);
    }
    static void dumpOld(const vector<uint16_t> &inDat, int rowsToShow=16, int columnsPerRow=16, bool fancy=true){
      vector<uint8_t> out;
      for(const auto &el: inDat){
        out.push_back( (el>>8u) &0xFFu );
        out.push_back( (el>>0u) &0xFFu );
      }
      dumpOld(out, rowsToShow, columnsPerRow, fancy);
    }
    static void dumpOld(const vector<uint8_t> &in, int rowsToShow= 16, int columnsPerRow= 16, bool fancy= true)
    {
      if(columnsPerRow == 0) return;
      int fullRowCount  = floor(in.size()/columnsPerRow);     //rows with chars in each space
      int totalRowCount = floor(in.size()/columnsPerRow+0.5); //all rows, including partial
      int extraCount    = in.size()%columnsPerRow;               //number of chars in partial row
      int totalRowCountDigits = (int)floor(log10(totalRowCount))+1;

      bool elided{false};
      hlog::info(hlog::kBlue, "Data size: {} | Cols per row: {} | Total Rows: {}", in.size(), columnsPerRow, totalRowCount);
      for(int row=0;row<=fullRowCount;row++){
        string fullBuff;
        string asciiBuff;
        if(row==rowsToShow/2 && fullRowCount>rowsToShow) row = fullRowCount-rowsToShow/2-1; //skip ahead
        if(row==fullRowCount && extraCount==0) break;
        if(row < rowsToShow/2 || row >= fullRowCount-rowsToShow/2){
          fullBuff += format(fg(color::yellow), "{:{}d} ", row, totalRowCountDigits);
          //fullBuff += format(fg(color::yellow), "{: 6d}   ", row);
          for(int i=0;i<columnsPerRow;i++){
            int idx = row*columnsPerRow + i;
            if(idx<in.size()){
              fullBuff += format("{:02X} ", in[idx]);
              //build the string of ASCII values
              if(in[idx]>=0x20&&in[idx]<=0x7e){
                asciiBuff += (char)in[idx];
              } else {
                asciiBuff += format(fg(color::cyan), ".");
              }
            }
          }
          if(fancy) {
            if(row==fullRowCount) for(int n=0;n<columnsPerRow-extraCount;n++) fullBuff += "   "; // pad it out
            fullBuff += asciiBuff;
          }
          hlog::info("{}", fullBuff); //need to use this format because fullBuff contains formatting
        } else {
          hlog::info(format(fg(color::yellow),"         [...{:d} rows skipped...]", fullRowCount-rowsToShow));
          elided = true;
        }
      }
    }
#endif

};
