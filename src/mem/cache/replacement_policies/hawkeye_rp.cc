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
            temp_data.tag = UINT64_MAX; // default value to mark it as invalid
            temp_data.first_touch = true;
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
    
    // We also invalidate the corresponding entry in RRIP vector
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    int index = hawkeye_data->index;
    uint64_t tag = hawkeye_data->tag;
    for(int i = 0; i < RRIP_vector[index].size(); i++){
        if(RRIP_vector[index][i].tag == tag){
            RRIP_vector[index][i].tag = UINT64_MAX;
            RRIP_vector[index][i].RRIP = 7;
            RRIP_vector[index][i].first_touch = true;
            break;
        }
    }

}

void Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("You are not supposed to use this method in Hawkeye replacement policy");
}

// this is called when an entry is touched
void
Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
    std::cout << "Init step" << std::endl;
    // get the PC from the packet
    if(pkt->req->hasPC() == false){
        //std::cout << "No PC found in the packet" << std::endl;
        return;
    }
    std::cout << "PC found in the packet" << std::endl;
    Addr pc = pkt->req->getPC();
    // get the first 13 bits of the PC
    int pc_index = pc & 0x1FFF;
    // get the address of the request
    if(pkt->req->hasPaddr() == false){
        //std::cout << "No address found in the packet" << std::endl;
        return;
    }
    std::cout << "Address found in the packet" << std::endl;
    Addr addr = pkt->req->getPaddr();
    // get the index bits
    int index_bits = (addr >> 3) & index_bit_mask;
    // get the tag bits
    uint64_t tag_bits = addr >> (3 + index_bit_count);

    std::cout << "Init step finished" << std::endl;
    // I hope I know what I am doing
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    hawkeye_data->index = index_bits;
    hawkeye_data->tag = tag_bits;
    // We need these to invalidate the correct entry in RRIP vector
    std::cout << "Wrote index and tag" << std::endl;

    //std::cout << "before element get" << std::endl;

    bool prediction = true;

    std::cout << "Before searching for element" << std::endl;
    // get the correct element
    bool found = false;
    RRIPCacheData* element = nullptr;
    for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
        if(RRIP_vector[index_bits][i].tag == tag_bits){
            element = &RRIP_vector[index_bits][i];
            found = true;
        }
    }
    std::cout << "Searched for element" << std::endl;
    if(!found){
        std::cout << "Element not found" << std::endl;
        // we need to initialize it!
        // and also we need to invalidate the entry from RRIP vector, too
        // Like we are managing our own cache
        // I assume that we don't need to find a victim, there is available space
        for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
            if(RRIP_vector[index_bits][i].tag == UINT64_MAX){
                element = &RRIP_vector[index_bits][i];
                element->tag = tag_bits;
                element->first_touch = true;
                element->RRIP = 7;
                element->index = index_bits; // TODO:
                break;
            }
        }
        prediction = false;
        if(element == nullptr){
            panic("Element is still nullptr");
        }
    }
    else{
        std::cout << "Element found" << std::endl;
        // write the prediction here!

    }



    if(prediction == false){ // Averse
        // Update RRIP vector
        //std::cout << "prediction is false" << std::endl;
        element->RRIP = 7;
    }
    else{
        //std::cout << "prediction is true" << std::endl;
        if(element->first_touch){ // if hit
            // Update RRIP vector
            std::cout << "Updating element->RRIP to 0" << std::endl;
            element->RRIP = 0;
        }
        else{ // if miss
            // Update RRIP vector
            std::cout << "Updating RRIP vector" << std::endl;
            for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
                //std::cout << "index: " << i << std::endl;
                if(RRIP_vector[index_bits][i].RRIP < 7){
                    RRIP_vector[index_bits][i].RRIP++; // this is not a problem for invalid entries, they will be initialized later anyway
                }
            }
            std::cout << "Updated RRIP vector" << std::endl;
            element->RRIP = 0;
        }
    }

    element->first_touch = false;
    std::cout << "Finished touch" << std::endl;

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
    
    // just call the touch method
    touch(replacement_data, pkt);
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