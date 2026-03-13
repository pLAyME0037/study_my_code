import 'package:flutter/material.dart';

class CustomerHomeScreen extends StatefulWidget {
    const CustomerHomeScreen ({super.key});

    @override
    State<CustomerHomeScreen> createState() => _CustomerHomeScreenState();
}

class _CustomerHomeScreenState extends State<CustomerHomeScreen> {
    int _index = 1;
    final _page = const [
        Text("hello"),
        Text("hello1111kl"),
        Text("hello12wythgtyghgeashkggfTUY7U8JIFNBJHBGNVMRUKGHDJIHJWEG"),
        Text("hellofnrejlfbefregregregbnehjghhebhfbbvgfwwfqcdfwgdcwgfqdcgffcdfge"),
    ];

    @override
        Widget build(BuildContext context) {
            return Scaffold (
             body: _page[_index],
             bottomNavigationBar: BottomNavigationBar(
               currentIndex: _index,
                 onTap: (i) => setState(() => _index = i),
               items: [
                 BottomNavigationBarItem(icon: Icon(Icons.home), label: "home"), 
                 BottomNavigationBarItem(icon: Icon(Icons.search), label: "Categlory"),
                 BottomNavigationBarItem(icon: Icon(Icons.shop), label: "Story"),
                 BottomNavigationBarItem(icon: Icon(Icons.shopping_cart), label: "Cart"),
               ],
               selectedItemColor: Colors.black,
               unselectedItemColor: Colors.red));
       }
}
