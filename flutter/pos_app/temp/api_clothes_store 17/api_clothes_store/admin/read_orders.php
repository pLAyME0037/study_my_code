<?php
include '../connection.php';

$sqlQuery = "SELECT * FROM orders_table WHERE status = 'new' ORDER BY dateTime DESC";  

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
            "allOrdersData"=>$ordersRecord,
        )
    );
}
else 
{
    echo json_encode(array("success"=>false));
}


