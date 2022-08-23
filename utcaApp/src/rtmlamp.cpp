#include <iocsh.h>
#include <epicsExport.h>

#include <asynPortDriver.h>

#include "lamp.h"

class RtmLamp: public asynPortDriver {

  public:
    RtmLamp(const char *portName):
      asynPortDriver(portName, 10, -1, -1, 0, 1, 0, 0)
    {
        printf("hallo %s\n", portName);
    }
};

/* Configuration routine.  Called directly, or from the iocsh function below */
extern "C" {
    /* EPICS iocsh shell commands */
    static const iocshArg initArg0 = { "portName", iocshArgString};
    static const iocshArg * const initArgs[] = {&initArg0};
    static const iocshFuncDef initFuncDef = {"RtmLamp", 1,initArgs};
    static void initCallFunc(const iocshArgBuf *args)
    {
        new RtmLamp(args[0].sval);
    }

    void testSupportRegister(void)
    {
        iocshRegister(&initFuncDef,initCallFunc);
    }

    epicsExportRegistrar(testSupportRegister);
}
