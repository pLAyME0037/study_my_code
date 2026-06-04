import 'package:pos_app/models/user.dart';
import 'package:pos_app/models/user_preferences.dart';
import 'package:get/get.dart';

class CurrentUser extends GetxController
{
  Rx<User> _currentUser = User(0,'','','').obs;

  User get user => _currentUser.value;

  getUserInfo() async {
    User? getUserInfoFromLocalStorage = await RememberUserPrefs.readUserInfo();
    _currentUser.value = getUserInfoFromLocalStorage!;
  }
}
