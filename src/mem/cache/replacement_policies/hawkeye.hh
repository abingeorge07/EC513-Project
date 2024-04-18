/**
 * @file
 * Declaration of Hawkeye replacement policy.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_HAWKEYE_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_HAWKEYE_HH__

#include "mem/cache/replacement_policies/base.hh"

namespace gem5
{

// This is wrong, but I hope that we won't need params, SimObject uses it in BaseReplacementPolicy class, I guess that should be enough for whole compatibility
// We should include "params/HawkeyeRP.hh", but it doesn't exist. I found params files in build folder, but not in source
// One guess is it may be compiled if I do a fresh build, but I don't want to do that now
struct BaseReplacementPolicyParams;

namespace replacement_policy
{

class Hawkeye : public Base
{
  protected:
    /** Random-specific implementation of replacement data. */
    struct HawkeyeReplData : ReplacementData
    {
        /**
         * Flag informing if the replacement data is valid or not.
         * Invalid entries are prioritized to be evicted.
         */
        bool valid;

        /**
         * Default constructor. Invalidate data.
         */
        HawkeyeReplData() : valid(false) {}
    };

  public:
    typedef BaseReplacementPolicyParams Params;
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
     * Touch an entry to update its replacement data.
     * Does not do anything.
     *
     * @param replacement_data Replacement data to be touched.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data. Used when an entry is inserted.
     * Unprioritize replacement data for victimization.
     *
     * @param replacement_data Replacement data to be reset.
     */
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
