
#ifndef _EVENT_HANDLER_INTERFACE_H
#define _EVENT_HANDLER_INTERFACE_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>

#include "EventInfo.h"


//--------------------------------------------------------------------------------------------------
// Class (Interface) EventHandlerInterface
//
/**
 *
 * This is an interface for objects which handle events receive from other objects.
 *
 * This class is designated as an interface by having all functions declared as virtual.
 *
 */

class EventHandlerInterface
{

	public:         // functions ~ all classes can see these

        virtual String handleEvent(EventInfo pEventInfo){ return(""); }

};// end of class (Interface) EventHandlerInterface
//--------------------------------------------------------------------------------------------------

#endif // _EVENT_HANDLER_INTERFACE_H