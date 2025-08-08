#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef struct InputContext InputContext;

struct InputContext {
    void (*onEnter)(InputContext*);
    void (*onExit)(InputContext*);
    void* ctxData;
};

void inputPushContext(InputContext* ctx);
void inputPopContext();
InputContext* inputGetCurrentContext();

#ifdef __cplusplus
}
#endif