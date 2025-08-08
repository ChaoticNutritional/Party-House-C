#include "input.h"
#include "inputcontext.h"

#define MAX_CONTEXT_STACK 5

/// @brief Context based stack to determine input actions
static InputContext* _contextStack[MAX_CONTEXT_STACK] = { 0 };
static int _contextTop = -1;

/// @brief 
/// @param ctx 
void inputPushContext(InputContext* ctx) {
    if (_contextTop < MAX_CONTEXT_STACK - 1) {
        _contextTop++;
        _contextStack[_contextTop] = ctx;
        if (ctx->onEnter) ctx->onEnter(ctx);
    }
}

/// @brief 
void inputPopContext() {
    if (_contextTop >= 0) {
        InputContext* ctx = _contextStack[_contextTop];
        if (ctx->onExit) ctx->onExit(ctx);
        _contextStack[_contextTop] = NULL;
        _contextTop--;
    }
}


/// @brief 
/// @return 
InputContext* inputGetCurrentContext() {
    if (_contextTop >= 0)
        return _contextStack[_contextTop];
    return NULL;
}