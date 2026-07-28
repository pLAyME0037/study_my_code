import 'package:pos_app/screens/authentication/login_screen.dart';
import 'package:pos_app/screens/fragments/dashboard_of_fragments.dart';
import 'package:pos_app/models/user_preferences.dart';
import 'package:flutter/material.dart';
import 'package:get/get.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return GetMaterialApp(
      title: 'Clothes App',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.purple,
      ),
      home: FutureBuilder(
        future: RememberUserPrefs.readUserInfo(),
        builder: (context, dataSnapShot) {
          if(dataSnapShot.data == null) {
            return LoginScreen();
          }
          else {
            return DashboardOfFragments();
          }
        },
      ),
    );
  }
}
