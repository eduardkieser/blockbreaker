<?php
    session_start();
    if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST['main_id'])){
        $conn1 = mysqli_connect("localhost","root","","block_breaker");
        $sel1 = "SELECT * FROM user_image";
        $que1 = mysqli_query($conn1, $sel1);
    if($que1){
        mysqli_data_seek($que1, $_POST['main_id'] - 1);
        $row1 = mysqli_fetch_array($que1);
        if($row1){
            $_SESSION['FileName'] = $row1['Name'];
            echo $_SESSION['FileName'];
            header("location:./User/User_data/User_data.php");
        }
    }
    }
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
            
            $counter = 1;
            while($row = mysqli_fetch_array($que)){
                if($row['Name'] != ''){
                $MainId = $counter;
                $HeadId = 'Name' . $counter;
                ?>
            <div class = "img1">
                <img class = "image" src="./User/<?=$row['IMG']?>">
                <div class = "Name">
                    <h10 id="<?php echo $HeadId; ?>" class = "Name1"><?=$row['Name']?></h10>
                    <form id="<?php echo $MainId; ?>" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
                            <input type="hidden" name="main_id" value="<?php echo $MainId; ?>">
                            </form>

                            <script>
                                document.addEventListener('DOMContentLoaded', function () {
                                 var form = document.getElementById('<?php echo $MainId; ?>');
                                 var h10 = document.getElementById('<?php echo $HeadId; ?>');

                                 console.log(form); // Log the form element to the console

                                if (form && h10) {
                                    h10.addEventListener('click', function () {
                                        form.submit();
                                    });
                                } else {
                                    console.error('Form submission error: Element not found');
                                }
                            });
        </script>
                </div>
            </div>    
            <?php    
                }
            $counter = $counter + 1;
            }
            ?>
        </div>
</body>