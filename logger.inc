// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_logger
	#undef _inc_logger
#endif
// custom include-guard to ensure we don't duplicate
#if defined _logger_included
	#endinput
#endif
#define _logger_included

#include <a_samp>
#include <crashdetect>


// An event represents a single log event
// a log event contains one or more fields
// a field is composed of a name, an equals sign and a value
// if a value is a string, it is enclosed in quotes
// if a value is a string that contains quotes, they are escaped

#if !defined MAX_EVENT_FIELDS
	#define MAX_EVENT_FIELDS (32)
#endif

#if !defined MAX_FIELD_NAME
	#define MAX_FIELD_NAME (16)
#endif

#if !defined MAX_FIELD_VALUE
	#define MAX_FIELD_VALUE (256)
#endif

#define MAX_FIELD_LENGTH	(MAX_FIELD_NAME + 3 + MAX_FIELD_VALUE)
#define MAX_EVENT_LENGTH	(MAX_EVENT_FIELDS * MAX_FIELD_LENGTH)

forward LOGGER_FIELD:Logger_I(const field[], value);
forward LOGGER_FIELD:Logger_B(const field[], value);
forward LOGGER_FIELD:Logger_X(const field[], value);
forward LOGGER_FIELD:Logger_F(const field[], Float:value);
forward LOGGER_FIELD:Logger_S(const field[], const value[]);

static
	FieldBuffer[MAX_FIELD_LENGTH],
	EventBuffer[MAX_EVENT_LENGTH];

// log is a structured log event printer, it takes a text parameter which describes the event
// followed by zero or more `LOGGER_FIELD` strings which are generated using `Logger_I`, `Logger_F` and `Logger_S`.
// example:
//   Logger_Log("an event has happened with values",
//       Logger_I("worldid", 4),
//       Logger_F("health", 64.5),
//       Logger_S("message", "tim said \"hello\".")
//   );
// this would be formatted as:
// text="an event has happened with values" worldid=4 health=64.500000 message="tim said \"hello\"."
// quotes in messages are escaped in order to make parsing the logs easier.
stock Logger_Log(const text[], LOGGER_FIELD:...) {
	new total = numargs();

	format(EventBuffer, sizeof EventBuffer, "lvl=\"info\" %s",
		_:Logger_S("msg", text)
	);

	if (total == 1) {
		print(EventBuffer);
		return;
	}

	for(new arg = 1; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(EventBuffer, " ");
				strcat(EventBuffer, field);
				break;
			}
		}
	}

	print(EventBuffer);
}

// dbg is a conditional version of log, it behaves the same but with one extra parameter which is
// the name of the debug handler. Internally, this is just an SVar and simply checks if the value
// is non-zero before continuing the print the event.
stock Logger_Dbg(const handler[], const text[], LOGGER_FIELD:...) {
	if(GetSVarInt(handler) == 0) {
		return;
	}

	new total = numargs();

	format(EventBuffer, sizeof EventBuffer, "lvl=\"debug\" %s",
		_:Logger_S("msg", text)
	);

	if (total == 2) {
		print(EventBuffer);
		return;
	}

	for(new arg = 2; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(EventBuffer, " ");
				strcat(EventBuffer, field);
				break;
			}
		}
	}

	// todo: process crashdetect backtrace to grab call site

	print(EventBuffer);
}

stock Logger_Err(const text[], LOGGER_FIELD:...) {
	new total = numargs();

	format(EventBuffer, sizeof EventBuffer, "lvl=\"error\" %s",
		_:Logger_S("msg", text)
	);

	if (total == 1) {
		_PrintBufferAmxBacktrace();
		return;
	}

	for(new arg = 1; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(EventBuffer, " ");
				strcat(EventBuffer, field);
				break;
			}
		}
	}

	_PrintBufferAmxBacktrace();
}

stock Logger_Fatal(const text[], LOGGER_FIELD:...) {
	new total = numargs();

	format(EventBuffer, sizeof EventBuffer, "lvl=\"fatal\" %s",
		_:Logger_S("msg", text)
	);

	if (total == 1) {
		_PrintBufferAmxBacktrace();
		return;
	}

	for(new arg = 1; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(EventBuffer, " ");
				strcat(EventBuffer, field);
				break;
			}
		}
	}

	
	// trigger a crash to escape the gamemode
	_PrintBufferAmxBacktrace(true);
}

static stock _PrintBufferAmxBacktrace(bool:crash = false) {
	print(EventBuffer);
	PrintAmxBacktrace();

	if(crash) {
		new File:f = fopen("nonexistentfile", io_read), tmp[1];
		fread(f, tmp);
		fclose(f);
	}
}

stock Logger_ToggleDebug(const handler[], bool:toggle) {
	if(toggle) {
		SetSVarInt(handler, 1);
	} else {
		DeleteSVar(handler);
	}
	return 1;
}

// Logger_I is a log field converter for integers, it takes a named integer and
// returns a log field representation of it.
stock LOGGER_FIELD:Logger_I(const field[], value) {
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%d", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// Logger_B is a log field converter for integers, it takes a named integer and
// returns a log field representation of it in binary.
stock LOGGER_FIELD:Logger_B(const field[], value) {
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%b", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// Logger_X is a log field converter for integers, it takes a named integer and
// returns a log field representation of it in hexadecimal.
stock LOGGER_FIELD:Logger_X(const field[], value) {
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%x", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// Logger_F is a log field converter for floats, it takes a named float and
// returns a log field representation of it.
stock LOGGER_FIELD:Logger_F(const field[], Float:value) {
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%f", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// Logger_S is a log field converter for strings, it takes a named string and
// returns a log field representation of it.
stock LOGGER_FIELD:Logger_S(const field[], const value[]) {
	new quoted[MAX_FIELD_VALUE];
	Logger_EscapeQuote(value, MAX_FIELD_VALUE, quoted, MAX_FIELD_VALUE);
	format(FieldBuffer, sizeof(FieldBuffer), "%s=\"%s\"", field, quoted);
	return LOGGER_FIELD:FieldBuffer;
}

// Logger_A is a log field converter for arrays, it takes a named array and
// returns a log field representation of it where each cell is printed.
stock LOGGER_FIELD:Logger_A(const field[], const value[], len = sizeof value) {
	new array[MAX_FIELD_VALUE];
	format(array, sizeof array, "%d", array, value[0]);
	for(new i = 1; i < len; i++) {
		format(array, sizeof array, "%s, %d", array, value[i]);
	}
	format(FieldBuffer, sizeof(FieldBuffer), "%s=\"[%s]\"", field, array);
	return LOGGER_FIELD:FieldBuffer;
}

// Logger_P is a log field converter for player names, it takes a player ID and
// returns a log field with the player's name.
stock LOGGER_FIELD:Logger_P(playerid) {
	if(!IsPlayerConnected(playerid)) {
		format(FieldBuffer, sizeof(FieldBuffer), "player=\"(disconnected)\"");
	} else {
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);
		format(FieldBuffer, sizeof(FieldBuffer), "player=\"%s\"", name);
	}
	return LOGGER_FIELD:FieldBuffer;
}


// quote_escape returns a copy of value with all quotes escaped with backslashes
stock Logger_EscapeQuote(const input[], inputLen, output[], outputLen) {
	new j;

	for(new i; input[i] != EOS && i < inputLen && j < (outputLen - 1); ++i) {
		if(input[i] == '"') {
			output[j++] = '\\';
		}
		output[j++] = input[i];
	}
	output[j] = EOS;
}
