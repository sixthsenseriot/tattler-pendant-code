//--------------------------------------------------------------------------------------------------
// main.cpp OR <project name>.ino
//
// VSCode projects all have a main.cpp file while Arduino projects have a <project name>.ino file.
// These serve as the starting point for the program...they are identical in function.
//
// For all Skoonie projects, the main.cpp file or the .ino file do NOTHING except create the
// MainController object and start it up.
//
// Note that 'real' cpp programs have a main.ccp file with a main function as the starting point.
// The setup and loop functions do not exist in 'real' cpp programs...those are creations for the
// Arduino ecosystem to simplify programming for newbies.
//
// See notes at the top of the MainController.cpp file for details pertaining to each project.
//
// ~~ DO NOT CHANGE THIS FILE...IT IS IDENTICAL FOR ALL SKOONIE PROJECTS ~~
//

#include <Arduino.h>

#include "MainController.h"

MainController mainController;

//--------------------------------------------------------------------------------------------------
// setup
//
// This is the standard Arduino "setup" function which is called once on startup.
//

void setup() {

  mainController.init();

}// end of setup
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// loop
//
// This is the standard Arduino "loop" function which loops continuously after "setup"
// function has been executed.
//
// At 1000 loops, the time it took for those loops will be printed on the serial com
// port. This is useful for debugging and code comparison.
//

void loop(void) {

  mainController.process();

}// end of loop
//--------------------------------------------------------------------------------------------------
