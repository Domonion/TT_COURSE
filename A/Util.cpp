//
// Created by Artemiy.Kononov on 11/5/2019.
//

#include "Util.h"

Util::Util() : OPEN("("), CLOSE(")"), VARIABLE_REGEX("(a..z)(a..z|0..9|’)*") {}

Util const &Util::GetInstance() {
    return mine;
}
