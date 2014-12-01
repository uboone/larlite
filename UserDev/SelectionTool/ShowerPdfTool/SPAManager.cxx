#ifndef SPAMANAGER_CXX
#define SPAMANAGER_CXX

#include "SPAManager.h"

namespace sptool {

  SPAManager::SPAManager() 
    : _algo   (nullptr)
    , _filter (nullptr)
  {}

  std::vector<std::pair<float,SPMatch_t> > SPAManager::Process(const SPAData &data)
  {
    if(!_algo) 

      throw SPAException("Algorithm is not set!");

    // Result container to be returned
    std::vector<std::pair<float,SPMatch_t> > result;

    // local data container
    SPAData input(data);
    input._showers.clear();

    //
    // First, find out valid showers among input by running SPAFilter
    //
    std::vector<size_t> valid_index_v;
    valid_index_v.reserve(data._showers.size());
    for(size_t i=0; i<data._showers.size(); ++i) {
      input._showers.resize(1);
      input._showers[0] = data._showers[i];
      if(_filter && _filter->Select(input))  valid_index_v.push_back(i);
      else if(!_filter) valid_index_v.push_back(i);
    
    }

    // Return empty result if # of valid showers is less than # of showers required by SPAlgo
    if(valid_index_v.size() < _algo->_nshowers) return result;

    //
    // Loop over valid shower indexies and call SPAlgo
    //
    std::multimap<float,SPMatch_t> score_map;
    std::vector<bool> comb_index_v(valid_index_v.size());
    std::fill(comb_index_v.begin() + _algo->_nshowers, comb_index_v.end(), true);

    do {

      std::vector<size_t> index_v;
      index_v.reserve(_algo->_nshowers);

      input._showers.clear();
      input._showers.reserve(_algo->_nshowers);

      for (int i = 0; i < comb_index_v.size(); ++i)
	
	if (!comb_index_v[i]) {
	  input._showers.push_back(data._showers[valid_index_v[i]]);
	  index_v.push_back(valid_index_v[i]);
	}

      score_map.insert(std::make_pair(_algo->Select(input),index_v));

    } while (std::next_permutation(comb_index_v.begin(), comb_index_v.end()));

    //
    // Now return result. Do not allow duplicate usage of the same shower.
    //
    std::set<size_t> used_index;
    for(auto iter = score_map.rbegin();
	iter != score_map.rend();
	++iter) {
      
      if((*iter).first<0) break;

      bool pass = false;
      for(auto const& index : (*iter).second)
	
	if(used_index.find(index) != used_index.end()) {
	  pass = true;
	  break;
	}

      if(pass) continue;
      
      result.push_back(std::make_pair((*iter).first,(*iter).second));
    }

    return result;
  }

}
#endif