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
}

// this is called when an entry is evicted
void
Hawkeye::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
{
    // Unprioritize replacement data victimization
    std::static_pointer_cast<HawkeyeReplData>(
        replacement_data)->valid = false;
}

// this is called when an entry is touched
void
Hawkeye::touch(const std::shared_ptr<ReplacementData>& replacement_data, const PacketPtr pkt)
{
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
    // There must be at least one replacement candidate
    assert(candidates.size() > 0);

    /*
    // Choose one candidate at random
    ReplaceableEntry* victim = candidates[random_mt.random<unsigned>(0,
                                    candidates.size() - 1)];

    // Visit all candidates to search for an invalid entry. If one is found,
    // its eviction is prioritized
    for (const auto& candidate : candidates) {
        if (!std::static_pointer_cast<HawkeyeReplData>(
                    candidate->replacementData)->valid) {
            victim = candidate;
            break;
        }
    }
    */
    ReplaceableEntry* victim = candidates[0];

    return victim;
}

std::shared_ptr<ReplacementData>
Hawkeye::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new HawkeyeReplData());
}

// Implement getPC() and setPC() methods
PCType
Hawkeye::HawkeyeReplData::getPC()
{
    return pc;
}

void
Hawkeye::HawkeyeReplData::setPC(PacketPtr pkt)
{
    // Need to hash the PC 
    // Don't know how good it will work
    // Will take the first 13 bits of the PC
    pc = pkt->req->getPC() & 0x1FFF;
}

} // namespace replacement_policy
} // namespace gem5
