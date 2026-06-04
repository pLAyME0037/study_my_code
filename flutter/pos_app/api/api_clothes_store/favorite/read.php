<?php
include '../connection.php';

$currentOnlineUserID = $_POST["user_id"];

$sqlQuery = "SELECT * FROM favorite_table CROSS JOIN items_table WHERE favorite_table.user_id = '$currentOnlineUserID' AND favorite_table.item_id = items_table.item_id";  

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery->num_rows > 0) 
{
    $favoriteRecord = array();
    while($rowFound = $resultOfQuery->fetch_assoc())
    {
        $favoriteRecord[] = $rowFound;
    }

    echo json_encode(
        array(
            "success"=>true,
            "currentUserFavoriteData"=>$favoriteRecord,
        )
    );
}
else 
{
    echo json_encode(array("success"=>false));
}

