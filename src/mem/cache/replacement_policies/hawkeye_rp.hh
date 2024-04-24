/**
 * @file
 * Declaration of Hawkeye replacement policy.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_HAWKEYE_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_HAWKEYE_HH__

// May not need some of these
#include <cstddef>
#include <vector>

#include "base/compiler.hh"
#include "base/sat_counter.hh"
#include "mem/packet.hh"

#include "mem/cache/replacement_policies/base.hh"

namespace gem5
{
  
// This is wrong, but I hope that we won't need params, SimObject uses it in BaseReplacementPolicy class, I guess that should be enough for whole compatibility
// We should include "params/HawkeyeRP.hh", but it doesn't exist. I found params files in build folder, but not in source
// One guess is it may be compiled if I do a fresh build, but I don't want to do that now
//struct BaseReplacementPolicyParams;
struct HawkeyeRPParams;

namespace replacement_policy
{
typedef std::size_t PCType;

class Hawkeye : public Base
{
  protected:
    /** Random-specific implementation of replacement data. */
    struct HawkeyeReplData : ReplacementData
    {
        public:
        /**
         * Flag informing if the replacement data is valid or not.
         * Invalid entries are prioritized to be evicted.
         */
        bool valid;

        /**
         * Default constructor. Invalidate data.
         */
        HawkeyeReplData(){
            valid = false;
        }
    };
    struct RRIPCacheData{
        int RRIP;
        int index;
        bool first_touch;
        uint64_t tag;
    };

  public:
    std::vector<std::vector<RRIPCacheData>> RRIP_vector;
    int vector_size;
    int way_assoc;
    int index_bit_count;
    int index_bit_mask;
    typedef HawkeyeRPParams Params;
    Hawkeye(const Params &p);
    ~Hawkeye() = default;

    /**
     * Invalidate replacement data to set it as the next probable victim.
     * Prioritize replacement data for victimization.
     *
     * @param replacement_data Replacement data to be invalidated.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                                    override;

    /**
     * This is required for some compatibility problems
    */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data,
        const PacketPtr pkt) override;
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
        override;

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Updates predictor and assigns RRPV values of Table 3.
     *
     * @param replacement_data Replacement data to be reset.
     * @param pkt Packet that generated this miss.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data,
        const PacketPtr pkt) override;
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
        override;

    /**
     * Find replacement victim at random.
     *
     * @param candidates Replacement candidates, selected by indexing policy.
     * @return Replacement entry to be replaced.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     *
     * @return A shared pointer to the new replacement data.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

} // namespace replacement_policy
} // namespace gem5

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_RANDOM_RP_HH__