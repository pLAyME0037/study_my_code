<?php

include '../connection.php';

$user_id = $_POST['user_id'];
$item_id = $_POST['item_id'];

$sqlQuery = "SELECT * FROM favorite_table WHERE user_id = '$user_id' AND item_id = '$item_id'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery->num_rows > 0) 
{
    echo json_encode(array("favoriteFound"=>true));
}
else
{
    echo json_encode(array("favoriteFound"=>false));
}
