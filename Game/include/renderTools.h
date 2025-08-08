#pragma once
#include "baseTypes.h"
#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

    void UpdateWindowSize(Application* app);
    uint32_t GetWindowHeight();
    uint32_t GetWindowWidth();

#ifdef __cplusplus
}
#endif
