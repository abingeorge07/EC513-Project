/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   /projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project/src/mem/slicc/symbols/Type.py:501
 */

#include <iostream>
#include <memory>

#include "mem/ruby/protocol/Directory_TBE.hh"
#include "mem/ruby/system/RubySystem.hh"

namespace gem5
{

namespace ruby
{

/** \brief Print the state of this object */
void
Directory_TBE::print(std::ostream& out) const
{
    out << "[Directory_TBE: ";
    out << "PhysicalAddress = " << printAddress(m_PhysicalAddress) << " ";
    out << "TBEState = " << m_TBEState << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Len = " << m_Len << " ";
    out << "Requestor = " << m_Requestor << " ";
    out << "]";
}
} // namespace ruby
} // namespace gem5
