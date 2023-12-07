<?php
    session_start();
    $conn = mysqli_connect("localhost","root","","block_breaker");
    if($conn->connect_error)
    {
        die('Connection Failed');
    }
    $FileName = $_SESSION['FileName'];
    if(isset($_SESSION['UserName']) && $_SESSION['UserName'] !== null){
        $username = $_SESSION['UserName'];
    }

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
        <h8 class = "Name1"><?php echo $_SESSION['FileName']; ?></h8>
        <div class = "right">
            <?php if(isset($_SESSION['UserName']) && $_SESSION['UserName'] !== null) 
            {
            ?>
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
            <h9 class = "username-box"><?=$_SESSION['UserName'];?></h9>
            <?php 
            }
            ?>
            <a href="../../main.php" class="home">
                Home
            </a>
        </div>
    </div>

    <div class="Data">
        <h4>This is the Image:- </h4>
        <?php 
            if(isset($_SESSION['UserName']) && $_SESSION['UserName'] !== null){
                $sel = "SELECT * FROM user_image WHERE username = '$username' AND Name = '$FileName'";
            }
            else{
                $sel = "SELECT * FROM user_image WHERE Name = '$FileName'";
            }
            $que = mysqli_query($conn, $sel);
            
            if($que){
                $row = mysqli_fetch_array($que);
                $_SESSION['STL'] = $row['STL'];
                $STL = str_replace('STL_Files/', '', $row['STL']);
                ?>
            <img class="image" src="../<?php echo $row['IMG']?>">    
            <?php
            }
        ?>
        <h4>This is the STL File:- </h4>
        <a href="download.php"><?php echo $STL; ?></a>
        <?php  
           if(isset($_SESSION['UserName']) && $_SESSION['UserName'] !== null){ 
            $sel1 = "SELECT * FROM user_data WHERE username = '$username' AND Name = '$FileName' ORDER BY Test ASC";
           }
           else{
            $sel1 = "SELECT * FROM user_data WHERE Name = '$FileName' ORDER BY Test ASC";
           }
            $que1 = mysqli_query($conn, $sel1);

            while($row1 = mysqli_fetch_array($que1)){
                ?>
                 <h4>This is the <?php echo $row1['Test'] ?> Test:-</h4>
                 <h6> Impact Test = <?php echo $row1['Impact'] ?> J</h6>
                 <h6> Tensile Test = <?php echo $row1['Tensile'] ?> MPa</h6>
                 <h6> Temperature = <?php echo $row1['Temperature'] ?> C</h6>
                <?php
            }
        ?>
    </div>    
</body>