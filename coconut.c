/**
 * main.c
 * The program.
 *
 * @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define REPL_TOKEN "> "

#ifdef __APPLE__
	#define __COMPUTER__
	#include <stdio.h>
#endif

typedef struct {
	char *array;
	size_t size;
} variables;


// ******************************
// **       Misc Helpers       **
// ******************************

char* strupper(char *str) {
	char *newstr, *p;
	p = newstr = strdup(str);
	while (*p++ = toupper(*p));

	return newstr;
}

char* substr(const char *str, size_t begin, size_t len) {
	if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin + len)) {
		return NULL;
	}

	return strndup(str + begin, len);
}


// ******************************
// **       REPL Helpers       **
// ******************************

/**
 * Raw print function.
 *
 * @param string What should be printed.
 */
void repl_print(const char *string) {
	#ifdef __COMPUTER__
		printf("%s", string);
	#endif
}

/**
 * Gets the string from the REPL.
 *
 * @param string The string to write to.
 */
void repl_gets(char *string) {
	#ifdef __COMPUTER__
		unsigned int pos = 0;

		while (true) {
			// Get character.
			char c = fgetc(stdin);

			// Check if it was a newline.
			if (c == '\n') {
				// NULL-terminate the string.
				string[pos] = '\0';
				break;
			}

			// Append character.
			string[pos] = c;
			pos++;
		}
	#endif
}


// ******************************
// **       Interpreter        **
// ******************************

void basic_let() {
	// Yeah, implement variables...
}

/**
 * Implementation of BASIC's PRINT.
 *
 * @param token The token from strtok().
 * @param args The arguments string.
 */
void basic_print(char *token, char *args) {
	bool contains_quote = false;
	token = strtok(args, ",");

	while (token != NULL) {
		// Ignore space after the comma.
		if (token[0] == ' ') {
			token = strdup(token + 1);
		}

		// Parse the arguments.
		if (token[0] == '"' && token[strlen(token) - 1] == '"') {
			// A normal string.
			repl_print(strndup(token + 1, strlen(token) - 2));
		} else if (token[0] == '"') {
			// Start of a string that contains a ",".
			contains_quote = true;
			repl_print(strdup(token + 1));
		} else if (contains_quote && token[strlen(token) - 1] != '"') {
			// Middle of a string that contains a ",".
			repl_print(",");
			repl_print(token);
			repl_print(",");
		} else if (token[strlen(token) - 1] == '"') {
			// End of a string.
			contains_quote = false;

			repl_print(",");
			repl_print(strndup(token, strlen(token) - 1));
		} else {
			// Other stuff.
			repl_print(token);
		}

		// Go for the next one.
		token = strtok(NULL, ",");
	}

	// Print the last newline.
	repl_print("\n");
}

/**
 * Interpret a line of code.
 *
 * @param line A line to interpret.
 * @param program Is it a program line or just a REPL command.
 */
void interpret_line(const char *line, bool program) {
	// Backup the line string since strtok fucks it.
	char cline[256];
	strcpy(cline, line);

	// Split by whitespace.
	char *token = strupper(strtok(cline, " "));

	// Check for the first keyword.
	if (strcmp(token, "PRINT") == 0) {
		if (strtok(NULL, " ") == NULL) {
			// Error: No arguments entered.
			repl_print("Error: Not enough arguments.\n");
			return;
		}

		// Get only the arguments.
		char *args = strdup(line + 6);
		basic_print(token, args);
	} else {
		// Error: Unknown command.
		repl_print("Error: Unknown command.");
	}
}


// ******************************
// **       Main Program       **
// ******************************

void repl() {
	while (true) {
		// Print the token.
		repl_print(REPL_TOKEN);

		// Get line and interpret.
		char line[256];
		repl_gets(line);
		interpret_line(line, false);
	}
}

int main(int argc, char *argv[]) {
	repl();
	return 0;
}
