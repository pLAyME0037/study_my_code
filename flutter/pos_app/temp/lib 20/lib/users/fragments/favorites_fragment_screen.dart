import 'dart:convert';

import 'package:clothes_app/users/model/favorite.dart';
import 'package:clothes_app/users/userPreferences/current_user.dart';
import 'package:flutter/material.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:get/get.dart';
import 'package:http/http.dart' as http;
import '../../api_connection/api_connection.dart';


class FavoritesFragmentScreen extends StatelessWidget
{
  final currentOnlineUser = Get.put(CurrentUser());

  Future<List<Favorite>> getCurrentUserFavoriteList() async
  {
    List<Favorite> favoriteListOfCurrentUser = [];

    try
    {
      var res = await http.post(
          Uri.parse(API.readFavorite),
          body:
          {
            "user_id": currentOnlineUser.user.user_id.toString(),
          }
      );

      if (res.statusCode == 200)
      {
        var responseBodyOfCurrentUserFavoriteListItems = jsonDecode(res.body);

        if (responseBodyOfCurrentUserFavoriteListItems['success'] == true)
        {
          (responseBodyOfCurrentUserFavoriteListItems['currentUserFavoriteData'] as List).forEach((eachCurrentUserFavoriteItemData)
          {
            favoriteListOfCurrentUser.add(Favorite.fromJson(eachCurrentUserFavoriteItemData));
          });
        }
      }
      else
      {
        Fluttertoast.showToast(msg: "Status Code is not 200");
      }
    }
    catch(errorMsg)
    {
      Fluttertoast.showToast(msg: "Error:: " + errorMsg.toString());
    }

    return favoriteListOfCurrentUser;
  }

  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [

          const Padding(
            padding: EdgeInsets.fromLTRB(16, 24, 8, 8),
            child: Text(
              "My Favorite List:",
              style: TextStyle(
                color: Colors.purpleAccent,
                fontSize: 30,
                fontWeight: FontWeight.bold,
              ),
            ),
          ),

          const Padding(
            padding: EdgeInsets.fromLTRB(16, 24, 8, 8),
            child: Text(
              "Order these best clothes for yourself now.",
              style: TextStyle(
                color: Colors.grey,
                fontSize: 16,
                fontWeight: FontWeight.w300,
              ),
            ),
          ),

          const SizedBox(height: 24),

          //displaying favoriteList
          favoriteListItemDesignWidget(context),

        ],
      ),
    );
  }

  favoriteListItemDesignWidget(context)
  {
    return FutureBuilder(
        future: getCurrentUserFavoriteList(),
        builder: (context, AsyncSnapshot<List<Favorite>> dataSnapShot)
        {
          if(dataSnapShot.connectionState == ConnectionState.waiting)
          {
            return const Center(
              child: CircularProgressIndicator(),
            );
          }
          if(dataSnapShot.data == null)
          {
            return const Center(
              child: Text(
                "No favorite item found",
                style: TextStyle(color: Colors.grey,),
              ),
            );
          }
          if(dataSnapShot.data!.length > 0)
          {
            return ListView.builder(
              itemCount: dataSnapShot.data!.length,
              shrinkWrap: true,
              physics: const NeverScrollableScrollPhysics(),
              scrollDirection: Axis.vertical,
              itemBuilder: (context, index)
              {
                Favorite eachFavoriteItemRecord = dataSnapShot.data![index];

                return GestureDetector(
                  onTap: ()
                  {
                    //Get.to(ItemDetailsScreen(itemInfo: eachClothItemRecord));
                  },
                  child: Container(
                    margin: EdgeInsets.fromLTRB(
                      16,
                      index == 0 ? 16 : 8,
                      16,
                      index == dataSnapShot.data!.length - 1 ? 16 : 8,
                    ),
                    decoration: BoxDecoration(
                      borderRadius: BorderRadius.circular(20),
                      color: Colors.black,
                      boxShadow:
                      const [
                        BoxShadow(
                          offset: Offset(0,0),
                          blurRadius: 6,
                          color: Colors.white,
                        ),
                      ],
                    ),
                    child: Row(
                      children: [

                        //name + price
                        //tags
                        Expanded(
                          child: Padding(
                            padding: const EdgeInsets.only(left: 15),
                            child: Column(
                              crossAxisAlignment: CrossAxisAlignment.start,
                              children: [

                                //name and price
                                Row(
                                  children: [

                                    //name
                                    Expanded(
                                      child: Text(
                                        eachFavoriteItemRecord.name!,
                                        maxLines: 2,
                                        overflow: TextOverflow.ellipsis,
                                        style: const TextStyle(
                                          fontSize: 18,
                                          color: Colors.grey,
                                          fontWeight: FontWeight.bold,
                                        ),
                                      ),
                                    ),

                                    //price
                                    Padding(
                                      padding: const EdgeInsets.only(left: 12, right: 12),
                                      child: Text(
                                        "\$ " + eachFavoriteItemRecord.price.toString(),
                                        maxLines: 2,
                                        overflow: TextOverflow.ellipsis,
                                        style: const TextStyle(
                                          fontSize: 18,
                                          color: Colors.purpleAccent,
                                          fontWeight: FontWeight.bold,
                                        ),
                                      ),
                                    ),

                                  ],
                                ),

                                const SizedBox(height: 16,),

                                //tags
                                Text(
                                  "Tags: \n" + eachFavoriteItemRecord.tags.toString().replaceAll("[", "").replaceAll("]", ""),
                                  maxLines: 2,
                                  overflow: TextOverflow.ellipsis,
                                  style: const TextStyle(
                                    fontSize: 12,
                                    color: Colors.grey,
                                  ),
                                ),

                              ],
                            ),
                          ),
                        ),

                        //image clothes
                        ClipRRect(
                          borderRadius: const BorderRadius.only(
                            topRight: Radius.circular(20),
                            bottomRight: Radius.circular(20),
                          ),
                          child: FadeInImage(
                            height: 130,
                            width: 130,
                            fit: BoxFit.cover,
                            placeholder: const AssetImage("images/place_holder.png"),
                            image: NetworkImage(
                              eachFavoriteItemRecord.image!,
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

                      ],
                    ),
                  ),
                );
              },
            );
          }
          else
          {
            return const Center(
              child: Text("Empty, No Data."),
            );
          }
        }
    );
  }
}
