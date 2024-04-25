/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   /projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project/src/mem/slicc/symbols/Type.py:501
 */

#include <iostream>
#include <memory>

#include "mem/ruby/protocol/L2Cache_Entry.hh"
#include "mem/ruby/system/RubySystem.hh"

namespace gem5
{

namespace ruby
{

/** \brief Print the state of this object */
void
L2Cache_Entry::print(std::ostream& out) const
{
    out << "[L2Cache_Entry: ";
    out << "CacheState = " << m_CacheState << " ";
    out << "Sharers = " << m_Sharers << " ";
    out << "Exclusive = " << m_Exclusive << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Dirty = " << m_Dirty << " ";
    out << "]";
}
} // namespace ruby
} // namespace gem5
