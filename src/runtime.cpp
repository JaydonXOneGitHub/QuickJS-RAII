#include "runtime.hpp"
#include "context.hpp"
#include <iostream>

using namespace QuickJS;

Runtime::Runtime() {
    this->rt = JS_NewRuntime();
}

Runtime::~Runtime() {
    this->runGarbageCollector();
    JS_FreeRuntime(this->rt);
}

JSRuntime* Runtime::getRuntime() const {
    return this->rt;
}

void Runtime::runGarbageCollector() const {
    JS_RunGC(this->rt);
}

void Runtime::executePendingJobs() const {
    JSContext* jobContext = nullptr;

    while (JS_IsJobPending(this->rt)) {
        int ret = JS_ExecutePendingJob(this->rt, &jobContext);

        if (ret < 0) {
            JSValue exception = JS_GetException(jobContext);
            JSValue message =
                JS_GetPropertyStr(jobContext, exception, "message");

            if (JS_IsString(message)) {
                const char* msg =
                    JS_ToCString(jobContext, message);

                std::string error = msg ? msg : "Unknown exception";

                if (msg) {
                    JS_FreeCString(jobContext, msg);
                }

                JS_FreeValue(jobContext, message);
                JS_FreeValue(jobContext, exception);

                throw Exception(error);
            }

            JS_FreeValue(jobContext, message);
            JS_FreeValue(jobContext, exception);

            throw Exception("JavaScript job failed");
        }
    }
}