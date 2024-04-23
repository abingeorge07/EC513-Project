/**
 * Copyright (c) 2018-2020 Inria
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mem/cache/replacement_policies/mru_rp.hh"

#include <cassert>
#include <memory>

#include "params/MRURP.hh"
#include "sim/cur_tick.hh"

namespace gem5
{

namespace replacement_policy
{

MRU::MRU(const Params &p)
  : Base(p)
{
}

void
MRU::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
{
    // Unprioritize replacement data victimization
    std::static_pointer_cast<MRUReplData>(
        replacement_data)->valid = false;
}

void MRU::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    panic("touch() should never be called with a single MRU replacement "
          "policy. It should always be called with a packet.\n");
}

void MRU::touch(const std::shared_ptr<ReplacementData>& replacement_data,
        const PacketPtr pkt)
{
    ;
}

void
MRU::reset(const std::shared_ptr<ReplacementData>& replacement_data,
    const PacketPtr pkt)
{   
    ;
}

void MRU::reset(const std::shared_ptr<ReplacementData>& replacement_data)
    const
{
    panic("Cant train SHiP's predictor without access information.");
}

ReplaceableEntry*
MRU::getVictim(const ReplacementCandidates& candidates) const
{

    // Do not forget to check valid entries somewhere
    ReplaceableEntry* victim = candidates[0];
    return victim;
}

std::shared_ptr<ReplacementData>
MRU::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new MRUReplData());
}

} // namespace replacement_policy
} // namespace gem5
