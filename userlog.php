<!DOCTYPE html>
<html>
<head>
  <title>Fetch image from database in PHP</title>
</head>
<body>

<h2>All Records</h2>

<table cellspacing="5" cellpadding="5"  >
      <tr class = "rib"> 
        <td>image</td>
        <td>ID</td> 
        <td>RFID </td> 
        <td>Location</td> 
        <td>NAME</td> 
        <td>MATNO</td>
        <td>COURSE</td> 
        <td>Timestamp</td> 
      </tr>

<?php
$servername = "localhost";

// REPLACE with your Database name
$dbname = "id17059011_esp_32";
// REPLACE with Database user
$username = "id17059011_esp_board";
// REPLACE with Database user password
$password = "124456AA@@1f";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
//$sql = "SELECT image, id, sensor, location, value1, value2, value3, reading_time FROM SensorData ORDER BY id DESC";
$records = mysqli_query($db, "SELECT image, id, sensor, location, value1, value1, value3, reading_time FROM SensorData ORDER BY id DESC");

// fetch data from database

while($data = mysqli_fetch_array($records))
{
?>
  <tr>
    <td><img src="<?php echo $data['images']; ?>" width="100" height="100"></td>
    <td><?php echo $data['id']; ?></td>
    <td><?php echo $data['sensor']; ?></td>
    <td><?php echo $data['location']; ?></td>
    <td><?php echo $data['value1']; ?></td>
    <td><?php echo $data['value2']; ?></td>
    <td><?php echo $data['value3']; ?></td>
    <td><?php echo $data['reading_time']; ?></td>
    
  </tr>	
<?php
}
?>

</table>

<?php mysqli_close($db);  // close connection ?>

</body>
</html>