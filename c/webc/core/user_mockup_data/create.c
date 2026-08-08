#include <stdio.h>
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
    Timestamps ts;
} User;

User *user_new(User user) {
    user.id = 1;
    user.name = "hello kitty";
    user.username = "hellokitty1";
    user.email = "hellokitty1@gmail.com";
    user.profile_pic = "../../resources/image/user1.png";
    user.bio = "hello kitty my email is hellokitty1@gmail.com";
    user.password = "12345678";
    user.email_verified_at = get.current_time;
    user.last_login_at = get.current_time;
    user.is_active = true;
    user.is_soft_del = false;
    user.ts = timestamp_add();
}

int main(void) {

    return 0;
}
