#pragma once

#ifndef _TOF_STRUCT_
#define _TOF_STRUCT_

#include "TObject.h"

class TOF_Struct : public TObject
{
  public:
    inline static TOF_Struct * theTofStr{nullptr}; 
		static TOF_Struct *getInstance() {
			if( theTofStr == nullptr )
			{
				TOF_Struct *theTofStr = new TOF_Struct;
			}
			return theTofStr;
		}

		~TOF_Struct() = default;

	public:
    struct TOF_TdcQdc {
      //uint32_t channelID;
      uint64_t frameID;
      uint8_t  tacID  ;
      uint16_t tCoarse;
      uint16_t eCoarse;
      uint16_t tFine  ;
      uint16_t eFine  ;
    };
	
		ClassDef(TOF_Struct, 1)

};

#endif

