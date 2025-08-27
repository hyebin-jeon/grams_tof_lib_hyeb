/* $Id: EnvPars.h,v 1.9 2009/03/28 11:06:12 kusumoto Exp $ */
#ifndef _ENV_PARS_H
#define _ENV_PARS_H

#include <cstdlib>
#include <cstring>
#include "TString.h"

#define PLIB getenv( "PLIB" )

// for DAQInterface
#define PDR_MOD_ENV  "PLIB_PDRV_MODULE"
#define PDR2_MOD_ENV "PLIB_PDRV2_MODULE"
#define PDR_PTH_ENV  "PLIB_PDRV_MODPATH"
#define PDR2_PTH_ENV "PLIB_PDRV2_MODPATH"
#define EHC_MOD_ENV  "PLIB_EHCI_MODULE"

#define PDR_MOD_DEF  "pdrv"
#define PDR2_MOD_DEF "pdrv2"
#define PDR_PTH_DEF  ONPLIB( "/pdrv/pdrv.ko"  )
#define PDR2_PTH_DEF ONPLIB( "/pdrv/pdrv2.ko" )
#define EHC_MOD_DEF  "ehci-hcd"

// for DiskManager
#define USB_DSK_ENV "PLIB_USBD_DEVICE"
#define USB_MNT_ENV "PLIB_USBD_MOUNT"
#define USB_TYP_ENV "PLIB_USBD_FSTYPE"
#define SCR_LOC_ENV "PLIB_LOCAL_SCRATCH"
#define SCR_USB_ENV "PLIB_USB_SCRATCH"
#define LBL_USB_ENV "PLIB_USBD_LABEL"
#define LBL_MEM_ENV "PLIB_USBM_LABEL"
#define LBL_FIL_ENV "PLIB_FILE_LABEL"
#define LBL_GND_ENV "PLIB_GDAT_LABEL"

#define USB_DSK_DEF "/dev/sda"
#define USB_MNT_DEF "/mnt/disk"
#define USB_TYP_DEF "ext3"
#define SCR_LOC_DEF "/scratch/bess/daq/data"
#define SCR_USB_DEF "/mnt/disk/daq/data"
#define LBL_USB_DEF "BESSP-DISK"
#define LBL_MEM_DEF "BESSP-USBMEM"
#define LBL_FIL_DEF "bessp_raw"
#define LBL_GND_DEF "bessp_ground"

// for DevIf
#define PDV_DEV_ENV  "PLIB_PDRV_DEVICE"
#define PDV2_DEV_ENV "PLIB_PDRV2_DEVICE"
#define BAS_FX2_ENV  "PLIB_BASE_FIRM_PATH"
#define FAD_FX2_ENV  "PLIB_FADC_FIRM_PATH"
#define MU2_FX2_ENV  "PLIB_MU2_FIRM_PATH"
#define CLK_FX2_ENV  "PLIB_CLK_FIRM_PATH"
#define PDV_DEV_DEF  "/dev/usb/pdrv"
#define PDV2_DEV_DEF "/dev/usb/pdrv2"
#define BAS_FX2_DEF ONPLIB("/fx2/hex/base_fx2.hex")
#define FAD_FX2_DEF ONPLIB("/fx2/hex/fadc_fx2.hex")
#define MU2_FX2_DEF ONPLIB("/fx2/hex/mu2_fx2.hex" )
#define CLK_FX2_DEF ONPLIB("/fx2/hex/clk_fx2.hex" )

// for LonControl
#define LON_SEQ_ENV "PLIB_LON_SEQUENCE"
#define LON_SEQ_DEF ONPLIB ("/dat/lon_seq.dat")
#define PWR_SEQ_ENV "PLIB_PWR_SEQUENCE"
#define PWR_SEQ_DEF ONPLIB ("/dat/pwr_seq.dat")

// for LonIf
#define LON_DEV_ENV "PLIB_LON_DEVICE"
#define LON_DEV_DEF "/dev/lonif"

// for GpsIf
#define GPS_INI_ENV "PLIB_GPS_INIT"
#define GPS_INI_DEF ONPLIB("/dat/gps_ini.dat")

// for Mu2Interface
#define MU2_ROM_ENV "MU2_ROM_PATH"
#define MU2_ROM_DEF "raw/flight.raw"

// local/UTC time zone
#define TIM_ZON_ENV "PLIB_TIME_ZONE"
#define TIM_ZON_DEF "local"

// for PctlMain
#define LSW_DBN_ENV "PLIB_LONSW_DATABASE"
#define LHV_DBN_ENV "PLIB_LONHV_DATABASE"
#define LSQ_DBN_ENV "PLIB_LON_SEQUENCE"

#define LSW_DBN_DEF ONPLIB("/dat/lonsw_db.dat")
#define LHV_DBN_DEF ONPLIB("/dat/lonhv_db.dat")
#define LSQ_DBN_DEF ONPLIB("/dat/lon_seq.dat" )

// for PmonMain, PmonText
#define LMN_DBN_ENV "PLIB_MON_DATABASE"
#define MAP_LYR_ENV "PLIB_MAP_LAYER"
#define LMN_DBN_DEF ONPLIB("/dat/lonmon_db.dat"  )
#define MAP_LYR_DEF ONPLIB("/dat/map_lyr%04d.dat")

// for ProcInfo
#define SNS_PTH_ENV "PLIB_SENSOR_PATH"
#define SNS_PTH_DEF "/proc/sys/dev/sensors/lm87-i2c-0-2e"

// for EventDisplay
#define PART_DB_ENV "PLIB_PARTICLE_DB"
#define PART_DB_DEF ONPLIB("/dat/part_table.dat")

// for pcom, pterm, plog
#define LON_HST_ENV "PLIB_LON_HOST"
#define SIP_HST_ENV "PLIB_SIP_HOST"
#define GSE_HST_ENV "PLIB_GSE_HOST"
#define ONL_HST_ENV "PLIB_ONL_HOST"
#define COM_SRV_ENV "PLIB_COM_SERV"
#define MON_SRV_ENV "PLIB_MON_SERV"

#define LON_HST_DEF "bessgx1-2"
#define SIP_HST_DEF "bessgx1-2"
#define GSE_HST_DEF "bessgx1-5"
#define ONL_HST_DEF "bessnx2"
#define COM_SRV_DEF "besspccom"
#define MON_SRV_DEF "besspcmon"

#define MAC_DIR_ENV "PLIB_MACRO_PATH"
#define MAC_DIR_DEF ONPLIB("/macro/command")

// for pdaq, plon
#define COM_HST_ENV "PLIB_COM_HOST"
#define DAQ_HST_ENV "PLIB_DAQ_HOST"
#define DQM_SRV_ENV "PLIB_DQM_SERV"
#define DQC_SRV_ENV "PLIB_DQC_SERV"
#define TRG_FIL_ENV "PLIB_DQB_FILE"
#define PMT_ENB_ENV "PLIB_ENB_FILE"

#define COM_HST_DEF "bessgx1"
#define DAQ_HST_DEF "besscpci1-gb"
#define DQM_SRV_DEF "bessdqmon"
#define DQC_SRV_DEF "bessdqcom"
#define TRG_FIL_DEF "trg_bit.dat"
#define PMT_ENB_DEF "pmt_enb.dat"

// for praw
#define JCW_ASG_ENV "PLIB_JCW_ASGN"
#define ICW_ASG_ENV "PLIB_ICW_ASGN"
#define JCW_ASG_DEF "dat/jcwire.dat"
#define ICW_ASG_DEF "dat/icwire.dat"

// for proot
#define ASG_DBN_ENV "PLIB_ASGN_DATABASE"
#define JET_WPS_ENV "PLIB_JET_WIRE"
#define IDC_WPS_ENV "PLIB_IDC_WIRE"
#define ELS_TBL_ENV "PLIB_ELOSS_TABLE"
#define PAR_DBN_ENV "PLIB_PARAMS_DATABASE"
#define PAR_DIR_ENV "PLIB_PARAMS_DIRECTORY"
#define JET_XTP_ENV "PLIB_JET_XTPAR"
#define ASG_DBN_DEF ONPLIB("/dat/assign_db.dat" )
#define JET_WPS_DEF ONPLIB("/dat/jcwpos.dat"    )
#define IDC_WPS_DEF ONPLIB("/dat/icwpos.dat"    )
#define ELS_TBL_DEF ONPLIB("/dat/elosstable.dat")
#define PAR_DBN_DEF ONPLIB("/dat/params_db.dat" )
#define PAR_DIR_DEF "/scratch/bess/exp65/params"
#define JET_XTP_DEF "xtpar.dat"

#define MON_DIR_ENV "PLIB_MON_DIRECTORY"
#define MON_DIR_DEF "/scratch/bess/exp75/mon"

// for pcmp
#define FADC_PED_ENV "PLIB_FPED_DATA"
#define FADC_PED_DEF ONPLIB("/dat/fped.dat")

// GETENV macro
#define GETENV(env,def) ( (getenv( env )) ? getenv( env ) : def )
#define ONPLIB(env) ( Form( "%s%s",PLIB, env ))
#endif

// Local Variables:
// mode:C++
// End:
