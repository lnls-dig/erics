#include <string>

#include <iocsh.h>
#include <epicsExport.h>

#include <asynPortDriver.h>

#include "lamp.h"

class RtmLamp: public asynPortDriver {
  public:
    RtmLamp(int port_number):
      asynPortDriver(
          (std::string("RTMLAMP") + std::to_string(port_number)).c_str(),
          12, /* channels */
          -1, -1, /* enable all interfaces and interrupts */
          0, 1, 0, 0 /* no flags, auto connect, default priority and stack size */
      )
    {
    }
};

/* Configuration routine.  Called directly, or from the iocsh function below */
extern "C" {
    /* EPICS iocsh shell commands */
    static const iocshArg initArg0 = {"portNumber", iocshArgInt};
    static const iocshArg *initArgs[] = {&initArg0};
    static const iocshFuncDef initFuncDef = {"RtmLamp", 1, initArgs};
    static void initCallFunc(const iocshArgBuf *args)
    {
        new RtmLamp(args[0].ival);
    }

    static void registerRtmLamp(void)
    {
        iocshRegister(&initFuncDef, initCallFunc);
    }

    epicsExportRegistrar(registerRtmLamp);
}
