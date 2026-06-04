<?php
include '../connection.php';


$userID = $_POST["user_id"];
$selectedItems = $_POST["selectedItems"];
$deliverySystem = $_POST["deliverySystem"];
$paymentSystem = $_POST["paymentSystem"];
$note = $_POST["note"];
$totalAmount = $_POST["totalAmount"];
$image = $_POST["image"];
$status = $_POST["status"];
$shipmentAddress = $_POST["shipmentAddress"];
$phoneNumber = $_POST["phoneNumber"];
$imageFileBase64 = $_POST["imageFile"];


$sqlQuery = "INSERT INTO orders_table SET user_id='$userID', selectedItems='$selectedItems', deliverySystem='$deliverySystem', paymentSystem='$paymentSystem', note='$note', totalAmount='$totalAmount', image='$image', status='$status', shipmentAddress='$shipmentAddress', phoneNumber='$phoneNumber'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    //upload image to server
    $imageFileOfTransactionProof = base64_decode($imageFileBase64);
    file_put_contents("../transactions_proof_images/".$image, $imageFileOfTransactionProof);

    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}