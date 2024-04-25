/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   /projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project/src/mem/slicc/symbols/Type.py:501
 */

#include <iostream>
#include <memory>

#include "mem/ruby/protocol/SequencerMsg.hh"
#include "mem/ruby/system/RubySystem.hh"

namespace gem5
{

namespace ruby
{

/** \brief Print the state of this object */
void
SequencerMsg::print(std::ostream& out) const
{
    out << "[SequencerMsg: ";
    out << "LineAddress = " << printAddress(m_LineAddress) << " ";
    out << "PhysicalAddress = " << printAddress(m_PhysicalAddress) << " ";
    out << "Type = " << m_Type << " ";
    out << "ProgramCounter = " << printAddress(m_ProgramCounter) << " ";
    out << "AccessMode = " << m_AccessMode << " ";
    out << "writeMask = " << m_writeMask << " ";
    out << "DataBlk = " << m_DataBlk << " ";
    out << "Len = " << m_Len << " ";
    out << "Prefetch = " << m_Prefetch << " ";
    out << "MessageSize = " << m_MessageSize << " ";
    out << "]";
}
bool
SequencerMsg::functionalRead(Packet* param_pkt)
{
return (false);

}
bool
SequencerMsg::functionalWrite(Packet* param_pkt)
{
return (testAndWrite(m_PhysicalAddress, m_DataBlk, param_pkt));

}
} // namespace ruby
} // namespace gem5
