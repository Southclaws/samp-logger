# samp-logger

This is a simple structured logger which provides a simple set of functions for logging and debugging.

## Installation

Simply install to your project:

```bash
sampctl package install Southclaws/samp-logger
```

Include in your code and begin using the library:

```pawn
#include <logger>
```

## Usage

To log an event:

```pawn
log("something happened!");
```

To log an event with values to give the event more context:

```pawn
log("player changed world",
    _s("name", playerName),
    _i("worldid", 4));
```

This would be printed as:

```bash
text="player changed world" name="Southclaws" worldid=4
```

Which means you can easily read the log. The description of the event is isolated from the parameters instead of being mixed up in a sentence such as `player Southclaws changed to world id 4`. This also means the log is now very easy to parse automatically and process in some way.

### Debugging

The library also includes a basic debug mechanism which uses the SVar system to facilitate named debug handlers which can be turned on and off at runtime.

The syntax is the same for `dbg` as `log` but with one extra parameter which describes the debug handler name:

```pawn
logger_debug("weapons", true); // activate the "weapons" debug handler
dbg("weapons", "player changed weapon",
    _i("weaponid", 31));
```

### Extending

The logging functions are just string concatenation functions. All `_s` and friends do is format the string into a `key=value` format and simply return the string. So it's easy to write your own field converters:

```pawn
stock _w(name[], weaponid) {
    new result[MAX_FIELD_NAME + 1 + MAX_FIELD_VALUE];
    new weaponName[32];
    GetWeaponName(weaponid, weaponName, 32);
    formatex(result, sizeof(result), "%s=\"%s\"", field, weaponName);
    return LOGGER_FIELD:result;
}
```

## Development

To run tests, simply build and run the package:

```bash
sampctl package run --forceBuild --forceEnsure
```
