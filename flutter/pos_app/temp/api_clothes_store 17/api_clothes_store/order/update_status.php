<?php
include '../connection.php';

$orderID = $_POST['order_id'];

$sqlQuery = "UPDATE orders_table SET status = 'received' WHERE order_id = '$orderID'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

