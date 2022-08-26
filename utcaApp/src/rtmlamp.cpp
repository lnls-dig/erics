#include <stdexcept>
#include <string>
#include <tuple>

#include <iocsh.h>
#include <epicsExport.h>

#include <asynPortDriver.h>

#include "lamp.h"

#include "pcie-single.h"

const static unsigned number_of_channels = 12;

class RtmLamp: public asynPortDriver {
    LnlsRtmLampControllerV2 ctl;

    int p_psstatus, p_overcurr_l, p_overtemp_l, p_overcurr_r, p_overtemp_r,
        p_pwrstate, p_opmode, p_loopkp, p_loopti,
        p_testwaveperiod, p_testlim_a, p_testlim_b,
        p_pi_sp, p_dac, p_eff_adc, p_eff_dac, p_eff_sp,
        p_trigen;

  public:
    RtmLamp(int port_number):
      asynPortDriver(
          (std::string("RTMLAMP-") + std::to_string(port_number)).c_str(),
          number_of_channels, /* channels */
          -1, -1, /* enable all interfaces and interrupts */
          0, 1, 0, 0 /* no flags, auto connect, default priority and stack size */
      ),
      ctl(&bars)
    {
        if (auto v = read_sdb(&bars, ctl.device_match, port_number)) {
            ctl.set_devinfo(*v);
        } else {
            throw std::runtime_error("couldn't find lamp module");
        }

        std::tuple<const char *, int &> name_and_index[] = {
            {"PS_STATUS", p_psstatus},
        };

        std::tuple<const char *, int &> name_and_index_channel[] = {
            {"OVERCURR_L", p_overcurr_l},
            {"OVERTEMP_L", p_overtemp_l},
            {"OVERCURR_R", p_overcurr_r},
            {"OVERTEMP_R", p_overtemp_r},
            {"PWR_STATE", p_pwrstate},
            {"OP_MODE", p_opmode},
            {"LOOP_KP", p_loopkp},
            {"LOOP_TI", p_loopti},
            {"TEST_WAVE_PERIOD", p_testwaveperiod},
            {"TEST_LIM_A", p_testlim_a},
            {"TEST_LIM_B", p_testlim_b},
            {"PI_SP", p_pi_sp},
            {"DAC", p_dac},
            {"EFF_ADC", p_eff_adc},
            {"EFF_DAC", p_eff_dac},
            {"EFF_SP", p_eff_sp},
            {"TRIG_EN", p_trigen},
        };

        for (auto &v: name_and_index) {
            createParam(std::get<0>(v), asynParamInt32, &std::get<1>(v));
            setIntegerParam(0, std::get<1>(v), 0);
        }

        for (auto &v: name_and_index_channel) {
            createParam(std::get<0>(v), asynParamInt32, &std::get<1>(v));
            for (unsigned addr = 0; addr < number_of_channels; addr++) {
                setIntegerParam(addr, std::get<1>(v), 0);
            }
        }
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
        try {
            new RtmLamp(args[0].ival);
        } catch (std::runtime_error &e) {
        }
    }

    static void registerRtmLamp(void)
    {
        iocshRegister(&initFuncDef, initCallFunc);
    }

    epicsExportRegistrar(registerRtmLamp);
}
