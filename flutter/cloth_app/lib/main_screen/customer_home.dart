import 'package:cloth_app/main_screen/cart.dart';
import 'package:cloth_app/main_screen/category.dart';
import 'package:cloth_app/main_screen/home.dart';
import 'package:cloth_app/main_screen/stores.dart';
import 'package:flutter/material.dart';

class CustomerHomeScreen extends StatefulWidget {
    const CustomerHomeScreen ({super.key});

    @override
    State<CustomerHomeScreen> createState() => _CustomerHomeScreenState();
}

class _CustomerHomeScreenState extends State<CustomerHomeScreen> {
    int _index = 0;
    final _page = const [
        HomeScreen(),
        CategoryScreen(),
        StoresScreen(),
        CartScreen(),
    ];

    @override
    Widget build(BuildContext context) {
      return Scaffold (
        body: _page[_index],
        bottomNavigationBar: BottomNavigationBar(
          type: BottomNavigationBarType.fixed,
          currentIndex: _index,
          onTap: (i) => setState(() => _index = i),
          items: [
            BottomNavigationBarItem(icon: Icon(Icons.home), label: "Home"), 
            BottomNavigationBarItem(icon: Icon(Icons.search), label: "Category"),
            BottomNavigationBarItem(icon: Icon(Icons.shop), label: "Stores"),
            BottomNavigationBarItem(icon: Icon(Icons.shopping_cart), label: "Cart"),
          ],
          selectedItemColor: Colors.black,
          unselectedItemColor: Colors.red)
      );
    }
}
