
#ifndef _BUZZER_HANDLER_H
#define _BUZZER_HANDLER_H

// the above preprocessor directives are referred to as 'Header Guards'

#include <Arduino.h>

#include "DelayHandler.h"

//--------------------------------------------------------------------------------------------------
// class BuzzerHandler
//
/**
 *
 * This is an example class.
 *
 */

class BuzzerHandler{

	public:         // functions ~ all classes can see these

                    // WARNING: if the parameter names used here do no match those used in the
                    // .cpp file, no error will occur...but for clarity they SHOULD match

                    // for SKOONIE projects, all parameters begin with 'p'

                    // use const prefix for all parameters where possible...this helps the
                    // compiler catch errors and can result in more efficient code

		BuzzerHandler(const int pIOPinNum, const String pObjectName, const int pObjectNumber);

		void init();

		void process();

        void handlePulse();

        void handlePulseSequence();

        void startPulseSequence(
            const int pFrequencyHz1, const int pDurationMS1,
            const int pFrequencyHz2, const int pDurationMS2,
            const int pFrequencyHz3, const int pDurationMS3);

        void emitTone(const int pFrequencyHz);

        void endTone();

        void startPulse(const int pFrequencyHz, const int pDurationMS);

        virtual ~BuzzerHandler();

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

        int frequencyHz;

        int stateMachine;

        int mode;

        int frequencyHz1, frequencyHz2, frequencyHz3;

        int durationMS1, durationMS2, durationMS3;

        DelayHandler pulseDelayHandler;

        const int PULSE_DELAY_LIMIT_MS = 3000;

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

        static const int PULSE_DELAY_HANDLER_OBJECT_NUMBER = 1;

        static const int PWM_CHANNEL = 0;

        static const int PWM_RESOLUTION = 8;

        static const int STATE_IDLE = -1;
        static const int STATE_START_SEQUENCE = 0;

        static const int MODE_IDLE = -1;
        static const int MODE_HANDLE_PULSE = 0;
        static const int MODE_HANDLE_SEQUENCE = 1;

    public:         // put static const variables down here where they are easy to find

};// end of class BuzzerHandler
//--------------------------------------------------------------------------------------------------

#endif // _BUZZER_HANDLER_H