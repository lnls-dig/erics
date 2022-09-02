#include <stdexcept>
#include <string>
#include <tuple>

#include <iocsh.h>
#include <epicsExport.h>
#include <cantProceed.h>

#include <asynPortDriver.h>

#include "lamp.h"

#include "pcie-single.h"

const static unsigned number_of_channels = 12;

class RtmLamp: public asynPortDriver {
    lamp::ControllerV2 ctl;

    int p_psstatus, p_overcurr_l, p_overtemp_l, p_overcurr_r, p_overtemp_r,
        p_pwrstate, p_opmode, p_loopkp, p_loopti,
        p_testwaveperiod, p_testlim_a, p_testlim_b,
        p_pi_sp, p_dac, p_eff_adc, p_eff_dac, p_eff_sp,
        p_trigen;

    /* XXX: update these when new p_* variables are added */
    const int &first_parameter = p_psstatus, &last_parameter = p_trigen;

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

        /* here, we don't call setIntegerParam to initialize these parameters:
         * - readback ones are expected to be initialized soon via SCAN
         * - setpoint ones are expected to be initialized whenever their records
         *   are written into: this also allows us to avoid writing into fields
         *   which aren't always supported, as long as the PVs haven't been written
         *   into */

        for (auto &v: name_and_index) {
            createParam(std::get<0>(v), asynParamInt32, &std::get<1>(v));
        }

        for (auto &v: name_and_index_channel) {
            createParam(std::get<0>(v), asynParamInt32, &std::get<1>(v));
        }
    }

    asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value)
    {
        const int function = pasynUser->reason;
        int addr;
        getAddress(pasynUser, &addr);

        if (function < first_parameter || function > last_parameter) {
            return asynPortDriver::writeInt32(pasynUser, value);
        }

        setIntegerParam(addr, function, value);
        callParamCallbacks(addr);

        ctl.channel = addr;

        auto write_param = [addr, this](auto fn, auto &ctl_value) {
            epicsInt32 tmp;
            /* we expect success, unless the parameter hasn't been initialized yet */
            if (getIntegerParam(addr, fn, &tmp) == asynSuccess)
                ctl_value = tmp;
        };

        write_param(p_pwrstate, ctl.amp_enable);
        write_param(p_opmode, ctl.mode_numeric);
        write_param(p_loopkp, ctl.pi_kp);
        write_param(p_loopti, ctl.pi_ti);
        write_param(p_testwaveperiod, ctl.cnt);
        write_param(p_testlim_a, ctl.limit_a);
        write_param(p_testlim_b, ctl.limit_b);
        write_param(p_pi_sp, ctl.pi_sp);
        write_param(p_dac, ctl.dac);
        /* FIXME: if someone writes into trigen, we'll error out continuously until
         * being restarted */
        write_param(p_trigen, ctl.trigger_enable);

        try {
            ctl.write_params();
        } catch (std::runtime_error &e) {
            const char *param_name;

            getParamName(function, &param_name);
            epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                "writeInt32:%s: %s", param_name, e.what());

            return asynError;
        }

        return asynSuccess;
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
        } catch (std::exception &e) {
            cantProceed("error creating rtmlamp: %s\n", e.what());
        }
    }

    static void registerRtmLamp(void)
    {
        iocshRegister(&initFuncDef, initCallFunc);
    }

    epicsExportRegistrar(registerRtmLamp);
}
