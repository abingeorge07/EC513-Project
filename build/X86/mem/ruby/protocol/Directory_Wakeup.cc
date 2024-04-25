/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   /projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project/src/mem/slicc/symbols/StateMachine.py:1497
 */

// Directory: MESI Two Level directory protocol

#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <typeinfo>

#include "base/logging.hh"

#include "debug/RubyGenerated.hh"
#include "debug/RubySlicc.hh"
#include "mem/ruby/protocol/Directory_Controller.hh"
#include "mem/ruby/protocol/Directory_Event.hh"
#include "mem/ruby/protocol/Directory_State.hh"

#include "mem/ruby/protocol/Types.hh"
#include "mem/ruby/system/RubySystem.hh"

#include "mem/ruby/slicc_interface/RubySlicc_includes.hh"
namespace gem5
{

namespace ruby
{

void
Directory_Controller::wakeup()
{
    if (getMemReqQueue() && getMemReqQueue()->isReady(clockEdge())) {
        serviceMemoryQueue();
    }

    int counter = 0;
    while (true) {
        unsigned char rejected[3];
        memset(rejected, 0, sizeof(unsigned char)*3);
        // Some cases will put us into an infinite loop without this limit
        assert(counter <= m_transitions_per_cycle);
        if (counter == m_transitions_per_cycle) {
            // Count how often we are fully utilized
            stats.fullyBusyCycles++;

            // Wakeup in another cycle and try again
            scheduleEvent(Cycles(1));
            break;
        }
            // DirectoryInPort requestNetwork_in
            m_cur_in_port = 0;
            try {
                            if ((((*m_requestToDir_ptr)).isReady((clockEdge())))) {
                                {
                                    // Declare message
                                    [[maybe_unused]] const RequestMsg* in_msg_ptr;
                                    in_msg_ptr = dynamic_cast<const RequestMsg *>(((*m_requestToDir_ptr)).peek());
                                    if (in_msg_ptr == NULL) {
                                        // If the cast fails, this is the wrong inport (wrong message type).
                                        // Throw an exception, and the caller will decide to either try a
                                        // different inport or punt.
                                        throw RejectException();
                                    }
                                #ifndef NDEBUG
                                if (!(((((*in_msg_ptr)).m_Destination).isElement(m_machineID)))) {
                                    panic("Runtime Error at MESI_Two_Level-dir.sm:200: %s.\n", "assert failure");

                                }
                                #endif
                                ;
                                    if ((isGETRequest(((*in_msg_ptr)).m_Type))) {
                                        {

                                            TransitionResult result = doTransition(Directory_Event_Fetch, (((*m_TBEs_ptr)).lookup(((*in_msg_ptr)).m_addr)), ((*in_msg_ptr)).m_addr);

                                            if (result == TransitionResult_Valid) {
                                                counter++;
                                                continue; // Check the first port again
                                            } else if (result == TransitionResult_ResourceStall) {

                                                scheduleEvent(Cycles(1));
                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                            } else if (result == TransitionResult_ProtocolStall) {

                                                scheduleEvent(Cycles(1));
                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                            }

                                        }
                                        ;
                                    } else {
                                            if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_DMA_READ)) {
                                                {

                                                    TransitionResult result = doTransition(Directory_Event_DMA_READ, (((*m_TBEs_ptr)).lookup((makeLineAddress(((*in_msg_ptr)).m_addr)))), (makeLineAddress(((*in_msg_ptr)).m_addr)));

                                                    if (result == TransitionResult_Valid) {
                                                        counter++;
                                                        continue; // Check the first port again
                                                    } else if (result == TransitionResult_ResourceStall) {

                                                        scheduleEvent(Cycles(1));
                                                        // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                    } else if (result == TransitionResult_ProtocolStall) {

                                                        scheduleEvent(Cycles(1));
                                                        // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                    }

                                                }
                                                ;
                                            } else {
                                                    if ((((*in_msg_ptr)).m_Type == CoherenceRequestType_DMA_WRITE)) {
                                                        {

                                                            TransitionResult result = doTransition(Directory_Event_DMA_WRITE, (((*m_TBEs_ptr)).lookup((makeLineAddress(((*in_msg_ptr)).m_addr)))), (makeLineAddress(((*in_msg_ptr)).m_addr)));

                                                            if (result == TransitionResult_Valid) {
                                                                counter++;
                                                                continue; // Check the first port again
                                                            } else if (result == TransitionResult_ResourceStall) {

                                                                scheduleEvent(Cycles(1));
                                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                            } else if (result == TransitionResult_ProtocolStall) {

                                                                scheduleEvent(Cycles(1));
                                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                            }

                                                        }
                                                        ;
                                                    } else {
                                                        DPRINTF(RubySlicc, "MESI_Two_Level-dir.sm:210: %s\n", (*in_msg_ptr));
                                                        panic("Runtime Error at MESI_Two_Level-dir.sm:211: %s.\n", ("Invalid message"));
                                                        ;
                                                    }
                                                }
                                            }
                                            }
                                        }
                        } catch (const RejectException & e) {
                            rejected[0]++;
                        }
            // DirectoryInPort responseNetwork_in
            m_cur_in_port = 1;
            try {
                            if ((((*m_responseToDir_ptr)).isReady((clockEdge())))) {
                                {
                                    // Declare message
                                    [[maybe_unused]] const ResponseMsg* in_msg_ptr;
                                    in_msg_ptr = dynamic_cast<const ResponseMsg *>(((*m_responseToDir_ptr)).peek());
                                    if (in_msg_ptr == NULL) {
                                        // If the cast fails, this is the wrong inport (wrong message type).
                                        // Throw an exception, and the caller will decide to either try a
                                        // different inport or punt.
                                        throw RejectException();
                                    }
                                #ifndef NDEBUG
                                if (!(((((*in_msg_ptr)).m_Destination).isElement(m_machineID)))) {
                                    panic("Runtime Error at MESI_Two_Level-dir.sm:220: %s.\n", "assert failure");

                                }
                                #endif
                                ;
                                    if ((((*in_msg_ptr)).m_Type == CoherenceResponseType_MEMORY_DATA)) {
                                        {

                                            TransitionResult result = doTransition(Directory_Event_Data, (((*m_TBEs_ptr)).lookup(((*in_msg_ptr)).m_addr)), ((*in_msg_ptr)).m_addr);

                                            if (result == TransitionResult_Valid) {
                                                counter++;
                                                continue; // Check the first port again
                                            } else if (result == TransitionResult_ResourceStall) {

                                                scheduleEvent(Cycles(1));
                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                            } else if (result == TransitionResult_ProtocolStall) {

                                                scheduleEvent(Cycles(1));
                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                            }

                                        }
                                        ;
                                    } else {
                                            if ((((*in_msg_ptr)).m_Type == CoherenceResponseType_ACK)) {
                                                {

                                                    TransitionResult result = doTransition(Directory_Event_CleanReplacement, (((*m_TBEs_ptr)).lookup(((*in_msg_ptr)).m_addr)), ((*in_msg_ptr)).m_addr);

                                                    if (result == TransitionResult_Valid) {
                                                        counter++;
                                                        continue; // Check the first port again
                                                    } else if (result == TransitionResult_ResourceStall) {

                                                        scheduleEvent(Cycles(1));
                                                        // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                    } else if (result == TransitionResult_ProtocolStall) {

                                                        scheduleEvent(Cycles(1));
                                                        // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                    }

                                                }
                                                ;
                                            } else {
                                                DPRINTF(RubySlicc, "MESI_Two_Level-dir.sm:226: %s\n", ((*in_msg_ptr)).m_Type);
                                                panic("Runtime Error at MESI_Two_Level-dir.sm:227: %s.\n", ("Invalid message"));
                                                ;
                                            }
                                        }
                                        }
                                    }
                        } catch (const RejectException & e) {
                            rejected[1]++;
                        }
            // DirectoryInPort memQueue_in
            m_cur_in_port = 2;
            try {
                            if ((((*m_responseFromMemory_ptr)).isReady((clockEdge())))) {
                                {
                                    // Declare message
                                    [[maybe_unused]] const MemoryMsg* in_msg_ptr;
                                    in_msg_ptr = dynamic_cast<const MemoryMsg *>(((*m_responseFromMemory_ptr)).peek());
                                    if (in_msg_ptr == NULL) {
                                        // If the cast fails, this is the wrong inport (wrong message type).
                                        // Throw an exception, and the caller will decide to either try a
                                        // different inport or punt.
                                        throw RejectException();
                                    }
                                    if ((((*in_msg_ptr)).m_Type == MemoryRequestType_MEMORY_READ)) {
                                        {

                                            TransitionResult result = doTransition(Directory_Event_Memory_Data, (((*m_TBEs_ptr)).lookup(((*in_msg_ptr)).m_addr)), ((*in_msg_ptr)).m_addr);

                                            if (result == TransitionResult_Valid) {
                                                counter++;
                                                continue; // Check the first port again
                                            } else if (result == TransitionResult_ResourceStall) {

                                                scheduleEvent(Cycles(1));
                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                            } else if (result == TransitionResult_ProtocolStall) {

                                                scheduleEvent(Cycles(1));
                                                // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                            }

                                        }
                                        ;
                                    } else {
                                            if ((((*in_msg_ptr)).m_Type == MemoryRequestType_MEMORY_WB)) {
                                                {

                                                    TransitionResult result = doTransition(Directory_Event_Memory_Ack, (((*m_TBEs_ptr)).lookup(((*in_msg_ptr)).m_addr)), ((*in_msg_ptr)).m_addr);

                                                    if (result == TransitionResult_Valid) {
                                                        counter++;
                                                        continue; // Check the first port again
                                                    } else if (result == TransitionResult_ResourceStall) {

                                                        scheduleEvent(Cycles(1));
                                                        // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                    } else if (result == TransitionResult_ProtocolStall) {

                                                        scheduleEvent(Cycles(1));
                                                        // Cannot do anything with this transition, go check next doable transition (mostly likely of next port)

                                                    }

                                                }
                                                ;
                                            } else {
                                                DPRINTF(RubySlicc, "MESI_Two_Level-dir.sm:242: %s\n", ((*in_msg_ptr)).m_Type);
                                                panic("Runtime Error at MESI_Two_Level-dir.sm:243: %s.\n", ("Invalid message"));
                                                ;
                                            }
                                        }
                                        }
                                    }
                        } catch (const RejectException & e) {
                            rejected[2]++;
                        }
        // If we got this far, we have nothing left todo or something went
        // wrong
        break;
    }
}

} // namespace ruby
} // namespace gem5
