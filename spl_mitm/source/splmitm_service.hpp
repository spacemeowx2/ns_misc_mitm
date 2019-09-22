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

class SplMitMService : public IMitmServiceObject {
    private:
        enum class CommandId {
            GenerateAesKek = 2,
            GenerateAesKey = 4,
        };  
    public:
        SplMitMService(std::shared_ptr<Service> s, u64 pid, sts::ncm::TitleId tid) : IMitmServiceObject(s, pid, tid) {
            /* ... */
        }
        
        static bool ShouldMitm(u64 pid, sts::ncm::TitleId tid) {
            LogFormat("should_mitm pid: %" PRIu64 " tid: %" PRIx64, pid, tid);
            return tid == sts::ncm::TitleId::Ldn;
        }
        
        static void PostProcess(IMitmServiceObject *obj, IpcResponseContext *ctx) {};
    protected:
        /* Overridden commands. */
        Result GenerateAesKek(Out<AesKey> out, AesKey key_source, u32 generation, u32 option);
        Result GenerateAesKey(Out<AesKey> out, AesKey access_key, AesKey key_source);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MAKE_SERVICE_COMMAND_META(SplMitMService, GenerateAesKek),
            MAKE_SERVICE_COMMAND_META(SplMitMService, GenerateAesKey),
        };
};
