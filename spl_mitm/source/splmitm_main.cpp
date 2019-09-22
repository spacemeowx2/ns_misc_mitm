/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <malloc.h>

#include <switch.h>
#include <stratosphere.hpp>

#include "splmitm_service.hpp"

extern "C" {
    extern u32 __start__;

    u32 __nx_applet_type = AppletType_None;

    #define INNER_HEAP_SIZE 0x100000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char   nx_inner_heap[INNER_HEAP_SIZE];
    
    void __libnx_initheap(void);
    void __appInit(void);
    void __appExit(void);
}


void __libnx_initheap(void) {
    void*  addr = nx_inner_heap;
    size_t size = nx_inner_heap_size;

    /* Newlib */
    extern char* fake_heap_start;
    extern char* fake_heap_end;

    fake_heap_start = (char*)addr;
    fake_heap_end   = (char*)addr + size;
}

void __appInit(void) {
    Result rc;
    svcSleepThread(10000000000L);

    SetFirmwareVersionForLibnx();

    rc = smInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
    }
    rc = fsInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    }

    rc = fsdevMountSdmc();
    if (R_FAILED(rc)) {
        fatalSimple(rc);
    }

    LogFormat("__appInit done");
}

void __appExit(void) {
    /* Cleanup services. */
    fsdevUnmountAll();
    fsExit();
    smExit();
}

struct SplMitmManagerOptions {
    static const size_t PointerBufferSize = 0x1000;
    static const size_t MaxDomains = 0x10;
    static const size_t MaxDomainObjects = 0x100;
};
using SplMitmManager = WaitableManager<SplMitmManagerOptions>;

int main(int argc, char **argv)
{
    LogFormat("main");

    auto server_manager = new SplMitmManager(2);

    /* Create spl: mitm. */
    AddMitmServerToManager<SplMitMService>(server_manager, "spl:", 3);
    AddMitmServerToManager<SplMitMService>(server_manager, "spl:mig", 3);
    AddMitmServerToManager<SplMitMService>(server_manager, "spl:fs", 3);
    AddMitmServerToManager<SplMitMService>(server_manager, "spl:ssl", 3);
    AddMitmServerToManager<SplMitMService>(server_manager, "spl:es", 3);
    AddMitmServerToManager<SplMitMService>(server_manager, "spl:manu", 3);

    /* Loop forever, servicing our services. */
    server_manager->Process();

    return 0;
}
