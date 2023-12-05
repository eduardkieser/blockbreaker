<?php
$conn = mysqli_connect("localhost","root","","block_breaker");
if($conn->connect_error)
{
    die('Connection Failed');
}
if(isset($_POST["btn"])){
    $FirstName = $_POST["first_name"];
    $LastName = $_POST["last_name"];
    $EmailID = $_POST["email_id"];
    $UserName = $_POST["username"];
    $password = $_POST["password"];
    $query = "INSERT INTO user_info VALUES('', '$FirstName', '$LastName', '$EmailID', '$UserName', '$password')";
    mysqli_query($conn, $query);
    $conn -> close();
    header("Location: ../User/user.php");
}
?>