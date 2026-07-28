import 'package:cloth_app/search_screen/searchApp.dart';
import 'package:flutter/material.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen ({super.key});

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  @override
  Widget build(BuildContext context) {
    return DefaultTabController (
      length: 9,
      child: Scaffold (
        appBar: AppBar (
          title: InkWell (
            onTap: () {
                Navigator.push (
                    context,
                    MaterialPageRoute(builder: (context) => const SearchScreen())
                );
            },
            child: Container(
                height: 35,
                decoration: BoxDecoration(
                    border: Border.all(color: Colors.yellow, width: 1.4),
                    borderRadius: BorderRadius.circular(15)),

                child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                    Icon(Icons.search, color: Colors.grey),
                    Text(
                        'What are you looking for?',
                        style: TextStyle(fontSize: 16, color: Colors.grey)),
                    Container(
                        height: 32,
                        width: 75,
                        decoration: BoxDecoration(
                            color: Colors.yellow,
                            borderRadius: BorderRadius.circular(10)),

                        child: Text(
                            'search',
                            style: TextStyle(color: Colors.grey)))
            ]))),
          bottom: TabBar(tabs: [
            RepeatedTab(label: "Men"),
            RepeatedTab(label: "Woman"),
            RepeatedTab(label: "Shoes"),
            RepeatedTab(label: "Bags"),
            RepeatedTab(label: "Eletronics"),
            RepeatedTab(label: "Accessories"),
            RepeatedTab(label: "Home & Garden"),
            RepeatedTab(label: "Kids"),
            RepeatedTab(label: "Beauty"),
          ],
          isScrollable: true)),
        backgroundColor: Colors.grey.shade300,
        body: TabBarView(children: [
          Center(child: Text("Man")),
          Center(child: Text("Woman")),
          Center(child: Text("Shoes")),
          Center(child: Text("Bags")),
          Center(child: Text("Eletronics")),
          Center(child: Text("Accessories")),
          Center(child: Text("Home & Garden")),
          Center(child: Text("Kids")),
          Center(child: Text("Beauty")),
        ])));
  }
}

class RepeatedTab extends StatelessWidget {
    final String label;
    const RepeatedTab({
        super.key,
        required this.label,
    });

    @override
    Widget build(BuildContext context) {
      return Tab(child: Text(label, style: TextStyle(color: Colors.grey.shade500,)));
    }
}
