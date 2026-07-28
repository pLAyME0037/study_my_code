import 'package:cloth_app/auth/customer_login.dart';
import 'package:cloth_app/auth/customer_signup.dart';
import 'package:cloth_app/auth/supplier_login.dart';
import 'package:cloth_app/auth/supplier_signup.dart';
import 'package:cloth_app/main_screen/customer_home.dart';
import 'package:cloth_app/main_screen/supplier_home.dart';
import 'package:cloth_app/main_screen/welcome_screen.dart';
import 'package:cloth_app/provider/cart_provider.dart';
import 'package:cloth_app/provider/wish_provider.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'firebase_options.dart';

void main(List<String> args) async {
  WidgetsFlutterBinding.ensureInitialized();
  if (defaultTargetPlatform != TargetPlatform.linux) {
    await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);
  }
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => Cart()),
        ChangeNotifierProvider(create: (_) => Wish()),
      ],
      child: MaterialApp(
        initialRoute: "/welcome_screen",
        debugShowCheckedModeBanner: false,
        routes: {
          "/welcome_screen": (context) => const WelcomeScreen(),
          "/customer_home": (context) => const CustomerHomeScreen(),
          "/customer_login": (context) => const CustomerLogin(),
          "/customer_signup": (context) => const CustomerRegister(),
          "/supplier_home": (context) => const SupplierHomeScreen(),
          "/supplier_login": (context) => const SupplierLogin(),
          "/supplier_signup": (context) => const SupplierRegister(),
        }),
    );
  }
}
