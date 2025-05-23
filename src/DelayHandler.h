
#ifndef _DELAY_HANDLER_H
#define _DELAY_HANDLER_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>
#include <cmath>

//--------------------------------------------------------------------------------------------------
// class DelayHandler
//
/**
 *
 * This class handles a delay timer.
 *
 */

class DelayHandler{

	public:         // functions ~ all classes can see these

                    // WARNING: if the parameter names used here do no match those used in the
                    // .cpp file, no error will occur...but for clarity they SHOULD match

                    // for SKOONIE projects, all parameters begin with 'p'

                    // use const prefix for all parameters where possible...this helps the
                    // compiler catch errors and can result in more efficient code

		explicit DelayHandler(
             const unsigned long pDelayTimeMS, const String pObjectName, const int pObjectNumber);

		void init();

        void start(const unsigned long pDelayTimeMS);

        void start();

		void process();

        virtual ~DelayHandler();
        
        bool isRunning();

	protected:      // functions ~ only this class and derived (child) classes can see these
    

    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these

        String className;

    protected:      // variables ~ only this class and derived (child) classes can see these

        int objectNumber;

        String objectName;

        unsigned long startTimeMS;

        unsigned long delayTimeMS;

                    // NOTE:
                    //
                    // for Arduino-based environments, String is part of the Arduino core libarary
                    // and you should use '#include <Arduino.h>' at the top of this file.
                    //
                    // For 'real' CPP programs, include the <string.h> library and use
                    // 'std::string objectName' instead of 'String objectName'.
                    //

    private:        // variables ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent variables )

    public:         // put static const variables down here where they are easy to find

};// end of class DelayHandler
//--------------------------------------------------------------------------------------------------

#endif // _DELAY_HANDLER_H