<?php
session_start();
$conn = mysqli_connect("localhost","root","","block_breaker");
if($conn->connect_error)
{
    die('Connection Failed');
}
if(isset($_POST["btn"])){
    $EmailID = $_POST["EmailID"];
    $pwd = $_POST["password"];
    $query = "SELECT * FROM user_info WHERE EmailID = '$EmailID'";
    $data = mysqli_query($conn, $query);
    $row = mysqli_fetch_assoc($data);
    $total = mysqli_num_rows($data);
    if($total != 1){
        header("Location: Email.php?error=Not found");
    }
    else if($row['password'] != $pwd){
        header("Location: Email.php?error=Wrong Password");
    }
    else if($row['password'] == $pwd){
        $_SESSION['FirstName'] = $row['FirstName'];
        $_SESSION['LastName'] = $row['LastName'];
        $_SESSION['UserName'] = $row['UserName'];
        header("Location: ../User/user.php");
    }
    
}
?>