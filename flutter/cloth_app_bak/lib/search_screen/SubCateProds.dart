import 'package:cloth_app/widgets/AppbarWidgets.dart';
import 'package:flutter/material.dart';

class SubcategProducts extends StatelessWidget {
  final String mainCategName;
  final String subCategName;
  const SubcategProducts ({
      Key? key,
      required this.subCategName,
      required this.mainCategName
  }): super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold (
      appBar: AppBar (
        leading: const AppbarBackButton(),
        elevation: 0,
        backgroundColor: Colors.white,
        title: AppbarTitle(title: subCategName),
      ),
      body: Center(
        child: Text(mainCategName),
      ),
    );
  }
}
