class API
{
  static const hostConnect = "http://192.168.18.21/api_clothes_store";
  static const hostConnectUser = "$hostConnect/user";
  static const hostConnectAdmin = "$hostConnect/admin";

  //signUp-Login user
  static const validateEmail = "$hostConnectUser/validate_email.php";
  static const signUp = "$hostConnectUser/signup.php";
  static const login = "$hostConnectUser/login.php";

  //login admin
  static const adminLogin = "$hostConnectAdmin/login.php";
}