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
        /*
        for(int j = 0; j < way_assoc; j++){
            RRIPCacheData temp_data;
            temp_data.RRIP = 7;
            temp_data.index = 0;
            temp_data.tag = UINT64_MAX; // default value to mark it as invalid
            temp.push_back(temp_data);
        }
        */
        RRIP_vector.push_back(temp);
    }

    index_bit_count = p.index_bit_count;
    index_bit_mask = (1 << index_bit_count) - 1;
}

// this is called when an entry is evicted
void
Hawkeye::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
{
    //std::cout << "Invalidating" << std::endl;
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
            //RRIP_vector[index][i].tag = UINT64_MAX;
            //RRIP_vector[index][i].RRIP = 7;
            //break;
            // pop the element
            RRIP_vector[index].erase(RRIP_vector[index].begin() + i);
            break;
        }
    }
    //std::cout << "Invalidated" << std::endl;
}

void Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("You are not supposed to use this method in Hawkeye replacement policy");
}

// this is called when an entry is touched
void
Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
    //std::cout << "Init step" << std::endl;
    // get the PC from the packet
    if(pkt->req->hasPC() == false){
        ////std::cout << "No PC found in the packet" << std::endl;
        return;
    }
    //std::cout << "PC found in the packet" << std::endl;
    Addr pc = pkt->req->getPC();
    // get the first 13 bits of the PC
    int pc_index = pc & 0x1FFF;
    // get the address of the request
    if(pkt->req->hasPaddr() == false){
        ////std::cout << "No address found in the packet" << std::endl;
        return;
    }
    //std::cout << "Address found in the packet" << std::endl;
    Addr addr = pkt->req->getPaddr();
    // get the index bits
    int index_bits = (addr >> 3) & index_bit_mask;
    // get the tag bits
    uint64_t tag_bits = addr >> (3 + index_bit_count);

    //std::cout << "Init step finished" << std::endl;
    // I hope I know what I am doing
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    hawkeye_data->index = index_bits;
    hawkeye_data->tag = tag_bits;
    // We need these to invalidate the correct entry in RRIP vector
    //std::cout << "Wrote index and tag" << std::endl;

    ////std::cout << "before element get" << std::endl;

    bool prediction = true;

    //std::cout << "Before searching for element" << std::endl;
    // get the correct element
    bool found = false;
    RRIPCacheData* element = nullptr;
    for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
        if(RRIP_vector[index_bits][i].tag == tag_bits){
            element = &RRIP_vector[index_bits][i];
            found = true;
        }
    }
    //std::cout << "Searched for element" << std::endl;
    if(!found){
        //std::cout << "Element not found" << std::endl;
        // we need to initialize it!
        // and also we need to invalidate the entry from RRIP vector, too
        // Like we are managing our own cache
        // I assume that we don't need to find a victim, there is available space
        /*
        for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
            if(RRIP_vector[index_bits][i].tag == UINT64_MAX){
                element = &RRIP_vector[index_bits][i];
                element->tag = tag_bits;
                element->RRIP = 7;
                element->index = index_bits; // TODO:
                break;
            }
        }
        */
        RRIPCacheData temp_data;
        temp_data.RRIP = 7;
        temp_data.index = index_bits;
        temp_data.tag = tag_bits;
        RRIP_vector[index_bits].push_back(temp_data);
        element = &RRIP_vector[index_bits][RRIP_vector[index_bits].size() - 1];
        prediction = false;
        if(element == nullptr){
            panic("Element is still nullptr");
        }
    }
    else{
        //std::cout << "Element found" << std::endl;
        // write the prediction here!

    }

    if(prediction == false){ // Averse
        // Update RRIP vector
        ////std::cout << "prediction is false" << std::endl;
        element->RRIP = 7;
    }
    else{
        // This is a hit, we know
        // Update RRIP vector
        //std::cout << "Updating RRIP vector" << std::endl;
        for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
            ////std::cout << "index: " << i << std::endl;
            if(RRIP_vector[index_bits][i].RRIP < 7){
                RRIP_vector[index_bits][i].RRIP++; // this is not a problem for invalid entries, they will be initialized later anyway
            }
        }
        //std::cout << "Updated RRIP vector" << std::endl;
        element->RRIP = 0;
        
    }

    //std::cout << "Finished touch" << std::endl;

}

void Hawkeye::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("You are not supposed to use this method in Hawkeye replacement policy");
}

// this is called when a new entry is inserted
void
Hawkeye::reset(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
    //std::cout << "Reset Init step" << std::endl;
    // Unprioritize replacement data victimization
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->valid = true;

    // get the PC from the packet
    if(pkt->req->hasPC() == false){
        ////std::cout << "No PC found in the packet" << std::endl;
        return;
    }
    //std::cout << "Reset PC found in the packet" << std::endl;
    Addr pc = pkt->req->getPC();
    // get the first 13 bits of the PC
    int pc_index = pc & 0x1FFF;
    // get the address of the request
    if(pkt->req->hasPaddr() == false){
        ////std::cout << "No address found in the packet" << std::endl;
        return;
    }
    //std::cout << "Reset Address found in the packet" << std::endl;
    Addr addr = pkt->req->getPaddr();
    // get the index bits
    int index_bits = (addr >> 3) & index_bit_mask;
    // get the tag bits
    uint64_t tag_bits = addr >> (3 + index_bit_count);

    //std::cout << "Reset Init step finished" << std::endl;
    // I hope I know what I am doing
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    hawkeye_data->index = index_bits;
    hawkeye_data->tag = tag_bits;
    // We need these to invalidate the correct entry in RRIP vector
    //std::cout << "Reset Wrote index and tag" << std::endl;

    //std::cout << "Reset Before searching for element" << std::endl;
    // get the correct element
    // We will not find the element, this is reset
    RRIPCacheData* element = nullptr;
    /*
    for(int i = 0; RRIP_vector[index_bits].size(); i++){
        if(RRIP_vector[index_bits][i].tag == UINT64_MAX){
            element = &RRIP_vector[index_bits][i];
            element->tag = tag_bits;
            element->RRIP = 7;
            element->index = index_bits; // TODO:
            break;
        }
    }
    */
    RRIPCacheData temp_data;
    temp_data.RRIP = 7;
    temp_data.index = index_bits;
    temp_data.tag = tag_bits;
    RRIP_vector[index_bits].push_back(temp_data);
    element = &RRIP_vector[index_bits][RRIP_vector[index_bits].size() - 1];

    // This is the first appearance, the prediction will be averse
    // Wrong, do prediction here
    bool prediction = false;

    if(element == nullptr){
        panic("Reset Element is still nullptr");
    }
    element->RRIP = 7; // Unnecessary, but for the sake of clarity
    //std::cout << "Reset Finished" << std::endl;
}

ReplaceableEntry*
Hawkeye::getVictim(const ReplacementCandidates& candidates) const
{
    //std::cout << "Getting victim" << std::endl;
    // Visit all candidates to search for an invalid entry. If one is found,
    // its eviction is certain
    for (const auto& candidate : candidates) {
        if (!std::static_pointer_cast<HawkeyeReplData>(
                    candidate->replacementData)->valid) {
            ReplaceableEntry* victim = candidate;
            return victim;
        }
    }
    //std::cout << "after the loop" << std::endl;

    int max_rrip = 0;
    int max_index = 0;
    int set_index = candidates[0]->getSet();
    //std::cout << "just second loop" << std::endl;
    //std::cout << "set: " << set_index << std::endl;
    //std::cout << "bos" << std::endl;
    for(int i = 0; i < RRIP_vector[set_index].size(); i++){
        int rrip = RRIP_vector[set_index][i].RRIP;
        if(rrip > max_rrip){
            max_rrip = rrip;
            max_index = i;
        }
    }
    //std::cout << "after second loop " << max_index << std::endl;
    

    // return the entry with the highest RRIP value
    return candidates[max_index];
    //std::cout << "Got victim" << std::endl;
}

std::shared_ptr<ReplacementData>
Hawkeye::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new HawkeyeReplData());
}

} // namespace replacement_policy
} // namespace gem5