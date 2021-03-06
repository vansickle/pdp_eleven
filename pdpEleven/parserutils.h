
#ifndef PARSERUTILS_H
#define PARSERUTILS_H

#include "dictionary.h"
#include "asmConstant.h"

uint16_t parseAttributeInCommand(const char* param);

const char* prepareString(dict_t macros, const char* str, uint32_t address);

const char* pushIfMacro(dict_t macros, const char* str, uint16_t address);

int parseCommand(const char* str, CmdStructPtr cmd);

void pushExprToDict(const char* str);

char* dumpMacroName(const char* srcStr);

void pushMacroToDictionary(dict_t dictionary, const char* srcStr, uint16_t address);

int getValueFromDictionary(dict_t dictionary, const char* srcStr);

#endif // PARSERUTILS_H
