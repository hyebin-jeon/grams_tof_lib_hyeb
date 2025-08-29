#pragma once

#ifndef _TOF_TDCQDCCALIBRATION_
#define _TOF_TDCQDCCALIBRATION_

#include <iostream>
#include "TObject.h"
#include "TString.h"
#include "TOF_Constants.h"

class TOF_TdcQdcCalibration : public TObject
{
	public:
    TOF_TdcQdcCalibration() { 
			initializeParams(); 
		};
    TOF_TdcQdcCalibration( const char* fTdcCalib, const char* fQdcCalib )
		{
			initializeParams(); 
      readTdcCalib( fTdcCalib );
      readQdcCalib( fQdcCalib );
		}

    inline static TOF_TdcQdcCalibration * theCalib{nullptr}; 
		static TOF_TdcQdcCalibration *getInstance() {
			if( theCalib == nullptr ) {
				theCalib = new TOF_TdcQdcCalibration;
			}
			return theCalib;
		}

    ~TOF_TdcQdcCalibration() = default;

	public:
		void initializeParams();
	  /// QDC calibration parameters
    inline double getP0( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P0[chipID][channelID][tacID]; };
    inline double getP1( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P1[chipID][channelID][tacID]; };
    inline double getP2( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P2[chipID][channelID][tacID]; };
    inline double getP3( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P3[chipID][channelID][tacID]; };
    inline double getP4( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P4[chipID][channelID][tacID]; };
    inline double getP5( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P5[chipID][channelID][tacID]; };
    inline double getP6( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P6[chipID][channelID][tacID]; };
    inline double getP7( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P7[chipID][channelID][tacID]; };
    inline double getP8( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P8[chipID][channelID][tacID]; };
    inline double getP9( uint8_t chipID, uint32_t channelID, uint8_t tacID ) { return P9[chipID][channelID][tacID]; };
 
	  /// TDC calibration parameters. 
	  /// T branch params when isT = kTRUE, else E branch. isT is fixed to kTRUE
    inline double getT0( uint8_t chipID, uint32_t channelID, uint8_t tacID, bool isT = kTRUE ) { return isT? T0[chipID][channelID][tacID][0] : T0[chipID][channelID][tacID][1]; };
    inline double getA0( uint8_t chipID, uint32_t channelID, uint8_t tacID, bool isT = kTRUE ) { return isT? A0[chipID][channelID][tacID][0] : A0[chipID][channelID][tacID][1]; };
    inline double getA1( uint8_t chipID, uint32_t channelID, uint8_t tacID, bool isT = kTRUE ) { return isT? A1[chipID][channelID][tacID][0] : A1[chipID][channelID][tacID][1]; };
    inline double getA2( uint8_t chipID, uint32_t channelID, uint8_t tacID, bool isT = kTRUE ) { return isT? A2[chipID][channelID][tacID][0] : A2[chipID][channelID][tacID][1]; };

    void readTdcCalib( const char *fname );
    void readQdcCalib( const char *fname );
		void readCalibrationFiles( const char* fTdcCalib, const char* fQdcCalib );

	public:
		std::vector<uint8_t> getChipIDChannelID( uint32_t absChannelID );
		uint8_t getChipID( uint32_t absChannelID ); // 0 to 15
		uint8_t getChannelID( uint32_t absChannelID ); // 0 to 63


	public:
		double getCalibratedTime_T( uint32_t absChannelID, uint8_t tacID, long long frameID, unsigned short tcoarse, unsigned short tfine );
    double getCalibratedTime_E( uint32_t absChannelID, uint8_t tacID, long long frameID, unsigned short ecoarse, unsigned short efine );
    double getCalibratedQDC( uint32_t absChannelID, uint8_t tacID, long long frameID, unsigned short ecoarse, unsigned short efine, double time ); // was getEnergy().
    
		double getCalibratedTime( TOF_Mode branchMode, uint32_t absChannelID, uint8_t tacID, long long frameID, unsigned short coarse, unsigned short fine )
		{
			if( branchMode == TOF_Mode::fQdc )
				return getCalibratedTime_T(absChannelID, tacID, frameID, coarse, fine );
			else if ( branchMode == TOF_Mode::fTot )
			  return getCalibratedTime_E(absChannelID, tacID, frameID, coarse, fine );
			else 
			{
				//cout << "ERROR" << endl;
				return TOF_ERR_OUT_OF_RANGE;
			}
		}


	private:
	  /// param[chipID][channelID][tacID][T or E]
    double T0[16][64][4][2];
    double A0[16][64][4][2];
    double A1[16][64][4][2];
    double A2[16][64][4][2];
    
	  /// param[chipID][channelID][tacID]
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
	
	ClassDef(TOF_TdcQdcCalibration, 1)
};

#endif
