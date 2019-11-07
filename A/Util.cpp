//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "Util.h"

std::regex const Util::VARIABLE_REGEX = std::regex("(a..z)(a..z|0..9|’)*");