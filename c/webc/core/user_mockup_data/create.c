#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "../../nob.h"
#include "../schema/blueprint.h"

typedef struct {
    int id;
    String_View name;
    String_View username;
    String_View email;
    String_View profile_pic;
    String_View bio;
    String_View password;
    String_View email_verified_at;
    String_View last_login_at;
    bool is_active;
    bool is_soft_del;
    TimeStamp ts;
} User;

tsa = timestamps_add();

User user = {
    .id                     = 1,
    .name.data              = "hello kitty",
    .username.data          = "hellokitty1",
    .email.data             = "hellokitty1@gmail.com",
    .profile_pic.data       = "../../resources/image/user1.png",
    .bio.data               = "hello kitty my email is hellokitty1@gmail.com",
    .password.data          = "12345678",
    .email_verified_at.data = __TIMESTAMP__,
    .last_login_at.data     = __TIMESTAMP__,
    .is_active              = true,
    .is_soft_del            = false,
    .ts                     = tsa,
};

User *user_new(User user) {
}

