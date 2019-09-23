#pragma once
#include <switch.h>
#include <stratosphere.hpp>
#include <cstring>
#include <atomic>
#include "debug.hpp"

struct ConnectParams {
    u8 inner[0x84];
};

class WlanMitMService : public IMitmServiceObject {
    private:
        enum class CommandId {
            Connect = 11,
        };  
    public:
        WlanMitMService(std::shared_ptr<Service> s, u64 pid, sts::ncm::TitleId tid) : IMitmServiceObject(s, pid, tid) {
            LogFormat("WlanMitMService pid: %" PRIu64 " tid: %" PRIx64, pid, tid);
        }
        
        static bool ShouldMitm(u64 pid, sts::ncm::TitleId tid) {
            LogFormat("should_mitm pid: %" PRIu64 " tid: %" PRIx64, pid, tid);
            return tid == sts::ncm::TitleId::Ldn;
        }
        
        static void PostProcess(IMitmServiceObject *obj, IpcResponseContext *ctx) {
            LogFormat("PostProcess cmdid %d rc %x", ctx->cmd_id, ctx->rc);
            LogHex(armGetTls(), 0x100);
        };
    protected:
        /* Overridden commands. */
        Result Connect(ConnectParams p);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MAKE_SERVICE_COMMAND_META(WlanMitMService, Connect),
        };
};
