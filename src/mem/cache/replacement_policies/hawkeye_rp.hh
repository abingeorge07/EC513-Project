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
        int index;
        uint64_t tag;
        Tick lastTouchTick; // as a backup LRU

        /**
         * Default constructor. Invalidate data.
         */
        HawkeyeReplData(){
            valid = false;
            index = 0;
            tag = UINT64_MAX;
        }
    };
    struct RRIPCacheData{
        int RRIP;
        int index;
        uint64_t tag;
    };
    struct OPTGenData{
        int hashed_pc;
        uint64_t address;
        int occupancy_count;
        bool is_incremented; // True only if the occupancy count has been incremented as a result of its own cache usage
    };
    struct OPTGenResponse{
        int last_pc;
        bool hit;
    };

  public:
    // Have a copy of LRU as a backup process

    std::vector<std::vector<RRIPCacheData>> RRIP_vector;
    // occupancy vector will be very tricky to implement efficiently
    std::vector<std::deque<OPTGenData>> occupancy_vector;
    std::vector<int> hawkeye_predictor_vector;
    bool hawkeye_predictor(OPTGenResponse resp);
    int vector_size;
    int way_assoc;
    int index_bit_count;
    int index_bit_mask;
    int offset_bit_count;
    int tag_bit_count;
    bool true_if_l1;
    int occupancy_vector_size;
    typedef HawkeyeRPParams Params;
    void initialize_occupancy_vector();
    OPTGenResponse occupancy_vector_query(int index, uint64_t address, int pc);
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
    void reset_backup(const std::shared_ptr<ReplacementData>& replacement_data,
        const PacketPtr pkt);
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