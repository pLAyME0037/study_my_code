<?php
include '../connection.php';

$currentOnlineUserID = $_POST["currentOnlineUserID"];

$sqlQuery = "SELECT * FROM orders_table WHERE user_id = '$currentOnlineUserID' AND status = 'new' ORDER BY dateTime DESC";  

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery->num_rows > 0) 
{
    $ordersRecord = array();
    while($rowFound = $resultOfQuery->fetch_assoc())
    {
        $ordersRecord[] = $rowFound;
    }

    echo json_encode(
        array(
            "success"=>true,
            "currentUserOrdersData"=>$ordersRecord,
        )
    );
}
else 
{
    echo json_encode(array("success"=>false));
}


