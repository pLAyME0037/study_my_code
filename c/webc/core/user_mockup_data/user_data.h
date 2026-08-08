typedef struct {
    const char *name;
    const char *email;
    const char *profile_pic;
} User;

static inline User User_data(void) {
    User u = {
        .name = "hello kitty",
        .email = "hellokitty1@gmail.com",
        .profile_pic = "resource/image/user1.png",
    };
    return u;
}
