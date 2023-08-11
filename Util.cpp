#include "Util.h"

bool dup_quotes(std::wstring* str) {
    int count{ 0 };
    for (strIteratorIndex qt = { str->begin() }; qt.it < str->end(); qt.it++, qt.id++) {
        if (*qt.it == L'\"') {
            qt.it = str->insert(qt.it, L'\"');

            count++;

            if (qt.it < str->end() - 1)
                qt.it++;
        }
    }

    return count > 0;
}