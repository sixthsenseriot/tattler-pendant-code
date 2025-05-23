
#ifndef _EVENT_INFO_H
#define _EVENT_INFO_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>


//--------------------------------------------------------------------------------------------------
// class EventInfo
//
/**
 *
 * This class contains the information for an event.
 *
 */

class EventInfo
{

	public:         // functions ~ all classes can see these

        /**
         *
         * This constructor takes a reference to a String containing a "command" and a reference to
         * a String containing "data".
         *
         * The "command" is any arbitrary text which is known to both the sender and the receiver
         * of the EventInfo object as describing an action to be performed.
         *
         * The "data" is a collection of text and numbers in text format in any arbitrary
         * arrangement. Both the sender and receiver are expected to use a format known to each of
         * them.
         *
         * Example:
         *
         *	pCommand = "Set Gain and Offset"
         *	pData = "1.0, 2.3"
         *		or
         *	pData = "Gain = 1.0 | Offset = 2.3"
         *		or
         *	...many other possible formats...
         *
         * The receiver should know how to split, trim, convert the values in the data String as required.
         *
         * @param pCommand	the "command" from sender to receiver in text
         * @param pData		the "data" from sender to receiver in text
         *
         */         
         
         
         
        EventInfo(String pCommand, String pData):
        command(pCommand),
        data(pData)
        {


        }//end of EventInfo::EventInfo (constructor)


	    String getCommand() { return command; }
	    void setCommand(String pCommand) { command = pCommand; }

	    String data;

	    String getData() { return(data); }
	    void setData(String pData) { data = pData; }

	protected:      // functions ~ only this class and derived (child) classes can see these


    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these


    protected:      // variables ~ only this class and derived (child) classes can see these

	    String command;

    private:        // variables ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent variables )

    public:         // put static const variables down here where they are easy to find

};// end of class EventInfo
//--------------------------------------------------------------------------------------------------

#endif // _EVENT_INFO_H