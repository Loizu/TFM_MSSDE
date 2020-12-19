<?php

    // Prepare variables for database connection
   
    $dbname = "soilmoisturedata_test";
    $dbusername = "esp32Test";
    $dbpassword = "moisture1215!standup!!!";  
    $server = "localhost"; 

    // Connect to database

    $dbconnect = new mysqli($server, $dbusername, $dbpassword, $dbname);

    // Prepare SQL statement

     $sql = "INSERT INTO soilmoisturedata_test.sensorsdata_test1 (sensor_type, sensor_id, board_id, time_s, value)
     VALUES ('".$_GET["sensor_type"]."', '".$_GET["sensor_id"]."', '".$_GET["board_id"]."', '".$_GET["time_s"]."', '".$_GET["value"]."')";

    // Execute SQL statement

    mysqli_query($dbconnect, $sql);

?>