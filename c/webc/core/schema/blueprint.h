#ifndef TIMESTAMP
#define TIMESTAMP

typedef struct {
    const char *created_at;
    const char *updated_at;
} TimeStamp;

void timestamps_add(void);
#endif // !TIMESTAMP
