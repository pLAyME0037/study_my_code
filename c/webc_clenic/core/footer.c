#include "../nob.h"

void render_page_footer(String_Builder *sb) {
#define OUT(buf, size) sb_append_buf(sb, buf, size);
#include "../auto_ctrl/cttochtml/footer.h"
#undef OUT
}

