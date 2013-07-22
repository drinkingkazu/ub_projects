/**
 * \file TrigPulser.hh
 *
 * \ingroup PMTReadOut
 * 
 * \brief Class def for a toy pulser 
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup PMTReadOut

    @{*/

#ifndef TRIGPULSER_HH
#define TRIGPULSER_HH

#include "types.hh"
#include "types_pcie.hh"
#include "nevis_pcie_utils.c"

/**
   \class TrigPulser
   A simple class to generate two pulses
 */
class TrigPulser {

public:

  TrigPulser();

  ~TrigPulser(){};

  void SetDevicePtr(WDC_DEVICE_HANDLE *p){_dev_ptr=p;};

  void SetModulePtr(PCIE::mod_address_t  *p){_module=p;};

  void SetP1Delay(PMT::timeslice_t d){_p1_delay=d;};

  void SetP2Delay(PMT::timeslice_t d){_p2_delay=d;};

  void SetP1Width(PMT::timeslice_t w){_p1_width=w;};

  void SetP2Width(PMT::timeslice_t w){_p2_width=w;};

  void SetTrigDelay(PMT::timeslice_t d){_trig_pulse_delay=d;};

  void SetNumPulse(uint32_t n){_npulse=n;};

  void SetSleepTime_AfterPrep(uint32_t t){_sleep_after_prep=t;};

  void SetSleepTime_AfterPulse(uint32_t t){_sleep_after_pulse=t;};

  bool Prepare();

  bool Fire();

protected:

  void InitD();

  PMT::timeslice_t _p1_delay;
  PMT::timeslice_t _p2_delay;

  PMT::timeslice_t _p1_width;
  PMT::timeslice_t _p2_width;

  PMT::timeslice_t _trig_pulse_delay;

  bool _ready;
  uint32_t _sleep_after_prep;
  uint32_t _sleep_after_pulse;
  uint32_t _npulse;

  PCIE::mod_address_t *_module;
  PCIE::buff_send_t _buf_send;

  WDC_DEVICE_HANDLE *_dev_ptr;

};
#endif

/** @} */ // end of doxygen group
