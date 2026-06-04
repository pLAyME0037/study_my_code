import 'dart:convert';

import 'package:pos_app/api_connection/api_connection.dart';
import 'package:pos_app/models/order.dart';
import 'package:pos_app/screens/order/order_details.dart';
import 'package:pos_app/controllers/current_user.dart';
import 'package:flutter/material.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:get/get.dart';
import 'package:http/http.dart' as http;
import 'package:intl/intl.dart';


class AdminGetAllOrdersScreen extends StatelessWidget
{
  final currentOnlineUser = Get.put(CurrentUser());


  Future<List<Order>> getAllOrdersList() async {
    List<Order> ordersList = [];

    try {
      var res = await http.post(
          Uri.parse(API.adminGetAllOrders),
          body:
          {

          }
      );

      if (res.statusCode == 200) {
        var responseBodyOfCurrentUserOrdersList = jsonDecode(res.body);

        if (responseBodyOfCurrentUserOrdersList['success'] == true) {
          (responseBodyOfCurrentUserOrdersList['allOrdersData'] as List).forEach((eachOrderData) {
            ordersList.add(Order.fromJson(eachOrderData));
          });
        }
      }
      else {
        Fluttertoast.showToast(msg: "Status Code is not 200");
      }
    }
    catch(errorMsg) {
      Fluttertoast.showToast(msg: "Error:: " + errorMsg.toString());
    }

    return ordersList;
  }


  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: Colors.black,
      body: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [

          Padding(
            padding: const EdgeInsets.fromLTRB(16, 24, 8, 0),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [

                Column(
                  children: [
                    Image.asset(
                      "images/orders_icon.png",
                      width: 140,
                    ),
                    const Text(
                      "All New Orders",
                      style: TextStyle(
                        color: Colors.purpleAccent,
                        fontSize: 24,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    const Padding(
                      padding: EdgeInsets.symmetric(horizontal: 30.0),
                      child: Text(
                        "Here are your successfully placed orders.",
                        style: TextStyle(
                          fontSize: 16,
                          color: Colors.white,
                          fontWeight: FontWeight.w400,
                        ),
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),

          Expanded(
            child: displayOrdersList(context),
          ),

        ],
      ),
    );
  }

  Widget displayOrdersList(context)
  {
    return FutureBuilder(
      future: getAllOrdersList(),
      builder: (context, AsyncSnapshot<List<Order>> dataSnapshot)
      {
        if(dataSnapshot.connectionState == ConnectionState.waiting)
        {
          return Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: const [
              Center(
                child: Text(
                  "Connection Waiting...",
                  style: TextStyle(color: Colors.grey,),
                ),
              ),
              Center(
                child: CircularProgressIndicator(),
              ),
            ],
          );
        }
        if(dataSnapshot.data == null)
        {
          return Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: const [
              Center(
                child: Text(
                  "No orders found yet...",
                  style: TextStyle(color: Colors.grey,),
                ),
              ),
              Center(
                child: CircularProgressIndicator(),
              ),
            ],
          );
        }
        if(dataSnapshot.data!.length > 0)
        {
          List<Order> orderList = dataSnapshot.data!;

          return ListView.separated(
            padding: const EdgeInsets.all(16),
            separatorBuilder: (context, index)
            {
              return const Divider(
                height: 1,
                thickness: 1,
              );
            },
            itemCount: orderList.length,
            itemBuilder: (context, index)
            {
              Order eachOrderData = orderList[index];

              return Card(
                color: Colors.white24,
                child: Padding(
                  padding: const EdgeInsets.all(18),
                  child: ListTile(
                    onTap: ()
                    {
                      Get.to(OrderDetailsScreen(
                        clickedOrderInfo: eachOrderData,
                      ));
                    },
                    title: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text(
                          "Order ID # " + eachOrderData.order_id.toString(),
                          style: const TextStyle(
                            fontSize: 16,
                            color: Colors.grey,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                        Text(
                          "Amount: \$ " + eachOrderData.totalAmount.toString(),
                          style: const TextStyle(
                            fontSize: 16,
                            color: Colors.purpleAccent,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ],
                    ),
                    trailing: Row(
                      mainAxisSize: MainAxisSize.min,
                      children: [

                        Column(
                          crossAxisAlignment: CrossAxisAlignment.end,
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [

                            Text(
                              DateFormat(
                                  "dd MMMM, yyyy"
                              ).format(eachOrderData.dateTime!),
                              style: const TextStyle(
                                color: Colors.grey,
                              ),
                            ),

                            const SizedBox(height: 4),

                            Text(
                              DateFormat(
                                  "hh:mm a"
                              ).format(eachOrderData.dateTime!),
                              style: const TextStyle(
                                color: Colors.grey,
                              ),
                            ),

                          ],
                        ),

                        const SizedBox(width: 6),

                        const Icon(
                          Icons.navigate_next,
                          color: Colors.purpleAccent,
                        ),

                      ],
                    ),
                  ),
                ),
              );
            },
          );
        }
        else
        {
          return Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: const [
              Center(
                child: Text(
                  "Nothing to show...",
                  style: TextStyle(color: Colors.grey,),
                ),
              ),
              Center(
                child: CircularProgressIndicator(),
              ),
            ],
          );
        }
      },
    );
  }
}
