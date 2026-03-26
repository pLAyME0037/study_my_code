import 'package:cloth_app/widgets/FakeScreen.dart';
import 'package:flutter/material.dart';

class CategoryScreen extends StatefulWidget {
    const CategoryScreen ({super.key});

    @override
    State<CategoryScreen> createState() => _CategoryScreenState();
}

class _CategoryScreenState extends State<CategoryScreen> {
    @override
        Widget build(BuildContext context) {
            return Scaffold (
                appBar: AppBar(
                    backgroundColor: Colors.white,
                    title: FakeScreen(),
                ),
            );
       }
}
