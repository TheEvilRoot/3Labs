#pragma once

#include <iostream>

const int ERR_SUCCEED = 0;
const int ERR_INVALID_VALUE = 1;
const int ERR_LOWER_BORDER_EXCEED = 2;
const int ERR_UPPER_BORDER_EXCEED = 3;

const char * localizeError(char error);

template <class T>
std::pair<char, T> enter(const char *message, T lower, T upper);

template <class T>
T handleInput(const char *message, T lower, T upper, const char * (*localize)(char) = &localizeError);

std::string enterString(const char *message, bool includeWhitespaces = true);