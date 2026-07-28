import 'package:cloth_app/widgets/AppbarWidgets.dart';
import 'package:flutter/material.dart';

class Statics extends StatelessWidget {
  const Statics({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        elevation: 0,
        backgroundColor: Colors.white,
        leading: const AppbarBackButton(),
        title: const AppbarWidgets(
          title: 'Statics',
        ),
      ),
    );
  }
}
