<?php
include '../connection.php';

$user_id = $_POST['user_id'];
$item_id = $_POST['item_id'];

$sqlQuery = "INSERT INTO favorite_table SET user_id = '$user_id', item_id = '$item_id'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

