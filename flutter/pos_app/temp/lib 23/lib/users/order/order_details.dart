import 'dart:convert';

import 'package:clothes_app/api_connection/api_connection.dart';
import 'package:clothes_app/users/model/order.dart';
import 'package:flutter/material.dart';
import 'package:intl/intl.dart';


class OrderDetailsScreen extends StatefulWidget
{
  final Order? clickedOrderInfo;

  OrderDetailsScreen({this.clickedOrderInfo,});

  @override
  State<OrderDetailsScreen> createState() => _OrderDetailsScreenState();
}



class _OrderDetailsScreenState extends State<OrderDetailsScreen>
{
  @override
  Widget build(BuildContext context)
  {
    return Scaffold(
      backgroundColor: Colors.black,
      appBar: AppBar(
        backgroundColor: Colors.black,
        title: Text(
          DateFormat("dd MMMM, yyyy - hh:mm a").format(widget.clickedOrderInfo!.dateTime!),
          style: const TextStyle(fontSize: 14),
        ),
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [

              //display items belongs to clicked order
              displayClickedOrderItems(),

              const SizedBox(height: 16,),

              //phoneNumber
              showTitleText("Phone Number:"),
              const SizedBox(height: 8,),
              showContentText(widget.clickedOrderInfo!.phoneNumber!),

              const SizedBox(height: 26,),

              //Shipment Address
              showTitleText("Shipment Address:"),
              const SizedBox(height: 8,),
              showContentText(widget.clickedOrderInfo!.shipmentAddress!),

              const SizedBox(height: 26,),

              //delivery
              showTitleText("Delivery System:"),
              const SizedBox(height: 8,),
              showContentText(widget.clickedOrderInfo!.deliverySystem!),

              const SizedBox(height: 26,),

              //payment
              showTitleText("Payment System:"),
              const SizedBox(height: 8,),
              showContentText(widget.clickedOrderInfo!.paymentSystem!),

              const SizedBox(height: 26,),

              //note
              showTitleText("Note to Seller:"),
              const SizedBox(height: 8,),
              showContentText(widget.clickedOrderInfo!.note!),

              const SizedBox(height: 26,),

              //total amount
              showTitleText("Total Amount:"),
              const SizedBox(height: 8,),
              showContentText(widget.clickedOrderInfo!.totalAmount.toString()),

              const SizedBox(height: 26,),

              //payment proof
              showTitleText("Proof of Payment/Transaction:"),
              const SizedBox(height: 8,),
              FadeInImage(
                width: MediaQuery.of(context).size.width * 0.8,
                fit: BoxFit.fitWidth,
                placeholder: const AssetImage("images/place_holder.png"),
                image: NetworkImage(
                  API.hostImages + widget.clickedOrderInfo!.image!,
                ),
                imageErrorBuilder: (context, error, stackTraceError)
                {
                  return const Center(
                    child: Icon(
                      Icons.broken_image_outlined,
                    ),
                  );
                },
              ),

            ],
          ),
        ),
      ),
    );
  }

  Widget showTitleText(String titleText)
  {
    return Text(
      titleText,
      style: const TextStyle(
        fontWeight: FontWeight.bold,
        fontSize: 20,
        color: Colors.grey,
      ),
    );
  }

  Widget showContentText(String contentText)
  {
    return Text(
      contentText,
      style: const TextStyle(
        fontSize: 14,
        color: Colors.white38,
      ),
    );
  }

  Widget displayClickedOrderItems()
  {
    List<String> clickedOrderItemsInfo = widget.clickedOrderInfo!.selectedItems!.split("||");

    return Column(
      children: List.generate(clickedOrderItemsInfo.length, (index)
      {
        Map<String, dynamic> itemInfo = jsonDecode(clickedOrderItemsInfo[index]);

        return Container(
          margin: EdgeInsets.fromLTRB(
            16,
            index == 0 ? 16 : 8,
            16,
            index == clickedOrderItemsInfo.length - 1 ? 16 : 8,
          ),
          decoration: BoxDecoration(
            borderRadius: BorderRadius.circular(20),
            color: Colors.white24,
            boxShadow:
            const [
              BoxShadow(
                offset: Offset(0, 0),
                blurRadius: 6,
                color: Colors.black26,
              ),
            ],
          ),
          child: Row(
            children: [

              //image
              ClipRRect(
                borderRadius: const BorderRadius.only(
                  topLeft: Radius.circular(20),
                  bottomLeft: Radius.circular(20),
                ),
                child: FadeInImage(
                  height: 150,
                  width: 130,
                  fit: BoxFit.cover,
                  placeholder: const AssetImage("images/place_holder.png"),
                  image: NetworkImage(
                    itemInfo["image"],
                  ),
                  imageErrorBuilder: (context, error, stackTraceError)
                  {
                    return const Center(
                      child: Icon(
                        Icons.broken_image_outlined,
                      ),
                    );
                  },
                ),
              ),

              //name
              //size
              //price
              Expanded(
                child: Padding(
                  padding: const EdgeInsets.all(8),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [

                      //name
                      Text(
                        itemInfo["name"],
                        maxLines: 1,
                        overflow: TextOverflow.ellipsis,
                        style: const TextStyle(
                          fontSize: 18,
                          color: Colors.white70,
                          fontWeight: FontWeight.bold,
                        ),
                      ),

                      const SizedBox(height: 16),

                      //size + color
                      Text(
                        itemInfo["size"].replaceAll("[", "").replaceAll("]", "") + "\n" + itemInfo["color"].replaceAll("[", "").replaceAll("]", ""),
                        maxLines: 2,
                        overflow: TextOverflow.ellipsis,
                        style: const TextStyle(
                          color: Colors.white54,
                        ),
                      ),

                      const SizedBox(height: 16),

                      //price
                      Text(
                        "\$ " + itemInfo["totalAmount"].toString(),
                        overflow: TextOverflow.ellipsis,
                        style: const TextStyle(
                          fontSize: 18,
                          color: Colors.purpleAccent,
                          fontWeight: FontWeight.bold,
                        ),
                      ),

                      Text(
                        itemInfo["price"].toString() + " x "
                            + itemInfo["quantity"].toString()
                            + " = " + itemInfo["totalAmount"].toString(),
                        style: const TextStyle(
                          color: Colors.grey,
                          fontSize: 12,
                        ),
                      ),


                    ],
                  ),
                ),
              ),

              //quantity
              Padding(
                padding: const EdgeInsets.all(8.0),
                child: Text(
                  "Q: " + itemInfo["quantity"].toString(),
                  style: const TextStyle(
                    fontSize: 24,
                    color: Colors.purpleAccent,
                  ),
                ),
              ),


            ],
          ),
        );
      }),
    );
  }
}
