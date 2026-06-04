<?php
include '../connection.php';

$currentOnlineUserID = $_POST["currentOnlineUserID"];

$sqlQuery = "SELECT * FROM cart_table CROSS JOIN items_table WHERE cart_table.user_id = '$currentOnlineUserID' AND cart_table.item_id = items_table.item_id";  

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery->num_rows > 0) 
{
    $cartRecord = array();
    while($rowFound = $resultOfQuery->fetch_assoc())
    {
        $cartRecord[] = $rowFound;
    }

    echo json_encode(
        array(
            "success"=>true,
            "currentUserCartData"=>$cartRecord,
        )
    );
}
else 
{
    echo json_encode(array("success"=>false));
}


