<?php
include '../connection.php';

$cartID = $_POST['cart_id'];

$sqlQuery = "DELETE FROM cart_table WHERE cart_id = '$cartID'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

