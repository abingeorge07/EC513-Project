/**
 * DO NOT EDIT THIS FILE!
 * File automatically generated by
 *   /projectnb/ec513/students/abg309/Project/HW6/spec-2017/EC513-Project/src/mem/slicc/symbols/Type.py:643
 */

#ifndef __LockStatus_HH__
#define __LockStatus_HH__

#include <iostream>
#include <string>

namespace gem5
{

namespace ruby
{


// Class definition
/** \enum LockStatus
 *  \brief ...
 */
enum LockStatus {
    LockStatus_FIRST,
    LockStatus_Unlocked = LockStatus_FIRST, /**< Lock is not held */
    LockStatus_Locked, /**< Lock is held */
    LockStatus_NUM
};

// Code to convert from a string to the enumeration
LockStatus string_to_LockStatus(const ::std::string& str);

// Code to convert state to a string
::std::string LockStatus_to_string(const LockStatus& obj);

// Code to increment an enumeration type
LockStatus &operator++(LockStatus &e);

::std::ostream&
operator<<(::std::ostream& out, const LockStatus& obj);

} // namespace ruby
} // namespace gem5
#endif // __LockStatus_HH__
