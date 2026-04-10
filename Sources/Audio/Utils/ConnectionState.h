#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include <stdint.h>

/** Outgoing call states
 *
 *  Idle -> OutgoingCall
 *  OutgoingCall -> Active/Denined/Ended
 *  Active -> Missing/Ended
 *  Missing -> Active/Denied/Ended
 *  Ended -> Idle
 *  Denied -> Idle
 **/

/** Incoming call states
 *
 *  Idle -> IncomingCall
 *  IncomingCall -> Active/Denied/Ended
 *  Active -> Missing/Ended
 *  Missing -> Active/Denined/Ended
 *  Ended -> Idle
 *  Denied -> Idle
 **/

enum ConnectionState
{
    Active, // Correct connection
    Denied, // Сall was cancelled / disconnected
    Missing, // Problem with internet connection
    Ended, // Call ended sucessfully
    Idle,
    IncomingCall,
    OutgoingCall
};

#endif // CONNECTIONSTATE_H
