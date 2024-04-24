#include "mem/cache/replacement_policies/hawkeye_rp.hh"

#include <cassert>
#include <memory>

// this stays for testing purposes
#include "base/random.hh"
// this file seems to be built by build_tools, I don't know if it's name will be this!
//#include "params/BaseReplacementPolicy.hh"
#include "params/HawkeyeRP.hh"

namespace gem5
{   
    
namespace replacement_policy
{

Hawkeye::Hawkeye(const Params &p)
  : Base(p)
{
    int vector_size = p.cache_size * 1024;
    int way_assoc = p.assoc;

    // start by initializing the RRIP vector
    for(int i = 0; i < vector_size/way_assoc; i++){
        std::vector<RRIPCacheData> temp;
        for(int j = 0; j < way_assoc; j++){
            RRIPCacheData temp_data;
            temp_data.RRIP = 7;
            temp_data.index = 0;
            temp_data.tag = 0;
            temp.push_back(temp_data);
        }
        RRIP_vector.push_back(temp);
    }

    index_bit_count = p.index_bit_count;
    index_bit_mask = (1 << index_bit_count) - 1;
}

// this is called when an entry is evicted
void
Hawkeye::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
{
    // Unprioritize replacement data victimization
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->valid = false;
}

void Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("You are not supposed to use this method in Hawkeye replacement policy");
}

// this is called when an entry is touched
void
Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
    // get the PC from the packet
    Addr pc = pkt->req->getPC();
    // get the first 13 bits of the PC
    int pc_index = pc & 0x1FFF;
    // get the address of the request
    Addr addr = pkt->req->getPaddr();
    // get the index bits
    int index_bits = (addr >> 3) & index_bit_mask;
    // get the tag bits
    uint64_t tag_bits = addr >> (3 + index_bit_count);

    // get the correct element
    RRIPCacheData* element;
    for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
        if(RRIP_vector[index_bits][i].tag == tag_bits){
            element = &RRIP_vector[index_bits][i];
        }
    }


    bool prediction = true;
    // Assume prediction is done
    if(prediction == false){ // Averse
        // Update RRIP vector
        element->RRIP = 7;
    }
    else{
        if(element->first_touch){ // if hit
            // Update RRIP vector
            element->RRIP = 0;
        }
        else{ // if miss
            // Update RRIP vector
            for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
                if(RRIP_vector[index_bits][i].RRIP < 7){
                    RRIP_vector[index_bits][i].RRIP++;
                }
            }
            element->RRIP = 0;
        }
    }

    element->first_touch = false;


}

void Hawkeye::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("You are not supposed to use this method in Hawkeye replacement policy");
}

// this is called when a new entry is inserted
void
Hawkeye::reset(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
    // Unprioritize replacement data victimization
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->valid = true;
}

ReplaceableEntry*
Hawkeye::getVictim(const ReplacementCandidates& candidates) const
{
    // Visit all candidates to search for an invalid entry. If one is found,
    // its eviction is certain
    for (const auto& candidate : candidates) {
        if (!std::static_pointer_cast<HawkeyeReplData>(
                    candidate->replacementData)->valid) {
            ReplaceableEntry* victim = candidate;
            return victim;
        }
    }

    int max_rrip = 0;
    int max_index = 0;
    int set_index = candidates[0]->getSet();
    for(int i = 0; i < candidates.size(); i++){
        int rrip = RRIP_vector[set_index][i].RRIP;
        if(rrip > max_rrip){
            max_rrip = rrip;
            max_index = i;
        }
    }
    

    // return the entry with the highest RRIP value
    return candidates[max_index];
}

std::shared_ptr<ReplacementData>
Hawkeye::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new HawkeyeReplData());
}

} // namespace replacement_policy
} // namespace gem5