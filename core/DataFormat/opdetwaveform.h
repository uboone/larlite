/**
 * raw/OpDetWaveform.h
 *
 * Raw signals from the photon detectors.
 * Waveform (adcs in time bins), a channel number, and a time stamp.
 *
 */


#ifndef LARLITE_OPDETWAVEFORM_H
#define LARLITE_OPDETWAVEFORM_H

#include "data_base.h"
#include <vector>
#include <functional> // so we can redefine less<> below
#include <limits>
#include <iosfwd>

namespace larlite {
  
  // Define the types used
  typedef short              ADC_Count_t;
  typedef unsigned int       Channel_t;
  typedef double             TimeStamp_t;  ///< us since 1970, based on TimeService

  class opdetwaveform : public std::vector< larlite::ADC_Count_t >,
			public data_base {
    
  public:
    
    // Simple constructors/destructors. 
    // Just in case the user forgets to supply the default channel, use
    // a garbage value to indicate that there's a problem.
    // To save on memory reallocations, offer an option to specify the
    // the initial memory allocation of the channel vector.
    
    //opdetwaveform() : data_base(data::kOpDetWaveform){clear_data();}

    //    optdetwaveform( larlite::TimeStamp_t time = std::numeric_limits<larlite::TimeStamp_t>::max(), 
    //		    larlite::Channel_t   chan = std::numeric_limits<larlite::Channel_t>::max(), 
    //		    size_t len = 0 );
 
    optdetwaveform( TimeStamp_t time = std::numeric_limits<TimeStamp_t>::max(), 
		    Channel_t   chan = std::numeric_limits<Channel_t>::max(), 
		    size_type len = 0 ) : fChannel(chan), 
					  fTimeStamp(time)
    {
      this->reserve(len);
    }

    /*
    optdetwaveform( TimeStamp_t time,
		    Channel_t   chan,
		    std::vector< uint16_t > rhs )
      : fChannel(chan)
      , fTimeStamp(time)
    {
      this->reserve(rhs.size());
      for (unsigned int i =0; i < rhs.size(); i++)
	this->push_back(rhs[i]);
    };
    */

    virtual ~opdetwaveform(){}
    
    // Functions included for backwards compatability with previous data types
    std::vector<ADC_Count_t>& Waveform()         { return *this;  }
    
    //kaleko doesn't understand this, so he assumes it is malware or something
    //static_assert(sizeof(unsigned long long)==8,"unsigned long long is not 8 bytes");
    
    Channel_t   ChannelNumber() const            { return fChannel; }
    TimeStamp_t TimeStamp() const                { return fTimeStamp; }
    void        SetChannelNumber(Channel_t chan) { fChannel = chan; }
    void        SetTimeStamp(TimeStamp_t time)   { fTimeStamp = time; }
    
    
  private:
    
    Channel_t   fChannel;
    TimeStamp_t fTimeStamp;
    
  };
  
  bool operator<( const opdetwaveform& lhs, const opdetwaveform& rhs )
  {
    // Sort by channel, then time
    if ( lhs.ChannelNumber()  < rhs.ChannelNumber() ) return true;
    if ( rhs.ChannelNumber()  > rhs.ChannelNumber() ) return false;
    
    return ( lhs.TimeStamp() < rhs.TimeStamp() );
  }

  // For no extra charge, include how to sort ChannelData*, just in
  // case we want (for example) a std::set<ChannelData*>.
  /*
template <> 
    class less<larlite::opdetwaveform*>
  {
  public:
    bool operator()( const larlite::opdetwaveform* lhs, const larlite::opdetwaveform* rhs )
    {
      return (*lhs) < (*rhs);
    }
    };*/
}
#endif

