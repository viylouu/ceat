#include "engine.h"
#include "../cutil.h"

eaa_state eaa_s;

void
eaa_engine_init(
    void
    ) {
    ma_result res;
    res = ma_engine_init(NULL, &eaa_s.eng);
    eat_assert(res == MA_SUCCESS, "failed to init miniaudio engine");
}

void
eaa_engine_stop(
    void
    ) {
    ma_engine_uninit(&eaa_s.eng);
}
