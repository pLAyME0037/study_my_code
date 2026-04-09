import 'package:cloth_app/categories/MenCateg.dart';
import 'package:cloth_app/widgets/FakeScreen.dart';
import 'package:flutter/material.dart';

List<ItemData> items = [
    ItemData(label: 'man'          , isSelected: true),
    ItemData(label: 'women'        , isSelected: false),
    ItemData(label: 'accessories'  , isSelected: false),
    ItemData(label: 'electronics'  , isSelected: false),
    ItemData(label: 'shoes'        , isSelected: false),
    ItemData(label: 'home & garden', isSelected: false),
    ItemData(label: 'beauty'       , isSelected: false),
    ItemData(label: 'kids'         , isSelected: false),
    ItemData(label: 'bags'         , isSelected: false),
];

class CategoryScreen extends StatefulWidget {
    const CategoryScreen({super.key});

    @override
    State<CategoryScreen> createState() => _CategoryScreenState();
}

class _CategoryScreenState extends State<CategoryScreen> {
    final PageController _pc = PageController();

    @override
    void initState() {
        for (var e in items) {
            e.isSelected = false;
        }
        setState(() {
            items[0].isSelected = true;
        });

        super.initState();
    }

    @override
    Widget build(BuildContext context) {
      var size = MediaQuery.of(context).size;

      return Scaffold(
        appBar: AppBar(
          elevation: 0,
          backgroundColor: Colors.white,
          title: FakeScreen(),
        ),

        body: Stack(
          children: [
            Positioned(bottom: 0, left: 0, child: sideNavigator(size)),
            Positioned(bottom: 0, right: 0, child: cateView(size)),
          ],
        ),
      );
    }

    // Create sideNavigator
    Widget sideNavigator(Size size) {
      return GestureDetector(
          child: Container(
              height: size.height * 0.89,
              width: size.width * 0.2,
              color: Colors.red.shade200,
              child: ListView.builder(
                  itemCount: items.length,
                  itemBuilder: (context, index) {
                    return GestureDetector(
                        onTap: () {
                            // _pc.jumpToPage(index);
                            _pc.animateToPage(
                                index,
                                duration: Duration(milliseconds: 300),
                                curve: Curves.easeIn
                            );
                            setState(() {
                                items[index].isSelected = true;
                            });
                        },
                        child: Container(
                            height: 100,
                            color: items[index].isSelected == true ? Colors.white : Colors.grey.shade300,
                            child: Center(child: Text(items[index].label))
                        ),
                    );
                  },
              ),
          ),
      );
    }

    // Create cateView to view
    Widget cateView(Size size) {
      return Container(
        height: size.height * 0.89,
        width: size.width * 0.8,
        color: Colors.blue.shade200,
        child: PageView( 
            controller: _pc,
            onPageChanged: (value) {
                for (var element in items) {
                    element.isSelected = false;
                }
                setState(() {
                    items[value].isSelected = true;
                });
            },
            scrollDirection: Axis.vertical,
            children: [
                ManCateg(),
                // Center(child: Text("Man Category")),
                Center(child: Text("Woman Category")),
                Center(child: Text("Accessories Category")),
                Center(child: Text("Electronics Category")),
                Center(child: Text("Shoes Category")),
                Center(child: Text("Home && Garden Category")),
                Center(child: Text("Beauty Category")),
                Center(child: Text("Kids Category")),
                Center(child: Text("Bags Category")),
            ],
        ),
      );
    }
}

class ItemData {
    String label;
    bool isSelected;
    ItemData({
        required this.label,
        this.isSelected = false
    });
}
