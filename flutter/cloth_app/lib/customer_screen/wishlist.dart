import 'package:cloth_app/provider/wish_provider.dart';
import 'package:cloth_app/widgets/AppbarWidgets.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

class WishlistScreen extends StatelessWidget {
  const WishlistScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.grey.shade200,
      appBar: AppBar(
        elevation: 0,
        backgroundColor: Colors.white,
        leading: const AppbarBackButton(),
        title: const AppbarWidgets(title: 'My Wishlist'),
      ),
      body: Consumer<Wish>(
        builder: (context, wish, child) {
          if (wish.getWishItems.isEmpty) {
            return const Center(
              child: Text(
                'Your wishlist is empty!',
                style: TextStyle(fontSize: 26),
              ),
            );
          }
          return ListView.builder(
            itemCount: wish.count,
            itemBuilder: (context, index) {
              final product = wish.getWishItems[index];
              return ListTile(
                leading: SizedBox(
                  height: 60,
                  width: 60,
                  child: Image.network(product.imagesUrl.first),
                ),
                title: Text(product.name),
                subtitle: Text('\$${product.price.toStringAsFixed(2)}'),
                trailing: IconButton(
                  icon: const Icon(Icons.delete, color: Colors.red),
                  onPressed: () {
                    wish.removeItem(product);
                  },
                ),
              );
            },
          );
        },
      ),
    );
  }
}
