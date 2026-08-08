typedef struct {
    const char *created_at;
    const char *updated_at;
} TimeStamp;

TimeStamp *timestamp_new(const char *created_at, const char *updated_at) {
    TimeStamp ts = {
        .created_at = created_at,
        .updated_at = updated_at,
    };
}

void timestamps_add(void) {
    timestamp_new(__TIMESTAMP__, "04-03-26.10:03:09-AM"
    );
}
