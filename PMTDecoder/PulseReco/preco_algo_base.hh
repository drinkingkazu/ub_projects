#ifndef PRECO_ALGO_BASE_HH
#define PRECO_ALGO_BASE_HH

#include "Base-TypeDef.hh"
#include <TString.h>
#include <cmath>
#include <functional>
#include <numeric>

struct pulse_param{
  
  double peak, area;
  double t_start, t_max, t_end;
  
  pulse_param(){
    reset_param();
  };
  
  void reset_param(){      
    peak = area = -1;
    t_start = t_max = t_end = -1;
  };
  
};


class preco_algo_base : public decoder_base {

public:

  preco_algo_base();

  virtual ~preco_algo_base(){};

  virtual void reset();

  virtual bool reco(const std::vector<uint16_t> *wf)=0;

  const pulse_param* get_pulse(size_t index=0) const;

  size_t get_npulse() const   {return _pulse_v.size();};

  void set_ped_mean(double v) { _ped_mean = v; };

  void set_ped_rms(double v)  { _ped_rms = v; };

  double ped_mean() const     {return _ped_mean; };

  double ped_rms() const      {return _ped_rms;  };

protected:

  std::vector<pulse_param> _pulse_v;

  pulse_param _pulse;

  double _ped_mean, _ped_rms;

protected:

  bool check_index(const std::vector<uint16_t> *wf, size_t begin, size_t end) const;

  bool integral   (const std::vector<uint16_t> *wf, double &result, size_t begin=0, size_t end=0) const;

  bool derivative (const std::vector<uint16_t> *wf, std::vector<int32_t> &diff, size_t begin=0, size_t end=0) const;

  size_t max(const std::vector<uint16_t> *wf, double &result, size_t begin=0, size_t end=0) const;

  size_t min(const std::vector<uint16_t> *wf, double &result, size_t begin=0, size_t end=0) const;

};


#endif

