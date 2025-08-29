#ifndef TYPE_H
#define TYPE_H

typedef bool (*Compare)(const void *, const void *);

typedef bool (*UnaryPred)(const void *);

typedef bool (*BinaryPred)(const void *, const void *);

#endif