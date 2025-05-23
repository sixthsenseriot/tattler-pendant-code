//--------------------------------------------------------------------------------------------------
// Tools.cpp
//
//
// Main code for the project.
//
// Authors:
//            Khanh Nguyen
//            Mike Schoonover
//
// Date of project start: 2025/01/17
//
//--------------------------------------------------------------------------------------------------

#include "Tools.h"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// class Tools
//
/**
 *
 * This class is an example class which holds various useful functions, most of which are static
 * so they can be called without instantiating a class.
 *
 */

//--------------------------------------------------------------------------------------------------
// Tools::Tools (constructor)
//
/**
 *
 * Class constructor.
 *
 * The ONLY thing that should be done here is initializing objects and variables and storing
 * parameters to class variables.
 *
 */

Tools::Tools()

{

}// end of Tools::Tools (constructor)
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Tools::init
//
/**
 *
 * Initializes the object...call immediately after instantiation.
 *
 */

void Tools::init()
{

}// end of Tools::init
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Tools::printIntAsHexToSerial
//
/**
 *
 * Prints an integer as a 4 digit hex number to the Serial port.
 *
 * @param pNumber   the integer to be printed as a 4 digit hex number
 *
 */

void Tools::printIntAsHexToSerial(int pNumber)
{

    Serial.printf("%04X", pNumber);

}// end of Tools::printIntAsHexToSerial
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Tools::~Tools (destructor)
//
/**
 *
 * This gets called when the object is destroyed. All objects and memory allocated by this object
 * should be released here.
 *
 */

Tools::~Tools()
{

}// end of Tools::~Tools (destructor)
//--------------------------------------------------------------------------------------------------

//end of class Tools
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
