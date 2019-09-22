#pragma once
#include <switch.h>
#include <stratosphere.hpp>
#include <cstring>
#include <atomic>
#include "debug.hpp"

struct AesKey {
    u64 p1;
    u64 p2;
};

struct GenerateAesKekParams {
    AesKey key_source;
    u32 generation;
    u32 option;
};

class SplMitMService : public IMitmServiceObject {
    private:
        enum class CommandId {
            GenerateAesKek = 2,
            GenerateAesKey = 4,
        };  
    public:
        SplMitMService(std::shared_ptr<Service> s, u64 pid, sts::ncm::TitleId tid) : IMitmServiceObject(s, pid, tid) {
            LogFormat("SplMitMService pid: %" PRIu64 " tid: %" PRIx64, pid, tid);
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
        Result GenerateAesKek(Out<AesKey> out, GenerateAesKekParams p);
        Result GenerateAesKey(Out<AesKey> out, AesKey access_key, AesKey key_source);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MAKE_SERVICE_COMMAND_META(SplMitMService, GenerateAesKek),
            MAKE_SERVICE_COMMAND_META(SplMitMService, GenerateAesKey),
        };
};
