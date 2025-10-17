#pragma once
#include "baseTypes.h"
#include "framework.h"

#ifdef __cplusplus
extern "C" {
#endif

    void UpdateWindowSize(Application* app);
    uint32_t GetWindowHeight(void);
    uint32_t GetWindowWidth(void);

#ifdef __cplusplus
}
#endif
