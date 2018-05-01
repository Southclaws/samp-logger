#include "logger.inc"

main() {
    new str[256];
    str = "this \"contains\" quotes and is a very long string because there was a bug discovered a while back that caused a crash if the parameter to this function was long so this ensures that bug is fixed.";
    printf("using test string: '%s'", str);
    new quoted[256];
    quote_escape(str, 256, quoted, 256);
    printf("- escaped: '%s'", quoted);
    printf("- with _s: '%s'", _:_s("name", str));

    print("\nsome log() calls\n");

    log("an event has happened");

    log("an event has happened with values",
        _i("worldid", 4),
        _f("health", 64.5),
        _s("message", "tim said \"hello\".")
    );

    print("\nsome dbg() calls\n");

    logger_debug("mydebugger", true);
    dbg("mydebugger", "an event has happened");

    dbg("mydebugger", "an event has happened with values",
        _i("worldid", 4),
        _f("health", 64.5),
        _s("message", "tim said \"hello\".")
    );

    print("this one should not print");
    logger_debug("mydebugger", false);
    dbg("mydebugger", "something happened");

    print("\nsome err() calls\n");

    err("an error has happened");

    err("an error has happened with values",
        _i("worldid", 4),
        _f("health", 64.5),
        _s("message", "tim said \"hello\".")
    );

    print("\n- end -");
}