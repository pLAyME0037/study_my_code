<?php
include '../connection.php';

$cartID = $_POST['cart_id'];
$quantity = $_POST['quantity'];

$sqlQuery = "UPDATE cart_table SET quantity = '$quantity' WHERE cart_id = '$cartID'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

