<?php
include '../connection.php';


$minRating = 4.4;
$limitClothItems = 5;

$sqlQuery = "Select * FROM items_table WHERE rating>= '$minRating' ORDER BY rating DESC LIMIT $limitClothItems";
                                                                    //5 or less than 5 newly available top rated clothes item
                                                                    //not greater than 5

$resultOfQuery = $connectNow->query($sqlQuery);

if($resultOfQuery->num_rows > 0) 
{
    $clothItemsRecord = array();
    while($rowFound = $resultOfQuery->fetch_assoc())
    {
        $clothItemsRecord[] = $rowFound;
    }

    echo json_encode(
        array(
            "success"=>true,
            "clothItemsData"=>$clothItemsRecord,
        )
    );
}
else 
{
    echo json_encode(array("success"=>false));
}