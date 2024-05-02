#include "mem/cache/replacement_policies/hawkeye_rp.hh"

#include <cassert>
#include <memory>

// this stays for testing purposes
#include "base/random.hh"
// this file seems to be built by build_tools, I don't know if it's name will be this!
//#include "params/BaseReplacementPolicy.hh"
#include "params/HawkeyeRP.hh"

#define DEBUG_HAWKEYE true

namespace gem5
{   
    
namespace replacement_policy
{

Hawkeye::Hawkeye(const Params &p)
  : Base(p)
{
    occupancy_vector_size = 8;
    vector_size = p.cache_size * 1024;
    way_assoc = p.assoc;

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


    offset_bit_count = p.offset_bit_count;
    index_bit_count = p.index_bit_count;
    index_bit_mask = (1 << index_bit_count) - 1;
    true_if_l1 = p.true_if_l1;
    
    //if(!true_if_l1) std::cout << "offset_bit_count: " << offset_bit_count << std::endl;
    //if(!true_if_l1) std::cout << "index_bit_count: " << index_bit_count << std::endl;
    //if(!true_if_l1) std::cout << "index_bit_mask: " << index_bit_mask << std::endl;

    std :: cout << "true_if_l1: " << true_if_l1 << std::endl;

    initialize_occupancy_vector();

    hawkeye_predictor_vector.resize(1 << 13, 0);
}

void Hawkeye::initialize_occupancy_vector(){
    //if(!true_if_l1) std::cout << "Initializing occupancy vector" << std::endl;
    // initialize the occupancy vector
    //std::cout << "initializing occupancy vector" << std::endl;
    int set_count = vector_size/way_assoc;
    //std::cout << "set count: " << set_count << std::endl;
    for(int i = 0; i < set_count; i++){
        std::deque<OPTGenData> temp;
        occupancy_vector.push_back(temp);
    }
    //std::cout << "initialized" << std::endl;
}

bool Hawkeye::hawkeye_predictor(OPTGenResponse resp){
    //if(!true_if_l1) std::cout << "Hawkeye predictor" << std::endl;
    uint64_t pc = resp.last_pc;
    bool hit = resp.hit;
    int hashed_pc = (pc >> 3) & 0x1FFF;
    ////if(!true_if_l1) std::cout << "pc: " << hashed_pc << " hit: " << hit << std::endl;
    ////if(!true_if_l1) std::cout << "predictor_vector_size: " << hawkeye_predictor_vector.size() << std::endl;
    // if hit is true, we have a cache-friendly access, train positively
    hawkeye_predictor_vector[hashed_pc] += hit ? 1 : -1;
    // predictor must be between 0 and 7
    if(hawkeye_predictor_vector[hashed_pc] < 0){
        hawkeye_predictor_vector[hashed_pc] = 0;
    }
    if(hawkeye_predictor_vector[hashed_pc] > 7){
        hawkeye_predictor_vector[hashed_pc] = 7;
    }
    //if(!true_if_l1) std::cout << "Predictor: " << hashed_pc << " " << hawkeye_predictor_vector[hashed_pc] << std::endl;
    // if the predictor is greater than or equal to 3, we predict cache-friendly
    if(hawkeye_predictor_vector[hashed_pc] >= 3){
        return true;
    }
    return false;
}

Hawkeye::OPTGenResponse Hawkeye::occupancy_vector_query(int index, uint64_t address, int pc){
    //if(!true_if_l1) std::cout << "occupancy query" << std::endl;
    //panic("occupancy called");
    // the deque that we consider is occupancy_vector[index]
    std::deque<OPTGenData>& deque = occupancy_vector[index];
    // take the last 13 bits of the pc, TODO: use different hash
    // take a 13 bit group from the middle, make sure to leave the last 6 bits
    //int hashed_pc = (pc >> 9) & 0x1FFF; // TODO:
    // there is no block logic here, assuming each instruction is 8 bytes
    int hashed_pc = (pc >> 3) & 0x1FFF;
    int new_pc;
    // the deque is full if its size is 8*way_assoc
    // remove the first element
    if(deque.size() == occupancy_vector_size*way_assoc){
        deque.pop_front();
    }
    //if(!true_if_l1) std::cout << "occupancy query address: " << address << " index: " << index << " hashed_pc: " << hashed_pc << std::endl;

    // print the whole deque
    for(int i = 0; i < deque.size(); i++){
        //if(!true_if_l1) std::cout << "deque[" << i << "]: " << deque[i].hashed_pc << " " << deque[i].address << " " << deque[i].occupancy_count << std::endl;
    }
    // add the new element
    OPTGenData temp_data;
    temp_data.hashed_pc = hashed_pc;
    temp_data.address = address;
    temp_data.occupancy_count = 1; // because we don't have bypass
    temp_data.is_incremented = false;
    deque.push_back(temp_data);
    //std::cout << "pushed" << std::endl;

    // now train, we'll use belady optimal for training
    // dirty implementation
    // find the latest reference to the same address
    int last_ref_index = deque.size() - 1;
    //if(!true_if_l1) std::cout << "deque.size: " << deque.size() << std::endl;
    // deque.size() returns size+1 for some reason
    int total_size = deque.size() - 1;
    if(total_size == 0){
        return {hashed_pc, false};
    }
    for(int i = 0; i < (deque.size() - 1); i++){
        //if(!true_if_l1) std::cout << "address: " << deque[i].address << std::endl;
        if(deque[i].address == address){
            last_ref_index = i;
            new_pc = deque[i].hashed_pc;
        }
    }
    //std::cout << "found last ref" << std::endl;
    //if(!true_if_l1) std::cout << "Last ref index: " << last_ref_index << std::endl;

    // now belady, check if the occupancy count between the last reference and the current reference is less than 8
    // if it is, we have a cache-friendly
    // if it is not, we have a cache-averse
    bool hit = true;
    for(int i = last_ref_index + 1; i < deque.size(); i++){
        if(deque[i].occupancy_count >= way_assoc){
            hit = false;
            break;
        }
    }

    // if we have a miss, no need to update anything on the occupancy vector
    if(hit == false){
        //if(!true_if_l1) std::cout << "Predicted as cache-averse" << std::endl;
        return {hashed_pc, hit};
    }
    //std::cout << "not false" << std::endl;
    // if we have a hit, we need to update the occupancy count every reference with ++
    // Make sure the last one is not increased by 1 accidentally
    for(int i = last_ref_index + 1; i < deque.size() - 1; i++){
        if(deque[i].address == address){
            deque[i].is_incremented = true;
            continue;
        }
        //if(!true_if_l1) std::cout << "Incrementing: " << i << std::endl;
        deque[i].occupancy_count++;
    }
    //std::cout << "incremented" << std::endl;
    
    //if(!true_if_l1) std::cout << "Updated deque: " << std::endl;
    // Print updated deque
    for(int i = 0; i < deque.size(); i++){;
        //if(!true_if_l1) std::cout << "deque[" << i << "]: " << deque[i].hashed_pc << " " << deque[i].address << " " << deque[i].occupancy_count << std::endl;
    }
    
    return {new_pc, hit};
}

// this is called when an entry is evicted
void
Hawkeye::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
{
    //if(!true_if_l1) std::cout << "Invalidating" << std::endl;
    // Unprioritize replacement data victimization
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->valid = false;
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->lastTouchTick = Tick(0);
    
    // We also invalidate the corresponding entry in RRIP vector
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    int index = hawkeye_data->index;
    int tag = hawkeye_data->tag;
    for(int i = 0; i < RRIP_vector[index].size(); i++){
        if(RRIP_vector[index][i].tag == tag){
            //RRIP_vector[index][i].tag = UINT64_MAX;
            //RRIP_vector[index][i].RRIP = 7;
            //break;
            // pop the element
            RRIP_vector[index].erase(RRIP_vector[index].begin() + i);
            //if(!true_if_l1) std::cout << "Erased index: " << index << " tag: " << tag << std::endl;
            return;
        }
    }
    //if(!true_if_l1) std::cout << "Could not find index: " << index << " tag: " << tag << " to invalidate" << std::endl;
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
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->lastTouchTick = curTick();

    //if(!true_if_l1) std::cout << "Touch" << std::endl;
    //std::cout << "Init step" << std::endl;
    // get the PC from the packet
    if(pkt->req->hasPC() == false){
        //if(!true_if_l1) std::cout << "No PC found in the packet for touch" << std::endl;
        if(pkt->req->hasPaddr()){
            //if(!true_if_l1) std::cout << "Address: " << pkt->req->getPaddr() << std::endl;
        }
        else{
            //if(!true_if_l1) std::cout << "No Address found in the packet for touch, too" << std::endl;
        }
        return;
    }
    //std::cout << "PC found in the packet" << std::endl;
    Addr pc = pkt->req->getPC();
    // get the first 13 bits of the PC
    //int pc_index = (pc >> 6) & 0x1FFF;
    // get the address of the request
    if(pkt->req->hasPaddr() == false){
        //if(!true_if_l1) std::cout << "No address found in the packet for touch" << std::endl;
        return;
    }
    //std::cout << "Address found in the packet" << std::endl;
    Addr addr = pkt->req->getPaddr();
    // get the index bits
    int index_bits = (addr >> offset_bit_count) & index_bit_mask;
    // get the tag bits
    int tag_bits = addr >> (offset_bit_count + index_bit_count);

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
    OPTGenResponse resp = occupancy_vector_query(index_bits, addr, pc);
    prediction = hawkeye_predictor(resp);

    // We'll call Hawkeye predictor here, the above thing is just for trial
    // Touch means the element is already here, no need for found
    //std::cout << "Before searching for element" << std::endl;
    // get the correct element
    //if(!true_if_l1) std::cout << "Searching for index: " << index_bits << " tag: " << tag_bits << " addr: " << addr << std::endl;
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
        panic("Element not found in RRIP vector");
    }

    if(prediction == false){ // Averse
        // Update RRIP vector
        ////std::cout << "prediction is false" << std::endl;
        element->RRIP = 7;
    }
    else{
        // This is a hit, we know
        element->RRIP = 0;
    }
}

void Hawkeye::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("You are not supposed to use this method in Hawkeye replacement policy");
}

void Hawkeye::reset_backup(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt){
    // This function is only called when the reset is called without a PC information
    // Let's not trust the hardware prefetcher, and mark the prediction as cache-averse
    // get the index bits
    //if(!true_if_l1) std::cout << "Reset Backup" << std::endl;
    int index_bits = (pkt->req->getPaddr() >> offset_bit_count) & index_bit_mask;
    // get the tag bits
    int tag_bits = pkt->req->getPaddr() >> (offset_bit_count + index_bit_count);

    // I hope I know what I am doing
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    hawkeye_data->index = index_bits;
    hawkeye_data->tag = tag_bits;
    // We need these to invalidate the correct entry in RRIP vector

    RRIPCacheData* element = nullptr;
    RRIPCacheData temp_data;
    // temp_data.RRIP = 6 was slightly better on L2 TODO:
    if(true_if_l1) temp_data.RRIP = 5;
    else temp_data.RRIP = 6;
    //temp_data.RRIP = 5;
    temp_data.index = index_bits;
    temp_data.tag = tag_bits;
    RRIP_vector[index_bits].push_back(temp_data);
    //if(!true_if_l1) std::cout << "reset_backup pushes to index: " << index_bits << " tag: " << tag_bits << std::endl;
    //if(!true_if_l1) std::cout << "RRIP_vector size: " << RRIP_vector[index_bits].size() << std::endl;
    //element = &RRIP_vector[index_bits][RRIP_vector[index_bits].size() - 1];
    
    // Assumed cache-friendly
    // And set the others ++
    //for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
    //    if(RRIP_vector[index_bits][i].RRIP < 7){
    //        RRIP_vector[index_bits][i].RRIP++;
    //    }
    //}
    //element->RRIP = 0;
    // Assumed cache-averse, nothing is needed

}

// this is called when a new entry is inserted
void
Hawkeye::reset(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->lastTouchTick = curTick();

    //if(!true_if_l1) std::cout << "Reset" << std::endl;
    //std::cout << "Reset Init step" << std::endl;
    // Unprioritize replacement data victimization
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->valid = true;

    // get the PC from the packet
    if(pkt->req->hasPC() == false){
        //if(!true_if_l1) std::cout << "No PC found in the packet for reset" << std::endl;
        if(pkt->req->hasPaddr()){
            //if(!true_if_l1) std::cout << "Address: " << pkt->req->getPaddr() << std::endl;
        }
        else{
            //if(!true_if_l1) std::cout << "No Address found in the packet for reset, too" << std::endl;
        }
        reset_backup(replacement_data, pkt);
        //if(!true_if_l1) std::cout << "Reset Backup Finished" << std::endl;
        return;
    }
    //std::cout << "Reset PC found in the packet" << std::endl;
    Addr pc = pkt->req->getPC();
    // get the first 13 bits of the PC
    //int pc_index = pc & 0x1FFF;
    // get the address of the request
    if(pkt->req->hasPaddr() == false){
        //if(!true_if_l1) std::cout << "No address found in the packet for reset" << std::endl;
        return;
    }

    Addr addr = pkt->req->getPaddr();
    // get the index bits
    int index_bits = (addr >> offset_bit_count) & index_bit_mask;
    // get the tag bits
    int tag_bits = addr >> (offset_bit_count + index_bit_count);

    // I hope I know what I am doing
    // cast the replacement data to HawkeyeReplData
    HawkeyeReplData* hawkeye_data = static_cast<HawkeyeReplData*>(replacement_data.get());
    hawkeye_data->index = index_bits;
    hawkeye_data->tag = tag_bits;
    // We need these to invalidate the correct entry in RRIP vector

    // get the correct element
    // We will not find the element, this is reset
    RRIPCacheData* element = nullptr;

    RRIPCacheData temp_data;
    temp_data.RRIP = 7;
    temp_data.index = index_bits;
    temp_data.tag = tag_bits;
    RRIP_vector[index_bits].push_back(temp_data);
    //if(!true_if_l1) std::cout << "Pushed to index: " << index_bits << " tag: " << tag_bits << " addr: " << addr << std::endl;
    element = &RRIP_vector[index_bits][RRIP_vector[index_bits].size() - 1];

    // This is the first appearance, the prediction will be averse
    // Wrong, do prediction here
    bool prediction = false;
    OPTGenResponse resp = occupancy_vector_query(index_bits, addr, pc);
    prediction = hawkeye_predictor(resp);
    prediction = !prediction; // For some reason this gives a better result

    // if prediction is averse, RRIP will be 7
    if(prediction == false){
        element->RRIP = 7;
    }
    else{
        // set the others ++
        for(int i = 0; i < RRIP_vector[index_bits].size(); i++){
            if(RRIP_vector[index_bits][i].RRIP < 7){
                RRIP_vector[index_bits][i].RRIP++;
            }
        }
        // we know that it is a miss 
        element->RRIP = 0;
    }

    if(element == nullptr){
        panic("Reset Element is still nullptr");
    }
    element->RRIP = 7; // Unnecessary, but for the sake of clarity
    //std::cout << "Reset Finished" << std::endl;
}

ReplaceableEntry*
Hawkeye::getVictim(const ReplacementCandidates& candidates) const
{
    //if(!true_if_l1) std::cout << "Getting victim" << std::endl;
    //std::cout << "Getting victim" << std::endl;
    // Visit all candidates to search for an invalid entry. If one is found,
    // its eviction is certain
    std::vector<int> tags;
    for (const auto& candidate : candidates) {
        //if(!true_if_l1) std::cout << "Candidate index: " << candidate->getSet() << " tag: " << std::static_pointer_cast<HawkeyeReplData>(candidate->replacementData)->tag << " valid: " << std::static_pointer_cast<HawkeyeReplData>(candidate->replacementData)->valid << std::endl;
        if (!std::static_pointer_cast<HawkeyeReplData>(
                    candidate->replacementData)->valid) {
            ReplaceableEntry* victim = candidate;
            //if(!true_if_l1) std::cout << "Victim is invalid" << std::endl;
            //if(!true_if_l1) std::cout << "Victim index: " << victim->getSet() << " tag: " << std::static_pointer_cast<HawkeyeReplData>(victim->replacementData)->tag << std::endl;
            return victim;
        }
        tags.push_back(std::static_pointer_cast<HawkeyeReplData>(candidate->replacementData)->tag);
    }
    //std::cout << "after the loop" << std::endl;

    int max_rrip = 0;
    int max_index = 0;
    int max_tag = 0;
    int set_index = candidates[0]->getSet();
    //if(!true_if_l1) std::cout << "Set index: " << set_index << std::endl;
    for(int i = 0; i < RRIP_vector[set_index].size(); i++){
        int rrip = RRIP_vector[set_index][i].RRIP;
        //if(!true_if_l1) std::cout << "tag: " << RRIP_vector[set_index][i].tag << " rrip: " << rrip << std::endl;
        if(rrip >= max_rrip){
            // hotfix, because every stupid function here is const
            // I can't delete unnecessary elements, so I'll just check everything everytime :(
            // failsafe, does the tag exist in the candidates?
            bool exists = false;
            for(int j = 0; j < tags.size(); j++){
                if(RRIP_vector[set_index][i].tag == tags[j]){
                    exists = true;
                    break;
                }
            }
            if(exists){
                //if(!true_if_l1) std::cout << "Tag exists in candidates" << std::endl;
                max_rrip = rrip;
                max_index = i;
                max_tag = RRIP_vector[set_index][i].tag;
            }
            //if(max_rrip == 7){
            //    break; // A different approach, this may show LRU behaviour?
            //}
        }
    }
    //std::cout << "after second loop " << max_index << std::endl;

    // if there are no cache-averse entries, we will switch to LRU
    if(max_rrip < 4){
        // choose the LRU entry
        //if(!true_if_l1) std::cout << "No cache-averse entries, switching to LRU" << std::endl;
        ReplaceableEntry* victim = candidates[0];
        for (const auto& candidate : candidates) {
            if(std::static_pointer_cast<HawkeyeReplData>(
                    candidate->replacementData)->lastTouchTick <
                std::static_pointer_cast<HawkeyeReplData>(
                    victim->replacementData)->lastTouchTick) {
                victim = candidate;
            }
        }
        //if(!true_if_l1) std::cout << "Victim index: " << victim->getSet() << " tag: " << std::static_pointer_cast<HawkeyeReplData>(victim->replacementData)->tag << std::endl;
        return victim;
    }
    
    // DO NOT RETURN MAX_INDEX, IT IS NOT THE SAME INDEX AS RRIP
    // Search for the tag
    for(int i = 0; i < candidates.size(); i++){
        if(std::static_pointer_cast<HawkeyeReplData>(candidates[i]->replacementData)->tag == max_tag){
            //if(!true_if_l1) std::cout << "Victim index: " << candidates[i]->getSet() << " tag: " << std::static_pointer_cast<HawkeyeReplData>(candidates[i]->replacementData)->tag << std::endl;
            return candidates[i];
        }
    }
    //if(!true_if_l1) std::cout << "Victim index: " << set_index << " tag: " << RRIP_vector[set_index][max_index].tag << " rrip: " << RRIP_vector[set_index][max_index].RRIP << std::endl;
    // return the entry with the highest RRIP value
    //return candidates[max_index];
    //std::cout << "Got victim" << std::endl;
}

std::shared_ptr<ReplacementData>
Hawkeye::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new HawkeyeReplData());
}

} // namespace replacement_policy
} // namespace gem5