#ifndef KEYBOARD_H
#define KEYBOARD_H

#define STORE_TAG_FILENAME "/mtd0/mpos/storeTag.dat"

enum {

    BUFVALUE_ENUM = 0x01,
    VALUE_ENUM,
    STRING_ENUM,
    STORTSTRING_ENUM,
    KEYBOARD_END_ENUM = 0xff
};

#endif

