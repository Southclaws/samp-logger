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
	#define MAX_EVENT_FIELDS (8)
#endif

#if !defined MAX_FIELD_NAME
	#define MAX_FIELD_NAME (16)
#endif

#if !defined MAX_FIELD_VALUE
	#define MAX_FIELD_VALUE (256)
#endif

#define MAX_FIELD_LENGTH	(MAX_FIELD_NAME + 3 + MAX_FIELD_VALUE)
#define MAX_EVENT_LENGTH	(MAX_EVENT_FIELDS * MAX_FIELD_LENGTH)

forward LOGGER_FIELD:_i(field[], value);
forward LOGGER_FIELD:_f(field[], Float:value);
forward LOGGER_FIELD:_s(field[], value[]);

// log is a structured log event printer, it takes a text parameter which describes the event
// followed by zero or more `LOGGER_FIELD` strings which are generated using `_i`, `_f` and `_s`.
// example:
//   log("an event has happened with values",
//       _i("worldid", 4),
//       _f("health", 64.5),
//       _s("message", "tim said \"hello\".")
//   );
// this would be formatted as:
// text="an event has happened with values" worldid=4 health=64.500000 message="tim said \"hello\"."
// quotes in messages are escaped in order to make parsing the logs easier.
stock log(text[], LOGGER_FIELD:...) {
	new total = numargs();

	if (total == 1) {
		print(_:_s("text", text));
		return;
	}

	new result[MAX_EVENT_LENGTH];
	strcat(result, _:_s("text", text));

	for(new arg = 1; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(result, " ");
				strcat(result, field);
				break;
			}
		}
	}

	print(result);
}

// dbg is a conditional version of log, it behaves the same but with one extra parameter which is
// the name of the debug handler. Internally, this is just an SVar and simply checks if the value
// is non-zero before continuing the print the event.
stock dbg(handler[], text[], LOGGER_FIELD:...) {
	if(GetSVarInt(handler) == 0) {
		return;
	}

	new total = numargs();

	if (total == 2) {
		print(_:_s("text", text));
		return;
	}

	new result[MAX_EVENT_LENGTH];
	strcat(result, _:_s("text", text));

	for(new arg = 2; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(result, " ");
				strcat(result, field);
				break;
			}
		}
	}

	// todo: process crashdetect backtrace to grab call site

	print(result);
}

stock err(text[], LOGGER_FIELD:...) {
	new total = numargs();

	if (total == 1) {
		print(_:_s("text", text));
		return;
	}

	new result[MAX_EVENT_LENGTH];
	strcat(result, _:_s("text", text));

	for(new arg = 1; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(result, " ");
				strcat(result, field);
				break;
			}
		}
	}

	print(result);
	PrintAmxBacktrace();
}

stock fatal(text[], LOGGER_FIELD:...) {
	new total = numargs();

	if (total == 1) {
		print(_:_s("text", text));
		return;
	}

	new result[MAX_EVENT_LENGTH];
	strcat(result, text);

	for(new arg = 1; arg < total && arg < MAX_EVENT_FIELDS; ++arg) {
		new field[MAX_FIELD_LENGTH];
		for(new ch; ch < MAX_FIELD_LENGTH; ++ch) {
			field[ch] = getarg(arg, ch);
			if(field[ch] == EOS) {
				strcat(result, " ");
				strcat(result, field);
				break;
			}
		}
	}

	print(result);
	PrintAmxBacktrace();

	// trigger a crash to escape the gamemode
	new File:f = fopen("nonexistentfile", io_read), tmp[1];
	fread(f, tmp);
	fclose(f);
}

stock logger_debug(handler[], bool:toggle) {
	if(toggle) {
		SetSVarInt(handler, 1);
	} else {
		DeleteSVar(handler);
	}
	return 1;
}

// _i is a log field converter for integers, it takes a named integer and
// returns a log field representation of it.
stock LOGGER_FIELD:_i(field[], value) {
	new result[MAX_FIELD_NAME + 1 + 12];
	format(result, sizeof(result), "%s=%d", field, value);
	return LOGGER_FIELD:result;
}

// _f is a log field converter for floats, it takes a named float and
// returns a log field representation of it.
stock LOGGER_FIELD:_f(field[], Float:value) {
	new result[MAX_FIELD_NAME + 1 + 64];
	format(result, sizeof(result), "%s=%f", field, value);
	return LOGGER_FIELD:result;
}

// _i is a log field converter for strings, it takes a named string and
// returns a log field representation of it.
stock LOGGER_FIELD:_s(field[], value[]) {
	new quoted[MAX_FIELD_VALUE];
	quote_escape(value, MAX_FIELD_VALUE, quoted, MAX_FIELD_VALUE);

	new result[MAX_FIELD_NAME + 3 + MAX_FIELD_VALUE];
	format(result, sizeof(result), "%s=\"%s\"", field, quoted);
	return LOGGER_FIELD:result;
}

// quote_escape returns a copy of value with all quotes escaped with backslashes
stock quote_escape(input[], inputLen, output[], outputLen) {
	new j;

	for(new i; input[i] != EOS && i < inputLen && j < (outputLen - 1); ++i) {
		if(input[i] == '"') {
			output[j++] = '\\';
		}
		output[j++] = input[i];
	}
	output[j] = EOS;
}
