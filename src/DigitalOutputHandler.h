
#ifndef _DIGITAL_OUTPUT_HANDLER_H
#define _DIGITAL_OUTPUT_HANDLER_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>

#include "DelayHandler.h"

//--------------------------------------------------------------------------------------------------
// class DigitalOutputHandler
//
/**
 *
 * This is an example class.
 *
 */

class DigitalOutputHandler{

	public:         // functions ~ all classes can see these

                    // WARNING: if the parameter names used here do no match those used in the
                    // .cpp file, no error will occur...but for clarity they SHOULD match

                    // for SKOONIE projects, all parameters begin with 'p'

                    // use const prefix for all parameters where possible...this helps the
                    // compiler catch errors and can result in more efficient code

		DigitalOutputHandler(
                        const int pIOPinNum, const String pObjectName, const int pObjectNumber);

		void init();

		void process();

        void setState(const int pState);

        void startOnPulse(const int pDurationMS);

        void startPulseSequence(
            const int pOnDurationMS1, const int pOffDurationMS1,
            const int pOnDurationMS2, const int pOffDurationMS2,
            const int pOnDurationMS3, const int pOffDurationMS3);

        void handleOnPulse();

        void handlePulseSequence();

        virtual ~DigitalOutputHandler();

	protected:      // functions ~ only this class and derived (child) classes can see these


    private:        // functions ~ only this class can see these ( use sparingly as it is often
                    //    becomes necessary for derived classes to access parent functions )


	public:         // variables ~ all classes can see these

        String className;

    protected:      // variables ~ only this class and derived (child) classes can see these

        int objectNumber;

        String objectName;

        int ioPinNum;

        int state;

        int stateMachine;

        int mode;

        int durationMS1, durationMS2, durationMS3;

        DelayHandler pulseDelayHandler;

        const int PULSE_DELAY_LIMIT_MS = 3000;

        int onDurationMS1, onDurationMS2, onDurationMS3;
        int offDurationMS1, offDurationMS2, offDurationMS3;

    private:

        static const int PULSE_DELAY_HANDLER_OBJECT_NUMBER = 1;

        static const int STATE_IDLE = -1;
        static const int STATE_START_SEQUENCE = 0;

        static const int MODE_IDLE = -1;
        static const int MODE_HANDLE_ON_PULSE = 0;
        static const int MODE_HANDLE_SEQUENCE = 1;

    public:         // put static const variables down here where they are easy to find





};// end of class DigitalOutputHandler
//--------------------------------------------------------------------------------------------------

#endif // _DIGITAL_OUTPUT_HANDLER_H