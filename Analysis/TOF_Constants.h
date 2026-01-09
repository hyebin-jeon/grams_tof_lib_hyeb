#pragma once


#define CLOCKS_IN_A_FRAME 1024

#define NUMBER_OF_ASICS   16
#define NUMBER_OF_CHANNELS_OF_ASIC 64
//#define NUMBER_OF_TAC_OF_CHANNEL 4

enum class TOF_Mode {
  fQdc = 0,
  fTot = 1,
};

enum class TOF_Branch {
  fBranchT = 0,
  fBranchE = 1,
};

enum class TOF_Discriminator {
  fDiscT1 = 0,
  fDiscT2 = 2,
  fDiscE  = 3,
};

#define TOF_ERR_OUT_OF_RANGE -1

