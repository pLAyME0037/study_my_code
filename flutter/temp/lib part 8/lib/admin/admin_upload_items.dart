import 'dart:io';

import 'package:flutter/material.dart';
import 'package:get/get.dart';
import 'package:image_picker/image_picker.dart';


class AdminUploadItemsScreen extends StatefulWidget
{


  @override
  State<AdminUploadItemsScreen> createState() => _AdminUploadItemsScreenState();
}





class _AdminUploadItemsScreenState extends State<AdminUploadItemsScreen>
{
  final ImagePicker _picker = ImagePicker();
  XFile? pickedImageXFile;


  //defaultScreen methods
  captureImageWithPhoneCamera() async
  {
    pickedImageXFile = await _picker.pickImage(source: ImageSource.camera);

    Get.back();

    setState(()=> pickedImageXFile);
  }

  pickImageFromPhoneGallery() async
  {
    pickedImageXFile = await _picker.pickImage(source: ImageSource.gallery);

    Get.back();

    setState(()=> pickedImageXFile);
  }

  showDialogBoxForImagePickingAndCapturing()
  {
    return showDialog(
      context: context,
      builder: (context)
      {
        return SimpleDialog(
          backgroundColor: Colors.black,
          title: const Text(
            "Item Image",
            style: TextStyle(
              color: Colors.deepPurple,
              fontWeight: FontWeight.bold,
            ),
          ),
          children: [
            SimpleDialogOption(
              onPressed: ()
              {
                captureImageWithPhoneCamera();
              },
              child: const Text(
                "Capture with Phone Camera",
                style: TextStyle(
                  color: Colors.grey,
                ),
              ),
            ),
            SimpleDialogOption(
              onPressed: ()
              {
                pickImageFromPhoneGallery();
              },
              child: const Text(
                "Pick Image From Phone Gallery",
                style: TextStyle(
                  color: Colors.grey,
                ),
              ),
            ),
            SimpleDialogOption(
              onPressed: ()
              {
                Get.back();
              },
              child: const Text(
                "Cancel",
                style: TextStyle(
                  color: Colors.red,
                ),
              ),
            ),
          ],
        );
      }
    );
  }
  //defaultScreen methods ends here

  Widget defaultScreen()
  {
    return Scaffold(
      appBar: AppBar(
        flexibleSpace: Container(
          decoration: const BoxDecoration(
            gradient: LinearGradient(
              colors: [
                Colors.black54,
                Colors.deepPurple,
              ],
            ),
          ),
        ),
        automaticallyImplyLeading: false,
        title: const Text(
            "Welcome Admin"
        ),
        centerTitle: true,
      ),
      body: Container(
        decoration: const BoxDecoration(
          gradient: LinearGradient(
            colors: [
              Colors.black54,
              Colors.deepPurple,
            ],
          ),
        ),
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              const Icon(
                Icons.add_photo_alternate,
                color: Colors.white54,
                size: 200,
              ),

              //button
              Material(
                color: Colors.black38,
                borderRadius: BorderRadius.circular(30),
                child: InkWell(
                  onTap: ()
                  {
                    showDialogBoxForImagePickingAndCapturing();
                  },
                  borderRadius: BorderRadius.circular(30),
                  child: const Padding(
                    padding: EdgeInsets.symmetric(
                      vertical: 10,
                      horizontal: 28,
                    ),
                    child: Text(
                      "Add New Item",
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: 16,
                      ),
                    ),
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget uploadItemFormScreen()
  {
    return Scaffold(
      backgroundColor: Colors.black,
      body: ListView(
        children: [

          //image
          Container(
            height: MediaQuery.of(context).size.height * 0.4,
            width: MediaQuery.of(context).size.width * 0.8,
            decoration: BoxDecoration(
              image: DecorationImage(
                image: FileImage(
                  File(pickedImageXFile!.path),
                ),
                fit: BoxFit.cover,
              ),
            ),
          ),

        ],
      ),
    );
  }




  @override
  Widget build(BuildContext context)
  {
    return pickedImageXFile == null ? defaultScreen() : uploadItemFormScreen();
  }
}
