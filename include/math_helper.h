//
// Created by treyb on 2025-02-13.
//

#ifndef MATH_HELPER_H
#define MATH_HELPER_H

int round_to_int(double n) {
    n = n + 0.5 - (n<0);
    return static_cast<int>(n);
}

#endif //MATH_HELPER_H
