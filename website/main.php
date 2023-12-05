<?php
    session_start();
?>

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>
        Block Breaker
    </title>
    <link rel="stylesheet" href="main.css?v=<?=time();?>">
</head>
<body>
        
        <div class="b1">
            <?php if(isset($_SESSION['UserName'])){
              ?>
             <h9 class = "username-box"> <?=$_SESSION['UserName'];?></h9>
             <a href="./logout.php" class="logout">
              <button class="logout1">
                  Logout
              </button>
          </a> 
          <a href="./User/user.php" class="user_page">
             <button class="user_page1">
                User Page
            </button>
        </a>
           <?php
            }
            else{ ?>
             <a href="./Email/Email.php" class="login_button">
                <button class="b2">
                    Login
                </button>
            </a>  
            <?php
            }
            ?>
            <a href="./main.php" class="home">
                Home
            </a>  
        </div>  
        <div class = "img">
            <?php 
            $output = "";
            $conn = mysqli_connect("localhost","root","","block_breaker");
            $sel = "SELECT * FROM user_image";
            $que = mysqli_query($conn, $sel);
            while($row = mysqli_fetch_array($que)){
                ?>
            <div class = "img1">
                <img class = "image" src="./User/<?=$row['IMG']?>">
                <div class = "Name">
                    <h10><?=$row['Name']?></h10>
                </div>
            </div>    
            <?php    
            }
            ?>
        </div>
</body>