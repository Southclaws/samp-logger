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

#if defined SAMP_LOGGER_COMPAT
forward LOGGER_FIELD:_i(const field[], value);
forward LOGGER_FIELD:_b(const field[], value);
forward LOGGER_FIELD:_x(const field[], value);
forward LOGGER_FIELD:_f(const field[], Float:value);
forward LOGGER_FIELD:_s(const field[], const value[]);
#else
forward LOGGER_FIELD:Logger_I(const field[], value);
forward LOGGER_FIELD:Logger_B(const field[], value);
forward LOGGER_FIELD:Logger_X(const field[], value);
forward LOGGER_FIELD:Logger_F(const field[], Float:value);
forward LOGGER_FIELD:Logger_F(const field[], const value[]);
#endif

static
	FieldBuffer[MAX_FIELD_LENGTH],
	EventBuffer[MAX_EVENT_LENGTH];

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
#if defined SAMP_LOGGER_COMPAT
stock log(const text[], LOGGER_FIELD:...) {
#else 
stock Logger_Log(const text[], LOGGER_FIELD:...) {
#endif
	new total = numargs();

	if (total == 1) {
		#if defined SAMP_LOGGER_COMPAT
		print(_:_s("text", text));
		#else 
		print(_:Logger_S("text", text));
		#endif
		return;
	}

	EventBuffer[0] = EOS;
	#if defined SAMP_LOGGER_COMPAT
	strcat(EventBuffer, _:_s("text", text));
	#else
	strcat(EventBuffer, _:Logger_S("text", text));
	#endif

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
#if defined SAMP_LOGGER_COMPAT
stock dbg(const handler[], const text[], LOGGER_FIELD:...) {
#else
stock Logger_Dbg(const handler[], const text[], LOGGER_FIELD:...) {
#endif
	if(GetSVarInt(handler) == 0) {
		return;
	}

	new total = numargs();

	if (total == 2) {
		#if defined SAMP_LOGGER_COMPAT
		print(_:_s("text", text));
		#else
		print(_:Logger_S("text", text));
		#endif
		return;
	}

	EventBuffer[0] = EOS;
	
	#if defined SAMP_LOGGER_COMPAT
	strcat(EventBuffer, _:_s("text", text));
	#else 
	strcat(EventBuffer, _:Logger_S("text", text));
	#endif

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

#if defined SAMP_LOGGER_COMPAT
stock err(const text[], LOGGER_FIELD:...) {
#else
stock Logger_Err(const text[], LOGGER_FIELD:...) {
#endif
	new total = numargs();

	if (total == 1) {
		#if defined SAMP_LOGGER_COMPAT
		print(_:_s("text", text));
		#else
		print(_:Logger_S("text", text));
		#endif
		return;
	}

	EventBuffer[0] = EOS;
	
	#if defined SAMP_LOGGER_COMPAT
	strcat(EventBuffer, _:_s("text", text));
	#else 
	strcat(EventBuffer, _:Logger_S("text", text));
	#endif

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
	PrintAmxBacktrace();
}

#if defined SAMP_LOGGER_COMPAT
stock fatal(const text[], LOGGER_FIELD:...) {
#else
stock Logger_Fatal(const text[], LOGGER_FIELD:...) {
#endif
	new total = numargs();

	if (total == 1) {
		#if defined SAMP_LOGGER_COMPAT
		print(_:_s("text", text));
		#else
		print(_:Logger_S("text", text));
		#endif
		return;
	}

	EventBuffer[0] = EOS;
	strcat(EventBuffer, text);

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
	PrintAmxBacktrace();

	// trigger a crash to escape the gamemode
	new File:f = fopen("nonexistentfile", io_read), tmp[1];
	fread(f, tmp);
	fclose(f);
}

#if defined SAMP_LOGGER_COMPAT
stock logger_debug(const handler[], bool:toggle) {
#else
stock Logger_Debug(const handler[], bool:toggle) {
#endif
	if(toggle) {
		SetSVarInt(handler, 1);
	} else {
		DeleteSVar(handler);
	}
	return 1;
}

// _i is a log field converter for integers, it takes a named integer and
// returns a log field representation of it.
#if defined SAMP_LOGGER_COMPAT
stock LOGGER_FIELD:_i(const field[], value) {
#else 
stock LOGGER_FIELD:Logger_I(const field[], value) {
#endif
	FieldBuffer[0] = EOS;
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%d", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// _b is a log field converter for integers, it takes a named integer and
// returns a log field representation of it in binary.
#if defined SAMP_LOGGER_COMPAT
stock LOGGER_FIELD:_b(const field[], value) {
#else
stock LOGGER_FIELD:Logger_B(const field[], value) {
#endif
	FieldBuffer[0] = EOS;
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%b", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// _x is a log field converter for integers, it takes a named integer and
// returns a log field representation of it in hexadecimal.
#if defined SAMP_LOGGER_COMPAT
stock LOGGER_FIELD:_x(const field[], value) {
#else
stock LOGGER_FIELD:Logger_X(const field[], value) {
#endif
	FieldBuffer[0] = EOS;
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%x", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// _f is a log field converter for floats, it takes a named float and
// returns a log field representation of it.
#if defined SAMP_LOGGER_COMPAT
stock LOGGER_FIELD:_f(const field[], Float:value) {
#else
stock LOGGER_FIELD:Logger_F(const field[], Float:value) {
#endif
	FieldBuffer[0] = EOS;
	format(FieldBuffer, sizeof(FieldBuffer), "%s=%f", field, value);
	return LOGGER_FIELD:FieldBuffer;
}

// _i is a log field converter for strings, it takes a named string and
// returns a log field representation of it.
#if defined SAMP_LOGGER_COMPAT
stock LOGGER_FIELD:_s(const field[], const value[]) {
#else
stock LOGGER_FIELD:Logger_S(const field[], const value[]) {
#endif
	new quoted[MAX_FIELD_VALUE];
	
	#if defined SAMP_LOGGER_COMPAT
	quote_escape(value, MAX_FIELD_VALUE, quoted, MAX_FIELD_VALUE);
	#else
	Logger_EscapeQuote(value, MAX_FIELD_VALUE, quoted, MAX_FIELD_VALUE);
	#endif

	FieldBuffer[0] = EOS;
	format(FieldBuffer, sizeof(FieldBuffer), "%s=\"%s\"", field, quoted);
	return LOGGER_FIELD:FieldBuffer;
}

// quote_escape returns a copy of value with all quotes escaped with backslashes
#if defined SAMP_LOGGER_COMPAT
stock quote_escape(const input[], inputLen, output[], outputLen) {
#else
stock Logger_EscapeQuote(const input[], inputLen, output[], outputLen) {
#endif
	new j;

	for(new i; input[i] != EOS && i < inputLen && j < (outputLen - 1); ++i) {
		if(input[i] == '"') {
			output[j++] = '\\';
		}
		output[j++] = input[i];
	}
	output[j] = EOS;
}
