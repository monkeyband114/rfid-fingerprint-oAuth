<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "id17059011_esp_32";
// REPLACE with Database user
$username = "id17059011_esp_board";
// REPLACE with Database user password
$password = "124456AA@@1f";


$api_key_value = "fyguhjbjvjsvhsv";

$api_key= $bmp280 = $presure = $mq2 = $mq3 = $mq6 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor = test_input($_POST["bmp280"]);
        $location = test_input($_POST["presure"]);
        $value1 = test_input($_POST["mq2"]);
        $value2 = test_input($_POST["mq3"]);
        $value3 = test_input($_POST["mq6"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (sensor, location, value1, value2, value3)
        VALUES ('" . $bmp280 . "', '" . $presure . "', '" . $mq2 . "', '" . $mq3 . "', '" . $mq6 . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
         
        $conn->close();
        
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
    
    
        }
       
       

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}



