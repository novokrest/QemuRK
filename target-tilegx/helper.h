DEF_HELPER_2(exception, noreturn, env, i32)
DEF_HELPER_FLAGS_1(cntlz, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(cnttz, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(pcnt, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_1(revbits, TCG_CALL_NO_RWG_SE, i64, i64)
DEF_HELPER_FLAGS_3(shufflebytes, TCG_CALL_NO_RWG_SE, i64, i64, i64, i64)

DEF_HELPER_FLAGS_2(v1shl, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shru, TCG_CALL_NO_RWG_SE, i64, i64, i64)
DEF_HELPER_FLAGS_2(v1shrs, TCG_CALL_NO_RWG_SE, i64, i64, i64)
