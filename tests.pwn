#include "logger.inc"

main() {
    new str[256];
    str = "this \"contains\" quotes and is a very long string because there was a bug discovered a while back that caused a crash if the parameter to this function was long so this ensures that bug is fixed.";
    printf("using test string: '%s'", str);
    new quoted[256];
    Logger_EscapeQuote(str, 256, quoted, 256);
    printf("- escaped: '%s'", quoted);
    printf("- with _s: '%s'", _:Logger_S("name", str));

    print("\nsome log() calls\n");

    Logger_Log("an event has happened");

    Logger_Log("an event has happened with values",
        Logger_I("worldid", 4),
        Logger_F("health", 64.5),
        Logger_S("message", "tim said \"hello\".")
    );

    print("\nsome dbg() calls\n");

    Logger_ToggleDebug("mydebugger", true);
    Logger_Dbg("mydebugger", "an event has happened");

    Logger_Dbg("mydebugger", "an event has happened with values",
        Logger_I("worldid", 4),
        Logger_F("health", 64.5),
        Logger_S("message", "tim said \"hello\".")
    );

    print("this one should not print");
    Logger_ToggleDebug("mydebugger", false);
    Logger_Dbg("mydebugger", "something happened");

    print("\nsome err() calls\n");

    Logger_Err("an error has happened");

    Logger_Err("an error has happened with values",
        Logger_I("worldid", 4),
        Logger_F("health", 64.5),
        Logger_S("message", "tim said \"hello\".")
    );

    print("\n- end -");
}