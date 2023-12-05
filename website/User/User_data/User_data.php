<?php
    session_start();
    $conn = mysqli_connect("localhost","root","","block_breaker");
    if($conn->connect_error)
    {
        die('Connection Failed');
    }
   # $FileName = $_SESSION['FileName'];
    #$username = $_SESSION['UserName'];

?>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>
        Block Breaker
    </title>
    <link rel="stylesheet" href="user_data.css?v=<?=time();?>">
</head>
<body>
    <div class = "Name">
        <h8 class = "Name1">TEST BLOCK<?php # echo $_SESSION['FileName']; ?></h8>
        <div class = "right">
            <a href="../user.php" class="user_page">
                <button class="user_page1">
                    User Page
                </button>
            </a>
            <a href="../../logout.php" class="logout">
                <button class="logout1">
                    Logout
                </button>
            </a>
            <h9 class = "username-box"> taheryusuf<?#=$_SESSION['UserName'];?></h9>
            <a href="../../main.php" class="home">
                Home
            </a>
        </div>
    </div>

    <div class="Data">
        <h4>This is the Image:- </h4>
        <?php 
            $sel = "SELECT * FROM user_image WHERE username = 'taheryusuf' AND Name = 'KUNAI'";
            $que = mysqli_query($conn, $sel);
            
            if($que){
                $row = mysqli_fetch_array($que);
                $_SESSION['STL'] = $row['STL'];
                $STL = str_replace('STL_Files/', '', 'test.stl');
                ?>
            <img class="image" src="../Image/test.JPG">    
            <?php
            }
        ?>
        <h4>This is the STL File:- </h4>
        <a href="download.php"><?php echo $STL; ?></a>
        <h4>This is the First Test:-</h4>
        <h6> Impact Test = 0.99 J</h6>
        <h6> Tensile Test = 80.67 MPa</h6>
        <h6> Temperature = 21 C</h6>
    </div>    
</body>