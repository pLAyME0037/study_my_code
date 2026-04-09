import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

// class SearchScreen extends StatefulWidget {
//     const SearchScreen ({super.key});
//
//     @override
//     State<SearchScreen> createState() => _SearchScreenState();
// }
//
// class _SearchScreenState extends State<SearchScreen> {
//     @override
//         Widget build(BuildContext context) {
//             return Scaffold (
//
//             );
//        }
// }

class SearchScreen extends StatelessWidget {
    const SearchScreen ({super.key});
    
    @override
    Widget build(BuildContext context) {
        return Scaffold (
          appBar: AppBar(
              elevation: 0,
              backgroundColor: Colors.white,
              leading: IconButton(
                  icon: Icon(Icons.arrow_back_ios_new),
                  onPressed: () {
                      Navigator.pop(context);
                  }),
              title: CupertinoSearchTextField()));
    }
}

