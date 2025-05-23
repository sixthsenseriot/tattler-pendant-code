
#ifndef _TOOLS_H
#define _TOOLS_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>


//--------------------------------------------------------------------------------------------------
// class Tools
//
/**
 *
 * This class is an example class which holds various useful functions, most of which are static
 * so they can be called without instantiating a class.
 *
 */

class Tools
{

	public:         // functions ~ all classes can see these

		Tools();

		void init();

        static void printIntAsHexToSerial(int pNumber);

        virtual ~Tools();


	protected:      // functions ~ only this class and derived (child) classes can see these


    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these


    protected:      // variables ~ only this class and derived (child) classes can see these


    private:        // variables ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent variables )

    public:         // put static const variables down here where they are easy to find


};// end of class Tools
//--------------------------------------------------------------------------------------------------

#endif // _TOOLS_H