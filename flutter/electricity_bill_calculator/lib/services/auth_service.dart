import '../models/user.dart';

class AuthService {
  static final AuthService _instance = AuthService._();
  factory AuthService() => _instance;
  AuthService._();

  final Map<String, User> _users = {};
  User? _currentUser;

  User? get currentUser => _currentUser;

  bool isUsernameTaken(String username) => _users.containsKey(username);

  bool isEmailTaken(String email) =>
      _users.values.any((u) => u.email == email);

  String? signup(String username, String email, String password) {
    if (_users.containsKey(username)) return 'Username already exists';
    if (_users.values.any((u) => u.email == email)) {
      return 'Email already registered';
    }
    _users[username] = User(
      username: username,
      email: email,
      password: password,
    );
    return null;
  }

  String? login(String usernameOrEmail, String password) {
    User? user;
    for (final u in _users.values) {
      if (u.username == usernameOrEmail || u.email == usernameOrEmail) {
        user = u;
        break;
      }
    }
    if (user == null) return 'User not found';
    if (user.password != password) return 'Wrong password';
    _currentUser = user;
    return null;
  }

  void logout() => _currentUser = null;
}
