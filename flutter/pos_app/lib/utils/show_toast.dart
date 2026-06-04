import 'package:flutter/material.dart';
import 'package:get/get.dart';

void showToast(String msg) {
  Get.snackbar(
    "",
    msg,
    snackPosition: SnackPosition.BOTTOM,
    backgroundColor: Colors.black54,
    colorText: Colors.white,
  );
}
