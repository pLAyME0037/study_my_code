<?php
include '../connection.php';

$itemName = $_POST['name'];
$itemRating = $_POST['rating']; 
$itemTags = $_POST['tags'];
$itemPrice = $_POST['price'];
$itemSizes = $_POST['sizes'];
$itemColors = $_POST['colors'];
$itemDescription = $_POST['description'];
$itemImage = $_POST['image'];

$sqlQuery = "INSERT INTO items_table SET name='$itemName', rating='$itemRating', tags='$itemTags', price='$itemPrice', sizes='$itemSizes', colors='$itemColors', description='$itemDescription', image='$itemImage'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}