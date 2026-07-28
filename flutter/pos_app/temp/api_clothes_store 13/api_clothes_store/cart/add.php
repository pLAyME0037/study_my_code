<?php
include '../connection.php';

$user_id = $_POST['user_id'];
$item_id = $_POST['item_id'];
$quantity = $_POST['quantity'];
$color = $_POST['color'];
$size = $_POST['size'];

$sqlQuery = "INSERT INTO cart_table SET user_id = '$user_id', item_id = '$item_id', quantity = '$quantity', color = '$color', size = '$size'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

