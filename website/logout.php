<?php
    session_start();
    unset($_SESSION['UserName']);
    unset($_SESSION['FirstName']);
    unset($_SESSION['LastName']);
    unset($_SESSION['FileName']);
    header("location:main.php");
?>