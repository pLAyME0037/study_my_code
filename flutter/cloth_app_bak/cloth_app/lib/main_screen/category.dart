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
            body: Stack(
            children: [
                Positioned(
                bottom: 0,
                left: 0,
                child: Container( 
                    height: MediaQuery.of(context).size.height*1,
                    width: MediaQuery.of(context).size.width*0.4,
                    color: Colors.red.withAlpha(100),
                    )
                ),
                Positioned(
                    bottom: 0,
                    left: 0,
                    child: Container( 
                        height: MediaQuery.of(context).size.height*1,
                        width: MediaQuery.of(context).size.width*0.2,
                        color: Colors.blue.shade200,
                    )
                ),
             ])
        );
    }
}

Widget SideNavigator() {
    return Container( 
    );
}
