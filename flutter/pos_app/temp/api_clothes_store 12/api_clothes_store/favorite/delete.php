<?php
include '../connection.php';

$user_id = $_POST['user_id'];
$item_id = $_POST['item_id'];

$sqlQuery = "DELETE FROM favorite_table WHERE user_id = '$user_id' AND item_id = '$item_id'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

