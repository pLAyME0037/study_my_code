<?php
include '../connection.php';

//POST = send/save data to mysql db
//GET = retrieve/read data from mysql db

$userName = $_POST['user_name'];
$userEmail = $_POST['user_email'];
$userPassword = md5($_POST['user_password']); 

$sqlQuery = "INSERT INTO users_table SET user_name = '$userName', user_email = '$userEmail', user_password = '$userPassword'";

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery)
{
    echo json_encode(array("success"=>true));
}
else
{
    echo json_encode(array("success"=>false));
}

