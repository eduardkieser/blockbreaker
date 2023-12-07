<?php
    session_start();
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $textValue = $_POST["text"];
        $conn = mysqli_connect("localhost","root","","block_breaker");
        $username = $_SESSION['UserName'];
        $sel = "SELECT * FROM user_data WHERE username = '$username' AND Name = '$textValue' ORDER BY Test DESC LIMIT 1";
        $que = mysqli_query($conn, $sel);
        $row = mysqli_fetch_array($que);
        $test = $row['Test'];
        $test = $test + 1;
        $sel = "SELECT * FROM user_data ORDER BY ID DESC LIMIT 1";
        $que = mysqli_query($conn, $sel);
        $row1 = mysqli_fetch_array($que);
        $ID = $row1['ID'];
        $ID = $ID + 1;
        $sel = "SELECT * FROM new_data WHERE username = '$username'";
        $que = mysqli_query($conn, $sel);
        $row = mysqli_fetch_array($que);
        $Impact = $row['Impact'];
        echo $Impact;
        $Tensile = $row['Tensile'];
        $temperature = $row['Temperature'];
        $sel = "INSERT INTO user_data (ID, UserName, Name, Test, Impact, Tensile, Temperature) VALUES ('$ID', '$username', '$textValue', '$test', '$Impact', '$Tensile', '$temperature')"; 
        $que = mysqli_query($conn, $sel);
        $sel = "DELETE FROM user_image WHERE Name = ''";
        mysqli_query($conn, $sel);
    }
?>