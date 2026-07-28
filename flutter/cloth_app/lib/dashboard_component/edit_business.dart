import 'package:cloth_app/widgets/AppbarWidgets.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';

class EditBusiness extends StatefulWidget {
  const EditBusiness({Key? key}) : super(key: key);

  @override
  _EditBusinessState createState() => _EditBusinessState();
}

class _EditBusinessState extends State<EditBusiness> {
  final GlobalKey<FormState> _formKey = GlobalKey<FormState>();
  late String storeName;
  late String storeLogo;

  @override
  Widget build(BuildContext context) {
    CollectionReference suppliers =
        FirebaseFirestore.instance.collection('suppliers');
    return Scaffold(
      backgroundColor: Colors.grey.shade200,
      appBar: AppBar(
        elevation: 0,
        backgroundColor: Colors.white,
        leading: const AppbarBackButton(),
        title: const AppbarWidgets(title: 'Edit Business'),
      ),
      body: FutureBuilder<DocumentSnapshot>(
        future: suppliers.doc(FirebaseAuth.instance.currentUser!.uid).get(),
        builder:
            (BuildContext context, AsyncSnapshot<DocumentSnapshot> snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return const Center(child: CircularProgressIndicator());
          }
          if (snapshot.hasError) {
            return const Text('Something went wrong');
          }
          if (!snapshot.data!.exists) {
            return const Text('Store not found');
          }

          Map<String, dynamic> data =
              snapshot.data!.data() as Map<String, dynamic>;
          storeName = data['storename'] ?? '';
          storeLogo = data['storelogo'] ?? '';

          return Padding(
            padding: const EdgeInsets.all(16.0),
            child: Form(
              key: _formKey,
              child: Column(
                children: [
                  TextFormField(
                    initialValue: storeName,
                    decoration: const InputDecoration(labelText: 'Store Name'),
                    validator: (value) {
                      if (value!.isEmpty) return 'Enter store name';
                      return null;
                    },
                    onSaved: (value) => storeName = value!,
                  ),
                  const SizedBox(height: 20),
                  TextFormField(
                    initialValue: storeLogo,
                    decoration:
                        const InputDecoration(labelText: 'Store Logo URL'),
                    onSaved: (value) => storeLogo = value!,
                  ),
                  const SizedBox(height: 30),
                  ElevatedButton(
                    onPressed: () async {
                      if (_formKey.currentState!.validate()) {
                        _formKey.currentState!.save();
                        await suppliers
                            .doc(FirebaseAuth.instance.currentUser!.uid)
                            .update({
                          'storename': storeName,
                          'storelogo': storeLogo,
                        });
                        Navigator.pop(context);
                      }
                    },
                    child: const Text('Save'),
                  ),
                ],
              ),
            ),
          );
        },
      ),
    );
  }
}
