//
// Created by Dima Zapolsky on 20/12/2018.
//

#ifndef LABA_SWAP_H
#define LABA_SWAP_H

template <typename Key>
void swap(Key &a, Key &b) {
    Key c = a;
    a = b;
    b = c;
}

#endif //LABA_SWAP_H
