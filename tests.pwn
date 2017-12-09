#include "logger.inc"

main() {
    new str[128];
    str = "this \"contains\" quotes";
    printf("using test string: '%s'", str);
    printf("- escaped: '%s'", quote_escape(str));
    printf("- with _s: '%s'", _s("name", str));

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


    print("\n- end -");
}