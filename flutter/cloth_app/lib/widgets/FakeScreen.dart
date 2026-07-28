import 'package:cloth_app/minor_screen/search.dart';
import 'package:flutter/material.dart';

class FakeScreen extends StatefulWidget {
  const FakeScreen({Key? key}) : super(key: key);

  @override
  _FakeScreenState createState() => _FakeScreenState();
}

class _FakeScreenState extends State<FakeScreen> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap: () {
        Navigator.push(
            context,
            MaterialPageRoute(
                builder: (context) => const SearchScreen()));
      },
      child: Container(
        height: 35,
        decoration: BoxDecoration(
            border: Border.all(color: Colors.yellow, width: 1.4),
            borderRadius: BorderRadius.circular(15)),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            const Icon(Icons.search, color: Colors.grey),
            Text(
              'What are you looking for?',
              style: TextStyle(fontSize: 16, color: Colors.grey)),
            Container(
              height: 32,
              width: 75,
              decoration: BoxDecoration(
                  color: Colors.yellow,
                  borderRadius: BorderRadius.circular(10)),
              child: const Center(
                child: Text(
                  'search',
                  style: TextStyle(color: Colors.grey))),
            )
          ]),
      ),
    );
  }
}
