<?php
include '../connection.php';

$sqlQuery = "Select * FROM items_table ORDER BY item_id DESC";

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