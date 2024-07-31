#include <iocsh.h>
#include <asynPortDriver.h>

#include <epicsExport.h>

#include <wiringPi.h>

static const char *driverName = "PI4PWM";

// Analog output parameters
#define analogOutValueString      "ANALOG_OUT_VALUE"

#define NUM_ANALOG_OUT  4

#define MAX_SIGNALS     NUM_ANALOG_OUT

class PI4PWM : public asynPortDriver {

public:
    PI4PWM(const char* portName, int pinNumber);

    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus getBounds(asynUser *pasynUser, epicsInt32 *low, epicsInt32 *high);

protected:
    int _analogOutValue;
    #define FIRST_PI4PWM_PARAM  _analogOutValue
    #define LAST_PI4PWM_PARAM   _analogOutValue

private:
    int _pinNumber;

};

#define NUM_PARAMS ((int)(&FIRST_PI4PWM_PARAM - &LAST_PI4PWM_PARAM + 1))

PI4PWM::PI4PWM(const char* portName, int pinNumber)
: asynPortDriver(portName, MAX_SIGNALS, NUM_PARAMS, 
      asynInt32Mask | asynDrvUserMask,  // Interfaces that we implement
      0,                                // Interfaces that do callbacks
      ASYN_MULTIDEVICE | ASYN_CANBLOCK, 1, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=1, autoConnect=1 */
      0, 0),  /* Default priority and stack size */
    _pinNumber(pinNumber) {
    
    // Analog output parameters
    createParam(analogOutValueString, asynParamInt32, &_analogOutValue);
}

asynStatus PI4PWM::getBounds(asynUser *pasynUser, epicsInt32 *low, epicsInt32 *high) {
    int function = pasynUser->reason;

    // Analog outputs are in the range 0-1023
    if (function == _analogOutValue) {
        *low = 0;
        *high = 1023;
        return(asynSuccess);
    } else {
        return(asynError);
    }
}

asynStatus PI4PWM::writeInt32(asynUser *pasynUser, epicsInt32 value) {
    int addr;
    int function = pasynUser->reason;
    static const char *functionName = "writeInt32";

    this->getAddress(pasynUser, &addr);
    setIntegerParam(addr, function, value);

    // Analog output functions
    if (function == _analogOutValue) {
        pwmWrite(_pinNumber, value);
    }

    callParamCallbacks(addr);
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
                "%s:%s, port %s, wrote %d to address %d\n",
                driverName, functionName, this->portName, value, addr);
    return asynSuccess;
}

/** Configuration command, called directly or from iocsh */

extern "C" int PI4PWMConfig(const char* portName, int pinNumber) {
    if (wiringPiSetup() == -1) {
        return asynError;
    }
    pinMode(pinNumber, PWM_OUTPUT);

    PI4PWM* pointerPI4PWM = new PI4PWM(portName, pinNumber);
    pointerPI4PWM = NULL; /* This is just to avoid compiler warnings */
    return asynSuccess;
}

static const iocshArg configArg0 = { "Port name", iocshArgString};
static const iocshArg configArg1 = { "Pin number", iocshArgInt};
static const iocshArg * const configArgs[] = {&configArg0,
                                              &configArg1};
static const iocshFuncDef configFuncDef = {"PI4PWMConfig", 2, configArgs};

static void configCallFunc(const iocshArgBuf *args) {
    PI4PWMConfig(args[0].sval, args[1].ival);
}

void drvPI4PWMRegister(void) {
    iocshRegister(&configFuncDef,configCallFunc);
}

extern "C" {
    epicsExportRegistrar(drvPI4PWMRegister);
}
